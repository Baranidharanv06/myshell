#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

void execute_command(char **args) {
    pid_t pid = fork();

    if (pid == 0) {
        // Child process — run the command
        if (execvp(args[0], args) == -1) {
            printf("myshell: command not found: %s\n", args[0]);
        }
        exit(1);
    } else if (pid > 0) {
        // Parent process — wait for child to finish
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

int main() {
    char input[MAX_INPUT];

    while (1) {
        printf("myshell> ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\nExiting...\n");
            break;
        }

        input[strcspn(input, "\n")] = 0;

        // Skip empty input
        if (strlen(input) == 0) continue;

        // Parse into args
        char **args = parse_input(input);

        // Built-in: exit
        if (strcmp(args[0], "exit") == 0) {
            printf("Bye!\n");
            free(args);
            exit(0);
        }

        // Built-in: cd
        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                chdir(getenv("HOME"));
            } else if (chdir(args[1]) != 0) {
                printf("myshell: cd: no such directory: %s\n", args[1]);
            }
            free(args);
            continue;
        }

        // Execute everything else
        execute_command(args);
        free(args);
    }

    return 0;
}