#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int readline(char *buf) {
    char c;
    while (1) {
        int num = read(0, &c, 1);
        if (num < 1) return 0;
        if (c == '\n' || c == '\r')
            break;
        *buf++ = c;
    }
    *buf = '\0';
    return 1;
}

int main(int argc, char *argv[]) {
    char buf[512], *argv2[MAXARG];
    memmove(argv2, argv + 1, (argc - 1) * 8);
    while (readline(buf)) {
        if (fork() == 0) {
            argv2[argc-1] = buf;
            exec(argv2[0], argv2);
            exit(0);
        }
        else wait(0);
    }
    exit(0);
}