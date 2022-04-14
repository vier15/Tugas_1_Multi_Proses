#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <string.h>
#include <time.h>
#define MSGSIZE 5


int main(int argc, char **argv) {
        char inbuf[MSGSIZE]; //buffer
        int fd[2]; //pipe
  			int fdCount[2];

        //create pipe
        if (pipe(fd) < 0) {
            exit(1); //error
        }
  			if (pipe(fdCount) < 0) {
            exit(1); //error
        }
        int arrIntProducer[MSGSIZE];
        int pidProducer, pidConsumer;
        int pid;
  			int x;
  			int total = 0;
  			int grandTotal;
  			
  			switch (pid = fork()) {
        case 0:         /* fork returns 0 ke proses anak */
                printf("Proses anak \n");
								int count = 0;
                for(int i = 0; i < 3 ; i++){
                  		fork();
                  		srand(getpid());
                      x = rand() % 10;
                      arrIntProducer[count] = x;
                  		printf("Producer (%d) membuat nilai %d di buffer %d\n", getpid(), arrIntProducer[count], count);
                      count++;
                }
                /* tutup bagian input dari pipe */
                close(fd[0]);
            		close(fdCount[0]);
                // tulis ke pipe
                write(fd[1], arrIntProducer, sizeof(arrIntProducer));
            		write(fdCount[1], &count, sizeof(int));
                break;
        default:        /* fork returns pid ke proses ortu */
                printf("Proses ortu\n");
                /* tutup bagian output dari pipe */
                close(fd[1]);
            		close(fdCount[1]);
                // baca yang ditulis child dari pipe
                read(fd[0], arrIntProducer,  sizeof(arrIntProducer)); //buffer terisi
            		read(fdCount[0], &count,  sizeof(int));
                
							int grandTotal = 0;
            	printf("nilai = %d\n", count);
               	for(int i = 0; i < 2 ; i++){
                  		fork();
                      total =  arrIntProducer[count-1];
                  		printf("Consumer (%d) mengambil nilai %d di buffer %d\n", getpid(),total, count-1);
                  		grandTotal += total;
                  		count--;
                }
								
                break;
        case -1:        /* error */
                perror("fork");
                exit(1);
        }
				printf("total = %d\n", grandTotal);
        exit(0);
}