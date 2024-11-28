#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int pleft[2]) {
    int mod;
    if (read(pleft[0], &mod, 4) == 0) {
        exit(0);
    }
    
    printf("prime %d\n", mod);
    int pright[2];
    pipe(pright);

    if (fork() != 0) {
        close(pright[0]);
        int num;
        int flag;
        flag = read(pleft[0], &num, 4);
        while(flag) {
            if (num % mod != 0)
                write(pright[1], &num, 4);
            flag = read(pleft[0], &num, 4);
        }
        close(pleft[0]);
        close(pright[1]);
        wait(0);
        exit(0);
    } else {
        close(pright[1]);
        close(pleft[0]);
        sieve(pright);
    }
}

int main(int argc, char *argv[]) {
    int pright[2];
    pipe(pright);

    if (fork() != 0) {
        close(pright[0]);
        for (int i = 2; i <= 35; i++) {
            write(pright[1], &i, 4);
        }
        close(pright[1]);
    } else {
        close(pright[1]);
        sieve(pright);
        exit(0);
    }
    wait(0);
    exit(0);
}