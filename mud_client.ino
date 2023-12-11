#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <ezButton.h>

#define SDA 13
#define SCL 14
#define DEBOUNCE 50
#define PORT 8888
#define MAX_LEN 1024

#define W_BUTTON 12
//Button Green (S):
#define S_BUTTON 33
//Button Yellow (E):
#define E_BUTTON 27
//Button Red (N):
#define N_BUTTON 32

const char *user = "blueberrypie";
const char *pass = "bbbbaaaa";
const char *inTopic = "home/ubuntu/final";

int sockfd;
char buffer[MAX_LEN];

ezButton east(E_BUTTON);
ezButton north(N_BUTTON);
ezButton south(S_BUTTON);
ezButton west(W_BUTTON);

IPAddress server(35, 212, 252, 59);
WiFiClient espClient;
PubSubClient client(espClient);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
    Serial.begin(115200);

    // button stuff
    east.setDebounceTime(50);
    north.setDebounceTime(50);
    south.setDebounceTime(50);
    west.setDebounceTime(50);

    // lcd stuff
    Wire.begin(SDA, SCL);
    if (!i2CAddrTest(0x27))
    {
        lcd = LiquidCrystal_I2C(0x3F, 16, 2);
    }
    lcd.init();
    lcd.backlight();
    lcd.clear();

    // wifi stuff
    WiFi.begin(user, pass);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi!");

    // socket stuff
    struct sockaddr_in servaddr;
    // Creating TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        Serial.println("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("35.212.252.59"); // Change this to the server's IP
    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
    {
        Serial.println("Connection failed");
        exit(EXIT_FAILURE);
    }
    Serial.println("Connected to MUD server");

    // mqtt stuff
    client.setServer(server, 1883);
    client.setCallback(callback);
    delay(1500);
    client.subscribe(inTopic);
    // close(sockfd); implement somewhere
}

void loop()
{
    if (!client.connected())
    {
        reconnect();
    }
    east.loop();
    north.loop();
    south.loop();
    west.loop();
    if (east.isPressed())
    {
        send("e");
    }
    if (north.isPressed())
    {
        send("n");
    }
    if (west.isPressed())
    {
        send("w");
    }
    if (south.isPressed())
    {
        send("s");
    }
    client.loop();
}

void send(const char *msg)
{
    snprintf(buffer, sizeof(buffer), msg);
    printf(msg + "\n"); // todo remove
    write(sockfd, buffer, strlen(buffer));
}

bool i2CAddrTest(uint8_t addr)
{
    Wire.begin();
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0)
    {
        return true;
    }
    return false;
}

void reconnect()
{
    while (!client.connected())
    {
        Serial.print("Attempting to connect to MQTT...");
        if (client.connect("esp32"))
        {
            Serial.println("connected");
            client.subscribe(inTopic);
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" trying again in 5 seconds");
            delay(5000);
        }
    }
}

void callback(char *topic, byte *payload, unsigned int length)
{
    lcd.setCursor(0, 0);
    for (int i = 0; i < length; i++)
    {
        if (i == 16)
            lcd.setCursor(0, 1);
        if (i == 32)
            return;
        lcd.print((char)payload[i]);
    }
}