#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <asm-generic/errno.h>

#include "MQTTClient.h"

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "MUD"
#define TOPIC       "home/ubuntu/final"

#define STACK_SIZE 64
#define MAX_LEN 1024
#define PORT 8888

void checkDir(char c);
void payload(char* payload);

void push(char c);
char pop();
char peek();
int empty();

MQTTClient client;
char buffer[MAX_LEN];
char curr[256] = "./start/";

int top = -1;
char stack[STACK_SIZE];

extern int errno;

int main() {
    printf("Attempting to initialize MQTT client...\n");
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc; // response code
    if ((rc = MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS) {
        printf("MQTT client initialization unsuccessful, rc=%d\n", rc);
        exit(EXIT_FAILURE);
    }
    conn_opts.keepAliveInterval = 0; // prevents bug with keep alive packets (https://github.com/eclipse/paho.mqtt.c/issues/1330)
    conn_opts.cleansession = 1; // make it clean
    conn_opts.connectTimeout = 5; // timeout 5 seconds
    printf("MQTT client initialization successful\n");
    printf("Attempting to connect to broker...\n");
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Broker connection unsuccessful, timeout=5s, rc=%d\n", rc);
        exit(EXIT_FAILURE);
    }
    printf("Broker connection successful\n");

    MQTTClient_message msg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    int listenfd, sockfd;
    struct sockaddr_in servaddr;
    // Creating socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    // Binding socket
    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) != 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }
    // Listening for connections
    if (listen(listenfd, 5) != 0) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }
    printf("Waiting for client...\n");
    sockfd = accept(listenfd, (struct sockaddr *) NULL, NULL);
    if (sockfd < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    printf("Client connected\n");

    while (1) {
        read(sockfd, buffer, sizeof(buffer));
        if (!strncmp(buffer, "exit", strlen("exit"))) {
            break;
        } else if (!strncmp(buffer, "n", strlen("n")) || !strncmp(buffer, "w", strlen("w")) || !strncmp(buffer, "s", strlen("s")) || !strncmp(buffer, "e", strlen("e"))) {
            checkDir(buffer[0]);
        }
    }

    close(sockfd);
    close(listenfd);
    printf("Closed sockets\n");
    if ((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS) {
        printf("MQTT client disconnection failed, rc=%d\n", rc);
    }
    MQTTClient_destroy(&client);
    return 0;
}

void push(char c) {
    if (top == STACK_SIZE - 1) {
        perror("Stack overflow");
        exit(EXIT_FAILURE);
    } else {
        top++;
        stack[top] = c;
    }
}

char pop() {
    char result = peek();
    top--;
    return result;
}

char peek() {
    if (empty()) {
        perror("Empty stack");
        exit(EXIT_FAILURE);
    } else {
        return stack[top];
    }
}

int empty() {
    return (top == -1) ? 1 : 0;
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
    msg.qos = 0;
    msg.retained = 0;
    if ((rc = MQTTClient_publishMessage(client, TOPIC, &msg, &token)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to publish message, rc=%d\n", rc);
        exit(EXIT_FAILURE);
    }
    MQTTClient_waitForCompletion(client, token, 10000L);
}

void printFile(char *fileName) {
    char path[256]; // file path
    char output[32]; // file output
    strcpy(path, curr);
    strncat(path, fileName, 16);
    // if file exists
    if (!access(path, F_OK)) {
        FILE *file = fopen(path, "r");
        fgets(output, sizeof(output), file);
        output[strcspn(output, "\n")] = 0; // remove newline
        payload(output);
        fclose(file);
    }
}

void checkDir(char c) {
    // peek from stack to navigate
    char back = 0;
    if (!empty()) {
        back = peek();
    }
    if (c == back) {
        // backtrack, cut off last 2 characters to go back a directory
        curr[strlen(curr) - 2] = '\0';
        pop();
    } else {
        char temp[3];
        temp[0] = c;
        temp[1] = '/';
        temp[2] = '\0';
        strncat(curr, temp, 3);
        if (c == 'n') push('s');
        if (c == 'w') push('e');
        if (c == 's') push('n');
        if (c == 'e') push('w');
    }

    DIR *dir = opendir(curr);
    if (dir) {
        printFile("desc.txt");
        printFile("item.txt");
    } else if (errno == ENOENT) {
        payload("You can't go there");
        curr[strlen(curr) - 2] = '\0';
        pop();
    }
}