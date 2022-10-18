#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *file_name) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    // int fstat(int fd, struct stat *st) system call
    // which save inode (file) information into st
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0) continue;
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        
        if (stat(buf, &st) < 0) {
            fprintf(2, "find: cannot stat %s\n", buf);
            continue;
        }

        switch (st.type) {
        case T_FILE:
            if (strcmp(file_name, de.name) == 0) {
                printf("%s\n", buf);
            }
            break;
        case T_DIR:
            if (strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) {
                find(buf, file_name);
            }
        default:
            break;
        }
    }
    close(fd);
}

int 
main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(2, "find: you need pass 2 arguments\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}