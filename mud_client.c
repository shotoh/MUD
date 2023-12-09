#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_LEN 1024
#define PORT 8888

int main(int argc, char const* argv[]) { //
    int socketfd;
    struct sockaddr_in address;
    char buffer[MAX_LEN];

    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = inet_addr("35.212.252.59");

    if (connect(socketfd, (struct sockaddr*) &address,sizeof(address)) < 0) {
        printf("Connection Failed");
        exit(EXIT_FAILURE);
    }



    write(socketfd, "Hello", strlen("Hello"));
    printf("Hello message sent\n");
    read(socketfd, buffer, MAX_LEN - 1);
    printf("%s\n", buffer);



    close(socketfd);
    return 0;
}