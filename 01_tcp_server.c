#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFFER_LEN 1024

int main(){
    char* ip = "127.0.0.1";
    int port = 5566;

    int serverfd, clientfd, n;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(server_addr);

    char buffer[BUFFER_LEN];

    serverfd = socket(AF_INET, SOCK_STREAM, 0);

    if (serverfd < 0){
        perror("[-] Socket Creation Failed");
        exit(1);
    }

    printf("[+] Socket Created Succesfully\n");

    memset(&server_addr, '\0', addr_size);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);

    if (bind(serverfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0){
        perror("[-] Bind Failed");
        close(serverfd);
        exit(1);
    }
    printf("[+] Bind Successfully at port %d\n", port);

    if (listen(serverfd, 5) < 0){
        perror("[-] Listening Error");
        close(serverfd);
        exit(1);
    }
    printf("Listening......\n");

    clientfd = accept(serverfd, (struct sockaddr*) &client_addr, &addr_size);

    if (clientfd < 0){
        perror("[-] Connection Error");
        close(serverfd);
        exit(1);
    }
    printf("[+] Connection with Client Successful\n");

    bzero(buffer, BUFFER_LEN);
    n = recv(clientfd, buffer, BUFFER_LEN, 0);

    if (n < 0){
        perror("[-] Recieve Error");
    }
    printf("[C] %s", buffer);

    bzero(buffer, BUFFER_LEN);
    printf("[S] ");
    fgets(buffer, BUFFER_LEN, stdin);

    if (send(clientfd, buffer, strlen(buffer), 0) < 0){
        perror("[-] Send Error");
    }

    printf("Connection closing .....\n");
    close(clientfd);
    close(serverfd);

    return 0;
}