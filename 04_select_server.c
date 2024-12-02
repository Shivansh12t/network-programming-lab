#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/time.h>

#define PORT 5000
#define MAXLINE 1024

int max(int a, int b){
    if (a > b) return a;
    else return b;
}

int main(){
    int listenfd, connfd, udpfd, nready, maxfd;
    char buffer[MAXLINE];
    fd_set rset;
    ssize_t n;
    socklen_t len;
    const int on = 1;
    struct sockaddr_in cliaddr, seraddr;
    char* message = "Hello client";

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&seraddr, sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(PORT);
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(listenfd, (struct sockaddr *)&seraddr, sizeof(seraddr));
    listen(listenfd, 10);

    udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    bind(udpfd, (struct sockaddr *)&seraddr, sizeof(seraddr));

    FD_ZERO(&rset);

    maxfd = max(listenfd, udpfd) + 1;

    for (;;){
        FD_SET(listenfd, &rset);
        FD_SET(udpfd, &rset);

        nready = select(maxfd, &rset, NULL, NULL, NULL);

        if (FD_ISSET(listenfd, &rset)){
            len = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len);
            bzero(buffer, sizeof(buffer));
            printf("Message from TCP client :");
            read(connfd, buffer, sizeof(buffer));
            fgets(buffer, MAXLINE, stdin);
            write(connfd, buffer, sizeof(buffer));
            close(connfd);
        }

        if (FD_ISSET(udpfd, &rset)) {
            len = sizeof(cliaddr);
            bzero(buffer, sizeof(buffer));
            printf("\nMessage from UDP client: ");
            n = recvfrom(udpfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cliaddr, &len);
            puts(buffer);
            fgets(buffer,100,stdin);
            sendto(udpfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
        }
    }
    return 0;
}
