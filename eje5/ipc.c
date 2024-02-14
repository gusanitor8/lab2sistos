#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>

#define SHMSZ 27

int main(int argc, char *argv[]){
    int shmid;
    key_t key;
    char *shm, *s;
    int n = atoi(argv[1]);
    char x = argv[2][0];
    
    // Verify command line arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <n> <x>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Generate a key for the shared memory segment
    if ((key = ftok("ipc.c", 'A')) == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Create the shared memory segment
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach to the shared memory segment
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Detach from the shared memory segment
    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    // Remove the shared memory segment
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }

    // Set up communication with the other instance
    if (*shm != '\0') {
        printf("Shared memory segment already exists. Receiving file descriptor from other instance...\n");
        shmid = atoi(shm);
        if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
            perror("shmat");
            exit(EXIT_FAILURE);
        }
    } else {
        sprintf(shm, "%d", shmid);
    }    

    // Map the shared memory segment to a pointer
    s = shm;

    // Fork a child process
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        while (*s != '\0') {
            sleep(1);
        }
        printf("Child process: Received shared memory segment.\n");
        // Wait for notifications from parent and write 'x' to shared memory
        while (1) {
            if (*s != '\0') {
                *s = x;
                s++;
            }
        }
    } else {
        // Parent process
        int i;
        for (i = 1; i <= SHMSZ; i++) {
            if (i % n == 0) {
                *s = x;
                s++;
            }
        }
        wait(NULL); // Wait for child process to finish
        printf("Parent process: Shared memory contents: ");
        for (s = shm; *s != '\0'; s++) {
            putchar(*s);
        }
        putchar('\n');
        // Detach from the shared memory segment
        if (shmdt(shm) == -1) {
            perror("shmdt");
            exit(EXIT_FAILURE);
        }
        // Remove the shared memory segment
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("shmctl");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}