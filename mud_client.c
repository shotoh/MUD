#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_LEN 1024
#define PORT 8888

int main(int argc, char const* argv[]) { //
    int sockfd;
    struct sockaddr_in address;
    char buffer[MAX_LEN];

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&address, 0, sizeof(address));
    memset(buffer, 0, sizeof(buffer));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = inet_addr("35.212.252.59");

    if (connect(sockfd, (struct sockaddr*) &address,sizeof(address)) < 0) {
        printf("Connection Failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        fgets(buffer, MAX_LEN, stdin);
        write(sockfd, buffer, strlen(buffer));
        if (!strncmp(buffer, "stop", strlen("stop"))) break;
    }

    close(sockfd);
    return 0;
}