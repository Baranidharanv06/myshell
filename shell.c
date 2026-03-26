\#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_ARGS 64

void execute_command(char **args, char *output_file) {
    pid_t pid = fork();
    if (pid == 0) {
        if (output_file != NULL) {
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                printf("myshell: cannot open file: %s\n", output_file);
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        if (execvp(args[0], args) == -1) {
            printf("myshell: command not found: %s\n", args[0]);
        }
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        printf("myshell: failed to fork\n");
    }
}

char **parse_input(char *input) {
    char **args = malloc(MAX_ARGS * sizeof(char *));
    int i = 0;
    char *token = strtok(input, " ");
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
    return args;
}

void execute_pipe(char *cmd1, char *cmd2) {
    int pipefd[2];
    pipe(pipefd);

    char **args1 = parse_input(cmd1);
    char **args2 = parse_input(cmd2);

    pid_t pid1 = fork();
    if (pid1 == 0) {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        execvp(args1[0], args1);
        exit(1);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]);
        close(pipefd[0]);
        execvp(args2[0], args2);
        exit(1);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    free(args1);
    free(args2);
}

int main() {
    // Colors
    char *prompt = "\033[1;32mmyshell\033[0m\033[1;34m>\033[0m ";

    char *input;

    while (1) {
        // readline handles history and arrow keys
        input = readline(prompt);

        if (input == NULL) {
            printf("\nExiting...\n");
            break;
        }

        if (strlen(input) == 0) {
            free(input);
            continue;
        }

        // Add to history so up arrow works
        add_history(input);

        // Check for pipe
        char *pipe_pos = strchr(input, '|');
        if (pipe_pos != NULL) {
            *pipe_pos = '\0';
            char *cmd1 = input;
            char *cmd2 = pipe_pos + 1;
            while (*cmd2 == ' ') cmd2++;
            execute_pipe(cmd1, cmd2);
            free(input);
            continue;
        }

        // Check for output redirection
        char *output_file = NULL;
        char *redir_pos = strchr(input, '>');
        if (redir_pos != NULL) {
            *redir_pos = '\0';
            output_file = redir_pos + 1;
            while (*output_file == ' ') output_file++;
            char *end = redir_pos - 1;
            while (end > input && *end == ' ') *end-- = '\0';
        }

        char **args = parse_input(input);

        if (strcmp(args[0], "exit") == 0) {
            printf("Bye!\n");
            free(args);
            free(input);
            exit(0);
        }

        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                chdir(getenv("HOME"));
            } else if (chdir(args[1]) != 0) {
                printf("myshell: cd: no such directory: %s\n", args[1]);
            }
            free(args);
            free(input);
            continue;
        }

        execute_command(args, output_file);
        free(args);
        free(input);
    }

    return 0;
}