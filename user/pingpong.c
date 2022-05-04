#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
   
int main(int argc, char *argv[]){
    int pid;
    int p1[2], p2[2];
    char buf[1];
    pipe(p1);// p1: parent write, child read
    pipe(p2);// p2: parent read, child write
    pid = fork();
    if (pid < 0) {
        exit(1);
    }
    else if (pid == 0) {//child
        close(p1[1]);
        close(p2[0]);
        read(p1[0], buf, 1);
        printf("%d: received ping\n", getpid());
        write(p2[1], "x", 1);
        close(p2[1]);
        close(p1[0]);
        exit(0);
    } else {//parent
        close(p1[0]);
        close(p2[1]);
        write(p1[1], "x", 1);
        read(p2[0], buf, 1);
        printf("%d: received pong\n", getpid());
        close(p1[1]);
        close(p2[0]);
        exit(0);
    }
}