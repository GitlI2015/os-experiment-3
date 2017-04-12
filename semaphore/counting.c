// created by gloit
// 17.04.12
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/types.h>

sem_t g_semt;
FILE* fp;
int buf[192];
int *buf_p;
int j;

void* reader_thread(void* args)
{
    long long a = 0;

    for(j = 0;j < 10;++j)
    {
        sem_wait(&g_semt);
        for(int  i = 0;i < 192;++i)
            a += buf[i];
        printf("Current sum is: %lld\n", a);
        sem_post(&g_semt);
        a = 0;
        usleep(100000);
    }
    return 0;
}

void* writer_thread(void* args)
{
    int s = *(int *)args;
    while(j != 10)
    {
        sem_wait(&g_semt);
        for(int i = 64*(s-1);i < 64*s-1;++i)
            buf[i] = rand() % 1000;
        sem_post(&g_semt);
    }
}

int main()
{
    const size_t nThreadCount = 4; //amounts of thread array
    const unsigned int nSemaphoreCount = 1; //initial value of semaphore
    int nRet = -1;
    void* pRet = NULL;
    memset(buf,0,sizeof(buf));
    pthread_t threadIDs[nThreadCount];
    fp = fopen("test","w+");
    
    nRet = sem_init(&g_semt, 0, nSemaphoreCount);
    if (0 != nRet)
        return -1;

    nRet = pthread_create(&threadIDs[0], NULL, reader_thread, NULL); 
    for (size_t i = 1; i < nThreadCount; ++ i)
    {
        nRet = pthread_create(&threadIDs[i], NULL, writer_thread, &i);
        if (0 != nRet)
            continue;
    }

    for (size_t i = 0; i < nThreadCount; ++ i)
        pthread_join(threadIDs[i], &pRet);

    sem_destroy(&g_semt);

    return 0;
}

