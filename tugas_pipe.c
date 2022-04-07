#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <string.h>
#include <time.h>
#define MSGSIZE 2

int main(int argc, char **argv) {
        srand(time(NULL));
        char inbuf[MSGSIZE]; //buffer
        int fd[2]; //pipe

        //create pipe
        if (pipe(fd) < 0) {
            exit(1); //error
        }
        printf("mulai \n");
        int n, m; 
        printf("Masukan banyaknya proses 1 yang diinginkan : ");
        scanf("%d", &n);
        printf("Masukan banyaknya proses 2 yang diinginkan : ");
        scanf("%d", &m);
        int pid;
            switch (pid = fork()) {
            case 0:         /* fork returns 0 ke Kelompok Proses 1 */
                   for(int i = 0; i < n; i++){
                       printf("Kelompok Proses 1\n");
                    // tulis data ke pipe
                    int arrIntChild[MSGSIZE] = {rand() % 100,rand() % 100};
                    for(int j = 0 ; j < 2 ;j++){
                        printf("nilai ke %d = %d\n", j+1, arrIntChild[j]);
                    }
                    /* tutup bagian input dari pipe */
                    close(fd[0]);
                    // tulis ke pipe
                    write(fd[1], arrIntChild, sizeof(arrIntChild));
                   } 
                    break;
            default:        /* fork returns pid Kelompok Proses 2 */
                    for(int i = 0; i < m; i++){
                        printf("Kelompok Proses 2\n");
                    int arrIntParent[MSGSIZE];
                    /* tutup bagian output dari pipe */
                    close(fd[1]);
                    // baca yang ditulis child dari pipe
                    read(fd[0], arrIntParent, sizeof(arrIntParent));
                    int total = 0;
                    if(i < n){
                        for (int j = 0; j<MSGSIZE; j++) {
                            total += arrIntParent[j];
                        }
                    }
                    printf("Total = %d\n", total);
                    }
                    
                    break;
    
            case -1:        /* error */
                    perror("fork");
                    exit(1);
            }

        exit(0);
}
