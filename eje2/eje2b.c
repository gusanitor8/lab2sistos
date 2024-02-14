#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    clock_t start, end;
    pid_t padre, hijo, nieto, bisnieto;

    padre = getpid();

    start = clock();
    hijo = fork();

    if (hijo == 0){
        hijo = getpid();
        nieto = fork(); // Haga que el proceso hijo realice otro fork()

        if (nieto == 0){
            nieto = getpid(); 
            bisnieto = fork(); // (el proceso nieto) haga también un nuevo fork()

            if (bisnieto == 0){ 
                // El proceso bisnieto (el creado por el fork() más anidado) debe realizar un ciclo for de un millón de iteraciones que no hagan nada
                bisnieto = getpid();
                for (int i = 0; i < 1000000; i++){}

                if (bisnieto == 0);
                    
            }else{
                // El proceso nieto debe realizar lo mismo que el bisnieto en el inciso anterior, pero de forma exclusiva
                for (int i = 0; i < 1000000; i++){}
                wait(NULL);
            }
        }else{
            // El proceso hijo debe realizar lo mismo, y en las mismas condiciones, que el proceso nieto en el inciso anterior.
            for (int i = 0; i < 1000000; i++){}
            wait(NULL);
        }
    }else{   // En el proceso raíz o padre exclusivamente (es decir, en el else menos anidado) espere a que termine la ejecución del proceso hijo
        wait(NULL);
        end = clock(); // luego ejecute clock()
        printf("Elapsed time: %f\n", (double)(end - start)/CLOCKS_PER_SEC);
    }    
}
