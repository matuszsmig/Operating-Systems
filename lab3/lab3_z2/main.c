#include <stdio.h>
#include <unistd.h>

int main(int agrc, char *argv[]) {
    printf("ścieżka moja to: %s", argv[0]);
    printf("\n%s\n", argv[1]);
    fflush(stdout);
    execl("/bin/ls", "ls", argv[1], NULL);


    return 0;
}
