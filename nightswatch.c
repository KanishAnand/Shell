#include <dirent.h>
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

void enable_raw_mode() {
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~(ICANON | ECHO);  // Disable echo as well
    tcsetattr(0, TCSANOW, &term);
}

void disable_raw_mode() {
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(0, TCSANOW, &term);
}

int _kbhit() {
    int byteswaiting;
    enable_raw_mode();
    ioctl(0, FIONREAD, &byteswaiting);
    disable_raw_mode();
    return byteswaiting > 0;
}

void nightswatch(char **args, int no_of_args) {
    if (no_of_args != 3)
        return;

    if (strcmp(args[3], "dirty") == 0) {
        char st[1000] = "/proc/meminfo";
        char *s = args[2];
        int l = strlen(s);
        l--;
        int p = 1;
        int a, time = 0;

        for (int i = l; i >= 0; i--) {
            a = s[i] - '0';
            time += a;
            p *= 10;
        }

        int fd1 = open(st, O_RDONLY);
        char *buff = (char *)calloc(7000, sizeof(char));
        read(fd1, buff, 7000);

        char **parts = (char **)malloc(100 * sizeof(char *));
        int i = 0;
        while (i < 50) {
            parts[i] = strsep(&buff, "\n");
            ++i;
        }
        printf("%s\n", parts[16]);
        close(fd1);

        int c = 0;
        int interval = time;
        int before = clock();
        int msec = 0, iterations = 0;
        interval *= 1000;

        // printf("%d\n", interval);
        while (1) {
            clock_t diff = clock() - before;
            msec = diff * 1000 / CLOCKS_PER_SEC;

            iterations++;

            if (_kbhit()) {
                char c = getchar();
                if (c == 'q') {
                    break;
                }
            }

            if (msec >= interval) {
                before = clock();
                int fd = open(st, O_RDONLY);

                if (fd == -1) {
                    perror("nightswatch");
                } else {
                    char *buff = (char *)calloc(7000, sizeof(char));
                    read(fd, buff, 7000);

                    char **parts = (char **)malloc(100 * sizeof(char *));
                    int i = 0;
                    while (i < 50) {
                        parts[i] = strsep(&buff, "\n");
                        ++i;
                    }
                    printf("%s\n", parts[16]);
                    close(fd);
                }
            }
        }
    } else if (strcmp(args[3], "interrupt") == 0) {
        char st[1000] = "/proc/interrupts";
        char *s = args[2];
        int l = strlen(s);
        // clock_t initial_time = clock();
        l--;
        int p = 1;
        int a, time = 0;

        for (int i = l; i >= 0; i--) {
            a = s[i] - '0';
            time += a;
            p *= 10;
        }
        int fd1 = open(st, O_RDONLY);
        char *buff = (char *)calloc(7000, sizeof(char));
        read(fd1, buff, 7000);

        char **parts = (char **)malloc(100 * sizeof(char *));
        int i = 0;
        while (i < 50) {
            parts[i] = strsep(&buff, "\n");
            ++i;
        }
        printf("%s\n", parts[0]);
        printf("%s\n", parts[2]);
        close(fd1);
        int c = 0;

        int interval = time;
        int before = clock();
        int msec = 0, iterations = 0;
        interval *= 1000;

        while (1) {
            // clock_t final_time = clock();
            clock_t diff = clock() - before;
            msec = diff * 1000 / CLOCKS_PER_SEC;
            // printf("%d\n", msec);
            iterations++;

            if (_kbhit()) {
                char c = getchar();
                if (c == 'q') {
                    break;
                }
            }
            if (msec >= interval) {
                // printf("df\n");
                before = clock();
                int fd = open(st, O_RDONLY);

                if (fd == -1) {
                    perror("nightswatch");
                } else {
                    char *buff = (char *)calloc(7000, sizeof(char));
                    read(fd, buff, 7000);

                    char **parts = (char **)malloc(100 * sizeof(char *));

                    int i = 0;
                    while (i < 50) {
                        parts[i] = strsep(&buff, "\n");
                        ++i;
                    }
                    printf("%s\n", parts[0]);
                    printf("%s\n", parts[2]);
                    close(fd);
                }
            }
        }
    } else {
        printf("Please give correct command\n");
    }
}