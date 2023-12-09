#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <dirent.h>
#include <asm-generic/errno.h>

#include "MQTTClient.h"

#define ADDRESS     "localhost"
#define CLIENTID    "Server"
#define TOPIC       "home/ubuntu/final"

#define MAX_LEN 1024
#define PORT 8888

void checkDir(char c);

MQTTClient client;
char buffer[MAX_LEN];
char curr[256] = "./start/";
char prev[256];
char back;

extern int errno;

int main() {
    printf("Attempting to initialize MQTT client...\n");
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc; // response code
    if ((rc = MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS) {
        printf("MQTT client initialization unsuccessful, rc=%d\n", rc);
        exit(EXIT_FAILURE);
    }
    conn_opts.keepAliveInterval = 20; // keep alive to 20 seconds
    conn_opts.cleansession = 1; // make it clean
    conn_opts.connectTimeout = 5; // timeout in 5 seconds
    printf("MQTT client initialization successful\n");
    printf("Attempting to connect to broker...\n");
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Broker connection unsuccessful, timeout=5s, rc=%d\n", rc);
        exit(EXIT_FAILURE);
    }
    printf("Broker connection successful\n");

    MQTTClient_message msg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    int servfd, sockfd;
    struct sockaddr_in servaddr, cliaddr;
    // Creating socket
    servfd = socket(AF_INET, SOCK_STREAM, 0);
    if (servfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    // Binding socket
    if (bind(servfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) != 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }
    // Listening for connections
    if (listen(servfd, 5) != 0) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }
    printf("Waiting for socket client to connect...\n");
    socklen_t len = sizeof(cliaddr);
    sockfd = accept(servfd, (struct sockaddr *)&cliaddr, &len);
    if (sockfd < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket client connected.\n");


    while (1) {
        read(sockfd, buffer, sizeof(buffer));
        if (!strncmp(buffer, "exit", strlen("exit"))) {
            printf("Exiting MUD");
            break;
        } else if (!strncmp(buffer, "N", strlen("N")) || !strncmp(buffer, "W", strlen("W")) || !strncmp(buffer, "S", strlen("S")) || !strncmp(buffer, "E", strlen("E"))) {
            checkDir(buffer[0]);
        }
    }
    read(sockfd, buffer, sizeof(buffer));
    printf("Reading %s\n", buffer);
    printf("Writing: hello\n");
    write(sockfd, "hello", strlen("hello"));


    printf("Closing sockets");
    close(sockfd);
    close(servfd);
    return 0;
}

void swap(char *str1, char *str2) {
    char *temp = (char *) malloc((strlen(str1) + 1) * sizeof(char));
    strcpy(temp, str1);
    strcpy(str1, str2);
    strcpy(str2, temp);
    free(temp);
}

void payload(char* payload) {
    int rc; // response code
    MQTTClient_message msg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    msg.payload = payload;
    msg.payloadlen = (int) strlen(payload);
    msg.qos = 1;
    msg.retained = 0;
    if ((rc = MQTTClient_publishMessage(client, TOPIC, &msg, &token)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to publish message, rc=%d\n", rc);
        exit(EXIT_FAILURE);
    }
    MQTTClient_waitForCompletion(client, token, 10000L);
    printf("Sending payload '%s'\n", payload);
}

void printFile(char *fileName) {
    char *path = NULL;
    char *output = NULL;
    strncat(path, fileName, 16);
    FILE *file = fopen(path, "r");
    fgets(output, sizeof(output), file);
    printf("%s", output); // replace w/ payload
    fclose(file);
}

void checkDir(char c) {
    char oldPrev[256];
    strcpy(oldPrev, prev);
    if (c == back) {
        swap(curr, prev);
    } else {
        strcpy(prev, curr);
        char temp[3];
        temp[0] = c;
        temp[1] = '/';
        temp[2] = '\0';
        strncat(curr, temp, 3);
    }
    DIR *dir = opendir(curr);
    if (dir) {
        back = c;
        printf("Curr: %s\n", curr);
        printFile("desc.txt");
        //printFile("item.txt");
    } else if (errno == ENOENT) {
        printf("You can't go there"); // replace payload
        strcpy(curr, prev);
        strcpy(prev, oldPrev);
    }
}