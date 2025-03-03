#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_CMD_LEN 1024
#define MAX_ARG_COUNT 100

// Function to parse the input command into individual arguments
void parse_input(char *input, char **args) {
    int i = 0;
    char *token = strtok(input, " \t\n");
    
    while (token != NULL) {
        args[i] = token;
        i++;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;  // Null-terminate the argument list
}

// Function to execute a single command (without prompting)
int execute_command(char *input) {
    char *args[MAX_ARG_COUNT];
    parse_input(input, args);

    if (execvp(args[0], args) == -1) {
        perror("Error executing command");
        return -1;
    }
    return 0;
}

// Function to handle piping and redirection
void handle_pipes_and_redirection(char *input) {
    char *command = strtok(input, "|");  // Split input by the pipe symbol
    
    int pipefd[2];
    int fd_in = 0;
    
    while (command != NULL) {
        pipe(pipefd);  // Create a pipe for the current command

        if (fork() == 0) {  // Child process
            dup2(fd_in, 0);  // Take input from the previous command (or stdin if first command)
            
            // Handle output redirection (>)
            if (strchr(command, '>')) {
                char *output_file = strchr(command, '>') + 1;
                while (*output_file == ' ') output_file++;  // Skip spaces
                
                int fd_out = open(output_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
                if (fd_out < 0) {
                    perror("Error opening output file");
                    exit(1);
                }
                dup2(fd_out, 1);  // Redirect output to file
                close(fd_out);
            }
            
            // Output to pipe if more commands follow
            if (strtok(NULL, "|") != NULL) {
                dup2(pipefd[1], 1);  // Write output to pipe
            }
            close(pipefd[0]);
            
            // Execute the command
            execute_command(command);
            exit(0);
        } else {  // Parent process
            wait(NULL);  // Wait for child process to finish
            close(pipefd[1]);
            fd_in = pipefd[0];  // Save pipe input for next command
            command = strtok(NULL, "|");  // Get the next command
        }
    }
}

// Function to execute a script file
void execute_script(const char *script_path) {
    FILE *file = fopen(script_path, "r");
    if (file == NULL) {
        perror("Error opening script file");
        exit(1);
    }

    char line[MAX_CMD_LEN];
    while (fgets(line, sizeof(line), file)) {
        // Trim newline characters
        line[strcspn(line, "\n")] = 0;

        // If there's a pipe or redirection, handle it
        if (strchr(line, '|') || strchr(line, '>')) {
            handle_pipes_and_redirection(line);
        } else {
            // Otherwise, run the command normally
            int status = system(line);
            if (status == -1) {
                perror("Error executing command");
            }
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    // Check for script path argument
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <script_path>\n", argv[0]);
        exit(1);
    }

    const char *script_path = argv[1];

    // Execute the provided script
    execute_script(script_path);

    return 0;
}

