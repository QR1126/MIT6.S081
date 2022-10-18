#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAXNUMBER 35
#define WRITE 1
#define READ 0

void child(int *pleft) {
    int n;
    int pright[2];

    close(pleft[WRITE]);

    int res = read(pleft[READ], &n, sizeof(int));
    if (res == 0) {
        exit(0);
    }

    pipe(pright);

    if (fork() == 0) {
        child(pright);
    } else {
        close(pright[READ]);
        printf("prime %d\n", n);
        int prime = n;
        while (read(pleft[READ], &n, sizeof(int)) != 0) {
            if (n % prime != 0) {
                write(pright[WRITE], &n, sizeof(int));
            }
        }
        close(pright[WRITE]);
        wait((int *) 0);
        exit(0);
    }
}

int 
main(int argc, char const *argv[])
{
    int p[2];
    pipe(p);
    if (fork() == 0) {
        child(p);
    } else {
        close(p[READ]);
        for (int i = 2; i <= MAXNUMBER; i++) {
            write(p[WRITE], &i, sizeof(int));
        }
        close(p[WRITE]);
        wait((int *) 0);
    }
    exit(0);
}