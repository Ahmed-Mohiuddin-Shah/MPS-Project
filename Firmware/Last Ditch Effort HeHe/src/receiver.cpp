#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <esp_now.h>
#include <WiFi.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

int lineNum = 0;
int order_total = 0;

typedef struct data_struct
{
    char key;
} data_struct;

data_struct myData;

// Function Prototypes
void processOrder(char key);
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
    Serial.begin(115200);

    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }
    display.display();
    delay(2000);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.display();

    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    esp_now_register_recv_cb(OnDataRecv);
}

void loop()
{
    // Display the text received
}

void processOrder(char key)
{
    char order[16];
    switch (key)
    {
    case '1':
        strcpy(order, "Tea      Rs.100");
        order_total += 100;
        lineNum += 1;
        break;
    case '2':
        strcpy(order, "Coffee   Rs.150");
        order_total += 150;
        lineNum += 1;
        break;
    case '3':
        strcpy(order, "Biscuits  Rs.40");
        order_total += 40;
        lineNum += 1;
        break;
    case '4':
        strcpy(order, "Cookies  Rs.250");
        order_total += 250;
        lineNum += 1;
        break;
    case '5':
        strcpy(order, "Roti      Rs.30");
        order_total += 30;
        lineNum += 1;
        break;
    case '6':
        strcpy(order, "Lunch    Rs.200");
        order_total += 200;
        lineNum += 1;
        break;
    case '7':
        strcpy(order, "Dinner   Rs.250");
        order_total += 250;
        lineNum += 1;
        break;
    case '8':
        strcpy(order, "BreakfastRs.150");
        order_total += 150;
        lineNum += 1;
        break;
    case '9':
        strcpy(order, "Snacks   Rs.100");
        order_total += 100;
        lineNum += 1;
        break;
    case '0':
        strcpy(order, "Water    Rs.20");
        order_total += 20;
        lineNum += 1;
        break;
    case 'C':
        strcpy(order, "Order Confirmed");
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.println(order);
        display.println("Order Total     ");
        char total[16];
        sprintf(total, "Rs.%d", order_total);
        display.println(total);
        display.display();
        order_total = 0;
        lineNum = 0;
        return;
        break;
    case 'D':
        strcpy(order, "Order Cancelled");
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.println(order);
        display.display();
        lineNum = 0;
        order_total = 0;
        break;
    default:
        return;
        break;
    }
    // display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    for (int i = 0; i < lineNum; i++)
    {
        display.println("");
    }
    display.println(order);
    display.display();
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
    memcpy(&myData, incomingData, sizeof(myData));
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    processOrder(myData.key);
    display.display();
}