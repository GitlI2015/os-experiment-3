// created by gloit
// 17.04.12
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int main()
{
    int shmid = shmget(1234, sizeof(int), IPC_CREAT | 0600);
    int* n     = shmat(shmid, 0, 0);
    scanf("%d", n);
    int mSize  = *n * *n;
    int shmid2 = shmget(1235, mSize * sizeof(int), IPC_CREAT | 0600);
    int shmid3 = shmget(1236, mSize * sizeof(int), IPC_CREAT | 0600);
    int shmid4 = shmget(1237, mSize * sizeof(int), IPC_CREAT | 0600);
    int shmid5  = shmget(1238, 3 * sizeof(int), IPC_CREAT | 0600);
    int* finishFlags = shmat(shmid5,0,0);

    int* m1 = shmat(shmid2, 0, 0);
    int* m2 = shmat(shmid3, 0, 0);
    int* m3 = shmat(shmid4, 0, 0);
    memset(m3, 0, mSize * sizeof(int));
    memset(finishFlags, 0, 3 * sizeof(int));

    for(int i = 0;i < *n;++i)
        for(int j = 0;j < *n;++j)
            scanf("%d",m1+i*(*n)+j);

    for(int i = 0;i < *n;++i)
        for(int j = 0;j < *n;++j)
            scanf("%d",m2+i*(*n)+j);

    pid_t fpid1, fpid2;

    fpid1 = fork();
    fpid2 = fork();

    n = shmat(shmid, 0, 0);
    m1 = shmat(shmid2, 0, 0);
    m2 = shmat(shmid3, 0, 0);
    m3 = shmat(shmid4, 0, 0);
    finishFlags = shmat(shmid5,0,0);

    if (fpid1 < 0 || fpid2 < 0)
        fprintf(stderr, "error: unable to fork!");
    else if (fpid1 == 0 || fpid2 == 0)
    {
        int t;
        if (fpid1)
            t = 0;
        else if (fpid2)
            t = 1;
        else
            t = 2;
        for(int i = t * (*n / 4); i < (t + 1) * (*n / 4);++i)
            for(int k = 0;k < *n;++k)
                for(int j = 0;j < *n;++j)
                    m3[i * (*n) + j] += m1[i * (*n) + k] * m2[k * (*n) + j];

        finishFlags[t] = 1;
        return 0;
    }
    else
    {
        for(int i = 3 * (*n / 4);i < *n;++i)
            for(int k = 0;k < *n;++k)
                for(int j = 0;j < *n;++j)
                    m3[i * (*n) + j] += m1[i * (*n) + k] * m2[k * (*n) + j];
    }
    while(!(finishFlags[0] && finishFlags[1] && finishFlags[2]))
    {
    };
    for(int i = 0;i < *n;++i)
    {
        for(int j = 0;j < *n;++j)
            printf("%d ",m3[i * (*n) + j]);
        puts("");
    }

    shmdt(n);
    shmdt(m1);
    shmdt(m2);
    shmdt(m3);
    shmdt(finishFlags);
    shmctl(shmid, IPC_RMID, NULL);
    shmctl(shmid2, IPC_RMID, NULL);
    shmctl(shmid3, IPC_RMID, NULL);
    shmctl(shmid4, IPC_RMID, NULL);
    shmctl(shmid5, IPC_RMID, NULL);

    return 0;
}
