#include <errno.h>  
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>
#include <stdio.h>
#define MAXLINE 4096  
int main()  
{      
    int  sockfd, n;      
    char sendline[4096];      
    char *sock_addr = "0.0.0.0";
    struct sockaddr_in    servaddr;      
    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)  
    {     
        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);      
        return 0;
    }      
    memset(&servaddr, 0, sizeof(servaddr));      
    servaddr.sin_family = AF_INET;      
    servaddr.sin_port = htons(1234);      
    if( inet_pton(AF_INET, sock_addr, &servaddr.sin_addr) <= 0)  
    {      
        printf("inet_pton error for %s\n",sock_addr);      
        return 0;
    }      
    if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)  
    {      
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno);      
        return 0;
    }      

    printf("register your name on server (shorter than 20 characters): ");     
    fgets(sendline, 4096 - 10, stdin);      
    for(n = 0;n < 21;++n)
        if(sendline[n] == '\n')
        {

            for(int k = n;k < 20;++k)
                sendline[k] = ' ';
            break;
        }
    if(n == 21)
    {
        fprintf(stderr,"error: name is too long!");
        close(sockfd);      
        return 0;
    }
    sendline[n] = ':';
    ++n;


    while(1)
    {
        printf("send msg: ");     
        fgets(sendline + n, 4096 - n, stdin);      
        if( send(sockfd, sendline, strlen(sendline) - 1, 0) < 0)      
        {      
            printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);      
            return 0;
        }     
    }
    close(sockfd);      
}  
