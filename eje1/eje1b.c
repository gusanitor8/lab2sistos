#include <stdio.h>
#include <unistd.h>

int main(){
    for (int i = 0; i < 4; i++){
        int pid = fork();        
    }
    printf("Soy un proceso : %d\n", getpid());
}
