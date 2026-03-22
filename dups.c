#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {

    if (argc < 4) {
        fprintf(stderr, "Usage: %s <argument>\n", argv[0]);
        return 1;
    }


    if (strcmp(argv[2], ">") == 0) {
        int fd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644); // O_WRONLY - write only, O_CREAT - create if doesnt exist, O_TRUNC - empty file if it isnt empty, 0644 - setting permissions for when creating file, owner can read/write, others only read

        if (fd == -1) {
            perror("Error opening file.");
            return 1;
        }

        int stdout_backup = dup(STDOUT_FILENO);
        if (dup2(fd, STDOUT_FILENO) == -1) {
            perror("Redirection failed.");
            return 1;
        }

        printf("%s\n", argv[1]);

        fflush(stdout); // ensures that is writted before we switch back to terminal

        if (dup2(stdout_backup, STDOUT_FILENO) == -1) {
            perror("Redirection failed.");
            return 1;
        }
        close(fd);
        close(stdout_backup);

        printf("%s\n", argv[1]);

    }
    else if (strcmp(argv[2], ">>") == 0) {
        int fd = open(argv[3], O_WRONLY | O_CREAT | O_APPEND, 0644); // O_APPEND - appends to the end of file

        if (fd == -1) {
            perror("Error opening file.");
            return 1;
        }

        int stdout_backup = dup(STDOUT_FILENO);
        if (dup2(fd, STDOUT_FILENO) == -1) {
            perror("Redirection failed.");
            return 1;
        }

        printf("%s\n", argv[1]);

        fflush(stdout); // ensures that is written before we switch back to terminal

        if (dup2(stdout_backup, STDOUT_FILENO) == -1) {
            perror("Redirection failed.");
            return 1;
        }
        close(fd);
        close(stdout_backup);

        printf("%s\n", argv[1]);

    }
    else if (strcmp(argv[2], "<") == 0) {
        int fd = open(argv[3], O_RDONLY); // R_ONLY - open file for reading only
        
        if (fd == -1) {
            perror("Error opening file.");
            return 1;
        }

        int stdin_backup = dup(STDIN_FILENO);
        if (dup2(fd, STDIN_FILENO) == -1) {
            perror("Redirection failed.");
            return 1;
        }

        char buffer[100];

        fgets(buffer, 100, stdin);
        printf("%s\n", buffer);

        if (dup2(stdin_backup, STDIN_FILENO) == -1) {
            perror("Redirection failed.");
            return 1;
        }
        close(fd);
        close(stdin_backup);

        printf("%s\n", buffer);

    }

    return 0;
}