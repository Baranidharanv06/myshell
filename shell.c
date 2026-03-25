#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 1024

int main() {
    char input[MAX_INPUT];

    while (1) {
        // Print the prompt
        printf("myshell> ");
        fflush(stdout);

        // Read input from user
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\nExiting...\n");
            break;
        }

        // Remove the newline at the end
        input[strcspn(input, "\n")] = 0;

        // For now just echo what was typed
        printf("You typed: %s\n", input);
    }

    return 0;
}