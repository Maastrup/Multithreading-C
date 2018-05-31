#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

// For pipe's ends
enum State{READ, WRITE};

int main() {
    // Pipe needs an array with two pointers
    // First element will be read-end and the other will be write-end
    int fd[2];
    pipe(fd);

    // Need to disable buffering for this program to print stuff
    setbuf(stdout, NULL);

    // Splits program into two separate processes, a child and the father
    pid_t pid = fork();
    if(pid == 0){
        printf("Hello from child\n");
        // Child process will be producer
        close(fd[READ]);

        while(true){
            // Sets the seed rand() uses for random generation
            srand((unsigned) time(NULL));
            // rand() generates a number between 0 and RAND_MAX
            // to constrain it to given limits, modulo is needed
            char randChar = (char)(rand() % 26 + 'a');
            write(fd[WRITE], &randChar, sizeof(char));
            printf("'%c' produced\n", randChar);

            // Time is in seconds
            sleep(1);
        }
    } else if(pid == -1) {
        // If fork fails
        printf("Fork failed");
        return 404;
    } else {
        printf("Hello from parent\n");
        // Parent process will be consumer
        close(fd[WRITE]);

        while(true){
            char consumedChar;
            read(fd[READ], &consumedChar, sizeof(char));
            printf("'%c' consumed\n", consumedChar);

            // Time is in seconds
            sleep(1);
        }
    }

    return 0;
}