#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char const *argv[])
{
    int p1[2], p2[2];
    char buf[1];
    if (pipe(p1) != 0 || pipe(p2) != 0) {
        fprintf(2, "create pipe error\n");
        exit(1);
    }
    // parent   write p1[1] -------> p1[0] read  child
    // parent   read  p2[0] <------- p2[1] write child
    if (fork() == 0) { // child
        close(p1[1]);
        close(p2[0]);
        int childpid = getpid();
        read(p1[0], buf, 1);
        fprintf(1, "%d: received ping\n", childpid);
        write(p2[1], " ", 1);
        close(p1[0]);
        close(p2[1]);
        exit(0);
    }
    else {
        close(p1[0]);
        close(p2[1]);
        int parentpid = getpid();
        write(p1[1], " ", 1);
        read(p2[0], buf, 1);
        fprintf(1, "%d: received pong\n", parentpid);
        close(p1[1]);
        close(p2[0]);
        exit(0);
    }
}