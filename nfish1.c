#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Check if the file exists
int file_exists(const char *path) {
    return access(path, F_OK) != -1; // returns 0 if the file exists
}

// Execute the script using bash
int execute_script(const char *script_path) {
    pid_t pid = fork();
    if (pid == 0) {
        if (execlp("/bin/bash", "bash", script_path, (char *)NULL) == -1) {
            perror("Exec failed");
            exit(1); // exit if the exec fails
        }
    } else if (pid < 0) {
        perror("Fork failed");
        return -1; // error if fork fails
    }

    // Wait for the child process to complete
    int status;
    wait(&status);

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status); // return the exit status of the script
    } else {
        return -1; // error if the child process didn't exit normally
    }
}

// Main function
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <script_path>\n", argv[0]);
        exit(1); // error if arguments are incorrect
    }

    const char *script_path = argv[1];

    // If the file doesn't exist, print an error and exit
    if (!file_exists(script_path)) {
        fprintf(stderr, "Error: '%s' not found\n", script_path);
        exit(1);
    }

    // Execute the script
    if (execute_script(script_path) != 0) {
        fprintf(stderr, "Error executing script '%s'\n", script_path);
        exit(1); // exit if the script execution fails
    }

    return 0; // successfully executed the script
}

