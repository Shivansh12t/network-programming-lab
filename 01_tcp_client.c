#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFFER_LEN 1024

int main(){
    int sockfd, n;
    char* ip = "127.0.0.1";
    int port = 5566;

    struct sockaddr_in server_addr;
    char buffer[BUFFER_LEN];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0){
        perror("[-] Socket Creation Failed");
        exit(1);
    }
    printf("[+] Socket Creation successful\n");

    memset(&server_addr, '\0', sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("[-] Connection Error");
        close(sockfd);
        exit(1);
    }

    printf("[+] TCP Connection established at port %d\n", port);

    bzero(buffer, BUFFER_LEN);
    printf("[C] ");
    fgets(buffer, BUFFER_LEN, stdin);

    if (send(sockfd, buffer, strlen(buffer), 0) < 0){
        perror("[-] Send Error");
    }

    bzero(buffer, BUFFER_LEN);
    n = recv(sockfd, buffer, BUFFER_LEN, 0);

    if (n < 0){
        perror("[-] Recieve Error");
    }
    printf("[S] %s", buffer);

    printf("Closing Connection...\n");
    close(sockfd);
    
    return 0;
}