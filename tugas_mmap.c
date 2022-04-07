#include <sys/mman.h>   /* mmap()  */
#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <sys/wait.h>   /*wait*/
#include <errno.h>	    /* errno */
#include <time.h>

#define MSGSIZE 4

int main(int argc, char **argv) {
        srand(time(NULL));
    
        int n, m; 
        printf("Masukan banyaknya proses 1 yang diinginkan : ");
        scanf("%d", &n);
        printf("Masukan banyaknya proses 2 yang diinginkan : ");
        scanf("%d", &m);
        
        int max = 0;
        if(n < m){
            max = m;
        }
        else{
            max = n;
        }
        
        int pid[max];

        printf("mulai \n");

        int size = MSGSIZE * sizeof(int);
        //buat memory map
        
       // for (int i = 0; i<max; i++) {
            void *addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
            printf("Mapped at %p\n", addr);
            int *shared = addr;
       // }
        
        int x = 0, y = 0;
        switch (pid[y] = fork()) {
        case 0:         /*  proses anak */
                for (int i = 0; i<n; i++) {
                    pid[y+1] = fork();
                }
                printf("Proses anak \n");
                //isi data di memory map
                shared[0+y] = rand() % 100;
                shared[1+y] = rand() % 100;
                printf("sh0 = %d\n", shared[0+y]);
                break;
        default:        /*  ortu */
                printf("Proses ortu\n");
                int pidWait,status;
                /* wait sampai child selesai */
                while (pidWait = wait(&status)) { 
                    for (int j = 0; j<2; j++) {
                    if (pidWait == pid[x]){  /* child sukses selesai*/
                        //cetak isi memory map
                        int jumlah;
                        for (int i = 0; i<MSGSIZE; i++) {
                          jumlah = jumlah + shared[i];
                        }
                        printf("Total = %d\n", jumlah);
                        x++;
                        break; //keluar dari loop wait
                    if ((pidWait == -1) && (errno != EINTR)) {
                        /* ada error*/
                        perror("waitpid");
                        exit(1);
                    }
                }
                    }
                break;
        case -1:        /* error */
                perror("fork");
                exit(1);
        }
        exit(0);
    }
}