#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(){
    int serverfd = socket(AF_INET, SOCK_STREAM, 0);
    int port = 8080;

    if (serverfd == -1){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverfd, 10) < 0){
        perror("Listen Failed");
        exit(EXIT_FAILURE);
    }

    printf("server is listening on port %d....\n", port);

    while(1){
        int clientfd = accept(serverfd, NULL, NULL);

        if (clientfd < 0){
            perror("Accept failed");
            continue;
        }

        char buffer[1024] = {0};
        read(clientfd, buffer, sizeof(buffer));
        printf("Request: %s\n", buffer);

        const char* response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 13\r\n"
            "\r\n"
            "Hello, World!";
        write(clientfd, response, strlen(response));

        close(clientfd);
    }

    close(serverfd);
    return 0;
}