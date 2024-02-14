#include <stdio.h>
#include <unistd.h>

int main(){
    int pid = fork();
    int pid2 = fork();
    int pid3 = fork();
    int pid4 = fork();

    printf("Soy un proceso\n");

    if (pid != 0){
        sleep(1);
    }

    return 0;
}