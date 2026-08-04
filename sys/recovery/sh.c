#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <ctype.h>
#include <fcntl.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

static void builtin_clear(void) {
    // ANSI escape code: \033[H (move cursor to top-left) + \033[2J (clear entire screen)
    printf("\033[H\033[2J");
    fflush(stdout);
}

static void builtin_ls(const char *path) {
    const char *target = path ? path : ".";
    DIR *dir = opendir(target);
    if (!dir) {
        perror("ls");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip hidden dotfiles like . and ..
        if (entry->d_name[0] == '.') continue;
        printf("%s  ", entry->d_name);
    }
    printf("\n");
    closedir(dir);
}

static void builtin_ps(void) {
    DIR *dir = opendir("/proc");
    if (!dir) {
        perror("ps: failed to open /proc");
        return;
    }

    printf("  PID TTY          TIME CMD\n");

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Filter out non-numeric directories in /proc
        if (!isdigit((unsigned char)entry->d_name[0])) continue;

        char path[300];
        snprintf(path, sizeof(path), "/proc/%s/cmdline", entry->d_name);

        int fd = open(path, O_RDONLY);
        char cmd[256] = {0};

        if (fd >= 0) {
            ssize_t bytes = read(fd, cmd, sizeof(cmd) - 1);
            close(fd);

            // Replace null bytes in cmdline arguments with spaces for readability
            if (bytes > 0) {
                for (ssize_t i = 0; i < bytes - 1; i++) {
                    if (cmd[i] == '\0') cmd[i] = ' ';
                }
            }
        }

        // Fallback to /proc/[pid]/comm if cmdline is empty (e.g., kernel threads or uninitialized procs)
        if (cmd[0] == '\0') {
            snprintf(path, sizeof(path), "/proc/%s/comm", entry->d_name);
            fd = open(path, O_RDONLY);
            if (fd >= 0) {
                ssize_t bytes = read(fd, cmd, sizeof(cmd) - 1);
                close(fd);
                if (bytes > 0) {
                    // Strip trailing newline
                    cmd[strcspn(cmd, "\n")] = '\0';
                }
            }
        }

        if (cmd[0] == '\0') {
            snprintf(cmd, sizeof(cmd), "[unknown]");
        }

        printf("%5s ?        00:00:00 %s\n", entry->d_name, cmd);
    }

    closedir(dir);
}

int main(void) {
    char line[MAX_LINE];
    char *args[MAX_ARGS];

    while (1) {
        printf("$ ");
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin)) break;

        line[strcspn(line, "\n")] = '\0';

        int i = 0;
        args[i] = strtok(line, " \t");
        while (args[i] != NULL && i < MAX_ARGS - 1) {
            args[++i] = strtok(NULL, " \t");
        }
        if (args[0] == NULL) continue;

        // Built-in commands
        if (strcmp(args[0], "exit") == 0) {
            break;
        } 
        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL || chdir(args[1]) != 0) {
                perror("cd failed");
            }
            continue;
        } 
        if (strcmp(args[0], "clear") == 0) {
            builtin_clear();
            continue;
        } 
        if (strcmp(args[0], "ls") == 0) {
            builtin_ls(args[1]);
            continue;
        } 
        if (strcmp(args[0], "ps") == 0) {
            builtin_ps();
            continue;
        }

        // External commands fallback
        pid_t pid = fork();
        if (pid == 0) {
            execvp(args[0], args);
            perror(args[0]);
            exit(1);
        } else if (pid > 0) {
            waitpid(pid, NULL, 0);
        } else {
            perror("fork failed");
        }
    }
    return 0;
}
