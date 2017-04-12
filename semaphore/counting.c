// created by gloit
// 17.04.12
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#define BUFFER_SIZE 192

int main()
{
    int shmid      = shmget(1234, 4, IPC_CREAT | 0600);
    int shmid2     = shmget(1235, sizeof(sem_t), IPC_CREAT | 0600);
    int shmid3     = shmget(1236, sizeof(sem_t), IPC_CREAT | 0600);
    int shmid4     = shmget(1237, BUFFER_SIZE * sizeof(int), IPC_CREAT | 0600);
    int* j         = shmat(shmid, 0, 0);
    sem_t* bufferSem  = shmat(shmid2, 0, 0);
    sem_t* counterSem = shmat(shmid3, 0, 0);
    int* buf       = shmat(shmid4, 0, 0);
    *j             = 0;
    sem_init(bufferSem, 1, 1);
    sem_init(counterSem, 1, 1);
    memset(buf, 0, BUFFER_SIZE * sizeof(int));

    pid_t fpid1, fpid2;

    fpid1 = fork();
    fpid2 = fork();

    j       = shmat(shmid, 0, 0);
    bufferSem  = shmat(shmid2, 0, 0);
    counterSem = shmat(shmid3, 0, 0);
    buf     = shmat(shmid4, 0, 0);

    if (fpid1 < 0 || fpid2 < 0)
        fprintf(stderr, "error: unable to fork!");
    else if (fpid1 == 0 || fpid2 == 0)
    {
        int n;
        if (fpid1)
            n = 0;
        else if (fpid2)
            n = 1;
        else
            n = 2;
        while (1) {
            sem_wait(counterSem);
            if (*j == 10) {
                sem_post(counterSem);
                shmdt(j);
                shmdt(bufferSem);
                shmdt(counterSem);
                shmdt(buf);
                return 0;
            }
            else
            {
                sem_post(counterSem);
                sem_wait(bufferSem);
                for (int i = n * 64; i < n * 64 + 63; ++i)
                    buf[i] = rand() % 1000;
                sem_post(bufferSem);
            }
        }
    }
    else
    {
        while (*j < 10) {
            int num = 0;
            sem_wait(counterSem);
            sem_wait(bufferSem);
            for (int i = 0; i < BUFFER_SIZE; i++) {
                num += buf[i];
            }
            sem_post(bufferSem);
            (*j)++;
            sem_post(counterSem);
            usleep(1000000);
            printf("Buffer sum of round %d is : %d\n", *j, num);
        }
    }

    sem_destroy(bufferSem);
    sem_destroy(counterSem);
    shmdt(j);
    shmdt(bufferSem);
    shmdt(counterSem);
    shmdt(buf);
    shmctl(shmid, IPC_RMID, NULL);
    shmctl(shmid2, IPC_RMID, NULL);
    shmctl(shmid3, IPC_RMID, NULL);
    shmctl(shmid4, IPC_RMID, NULL);

    return 0;
}
