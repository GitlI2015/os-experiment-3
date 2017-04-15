#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAXLINE 4096
int conNum;
int listenfd;
int confds[10];
void connection_thread()
{
    for (conNum = 0; conNum < 10; ++conNum) {
        if ((confds[conNum] = accept(listenfd, (struct sockaddr*)NULL, NULL)) ==
            -1)
        {
            fprintf(stderr, "accept error");
            return;
        }
    }
}
int main()
{
    struct sockaddr_in servaddr;
    char buff[4096];
    int n, ret;
    void* nothing;
    conNum = 0;
    pthread_t p_id;
    fd_set set;
    struct timeval timeout = {1, 0};
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        exit(0);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(1234);
    if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
        printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
        return 0;
    }
    if (listen(listenfd, 10) == -1) {
        printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
        exit(0);
    }
    printf("Server start\n");
    ret = pthread_create(&p_id, NULL, (void*)connection_thread, NULL);
    if (ret != 0) {
        fprintf(stderr, "pthread create thread error!\n");
        close(listenfd);
        return 0;
    }
    while (1) {
        FD_ZERO(&set);
        int j = 0;
        for (int i = 0; i < conNum; ++i) 
        {
            FD_SET(confds[i], &set);
            j = j < confds[i] + 1 ? confds[i] + 1 : j;
        }

        switch (select(j, &set, NULL, NULL, &timeout))
        {
            case -1: exit(EXIT_FAILURE);
            case 0: break;
            default:
                for (int i = 0; i < conNum; ++i) {
                    if (FD_ISSET(confds[i], &set)) 
                        if((n = recv(confds[i], buff, MAXLINE, 0)) != 0)
                        {
                            buff[n] = '\0';
                            printf("%s\n", buff);
                        }
                }
        }
    }
    pthread_join(p_id, nothing);
    for (int i = 0; i < conNum; ++i) close(confds[i]);
    close(listenfd);
}
