#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int pipfd[2];
    pipe(pipfd);

    if (fork() == 0) {
        char s;
        if (read(pipfd[0], &s, 1) != 1) {
            fprintf(2, "fail to read in child.\n");
            exit(1);
        }
        close(pipfd[0]);
        printf("%d: received ping\n", getpid());
        if (write(pipfd[1], "!", 1) != 1) {
            fprintf(2, "fail to write in child.\n");
            exit(1);
        }
        close(pipfd[1]);
    } else {
        char s;
        if (write(pipfd[1], "!", 1) != 1) {
            fprintf(2, "fail to write in parent.\n");
            exit(1);
        }
        close(pipfd[1]);
        wait(0);
        if (read(pipfd[0], &s, 1) != 1) {
            fprintf(2, "fail to read in parent.\n");
        }
        printf("%d: received pong\n", getpid());
    }
    exit(0);
}