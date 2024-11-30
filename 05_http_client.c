#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(){
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    char* ip = "127.0.0.1";
    int port = 8080;

    if (clientfd == -1){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(clientfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }

    const char* request =
        "GET / HTTP/1.1\r\n"
        "Host : 127.0.0.1\r\n"
        "Connection: close\r\n\r\n";
    write(clientfd, request, strlen(request));

    char buffer[4096];
    int bytes_recieved = read(clientfd, buffer, sizeof(buffer) - 1);

    if (bytes_recieved > 0){
        buffer[bytes_recieved] = '\0';
        printf("Response: %s\n", buffer);
    }

    close(clientfd);
    return 0;
}