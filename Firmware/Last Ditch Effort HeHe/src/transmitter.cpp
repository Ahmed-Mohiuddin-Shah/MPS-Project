// Complete Instructions to Get and Change ESP MAC Address: https://RandomNerdTutorials.com/get-change-esp32-esp8266-mac-address-arduino/

#include <SPI.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Keypad.h>
#include <SPI.h>
#include <Adafruit_I2CDevice.h>

//=================

int order_total = 0;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int lineNum = 0;

// Process Order Function
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

// Display Menu Function
// void displayMenu(void)
// {

//     static int count = 0;

//     switch (count++)
//     {
//     case 0:
//         display("1.Tea     Rs.100", 1);
//         display("2.Coffee  Rs.150", 2);
//         break;
//     case 1:
//         display("3.Biscuits Rs.40", 1);
//         display("4.Cookies Rs.250", 2);
//         break;
//     case 2:
//         display("5.Roti     Rs.30", 1);
//         display("6.Lunch   Rs.200", 2);
//         break;
//     case 3:
//         display("7.Dinner  Rs.250", 1);
//         display("8.BreakfastRs.90", 2);
//         break;
//     case 4:
//         display("9.Snacks  Rs.100", 1);
//         display("0.Water    Rs.20", 2);
//         break;
//     case 5:
//         display("# to Confirm    ", 1);
//         display("* to Cancel     ", 2);
//         break;
//     case 6:
//         display("Place Order     ", 1);
//         display("UwU             ", 2);
//         break;
//     default:
//         count = 0;
//         break;
//     }
// }

// Receiver Code
#include <esp_now.h>
#include <WiFi.h>

#define ROW_NUM 4    // four rows
#define COLUMN_NUM 4 // four columns

char keys[ROW_NUM][COLUMN_NUM] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

byte pin_rows[ROW_NUM] = {19, 18, 5, 17};    // GPIO19, GPIO18, GPIO5, GPIO17 connect to the row pins
byte pin_column[COLUMN_NUM] = {16, 4, 0, 2}; // GPIO16, GPIO4, GPIO0, GPIO2 connect to the column pins

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

// Structure example to receive data
// Must match the sender structure
typedef struct test_struct
{
    char key;
} test_struct;

// Create a struct_message called myData
test_struct myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
    // display.clearDisplay();
    memcpy(&myData, incomingData, sizeof(myData));
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    // display.println(myData.key);
    processOrder(myData.key);
    display.display();
}

void setup()
{
    // Initialize Serial Monitor
    Serial.begin(115200);

    // Initialize the OLED display
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }

    // Display the text
    display.display();
    delay(2000);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    // display.println("Hello, world!");
    display.display();

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Once ESPNow is successfully Init, we will register for recv CB to
    // get recv packer info
    esp_now_register_recv_cb(OnDataRecv);
}

void loop()
{
    // Display the text received
}

// Transmitter Code

// #include <esp_now.h>
// #include <WiFi.h>

// #define SCREEN_WIDTH 128 // OLED display width, in pixels
// #define SCREEN_HEIGHT 64 // OLED display height, in pixels

// #define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
// #define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

// #define ROW_NUM 4    // four rows
// #define COLUMN_NUM 4 // four columns

// char keys[ROW_NUM][COLUMN_NUM] = {
//     {'1', '2', '3', 'A'},
//     {'4', '5', '6', 'B'},
//     {'7', '8', '9', 'C'},
//     {'*', '0', '#', 'D'}};

// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// byte pin_rows[ROW_NUM] = {19, 18, 5, 17};    // GPIO19, GPIO18, GPIO5, GPIO17 connect to the row pins
// byte pin_column[COLUMN_NUM] = {16, 4, 0, 2}; // GPIO16, GPIO4, GPIO0, GPIO2 connect to the column pins

// Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

// // REPLACE WITH YOUR ESP RECEIVER'S MAC ADDRESS
// // ESP Board MAC Address : 9C : 9C : 1F : C4 : FA : 80
// uint8_t broadcastAddress1[] = {0x9C, 0x9C, 0x1F, 0xC4, 0xFA, 0x80};

// typedef struct test_struct
// {
//     char key;
// } test_struct;

// test_struct test;

// esp_now_peer_info_t peerInfo;

// // callback when data is sent
// void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
// {
//     char macStr[18];
//     Serial.print("Packet to: ");
//     // Copies the sender mac address to a string
//     snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
//              mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
//     Serial.print(macStr);
//     Serial.print(" send status:\t");
//     Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
// }

// void setup()
// {
//     Serial.begin(115200);

//     WiFi.mode(WIFI_STA);

//     if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
//     {
//         Serial.println(F("SSD1306 allocation failed"));
//         for (;;)
//             ;
//     }

//     if (esp_now_init() != ESP_OK)
//     {
//         Serial.println("Error initializing ESP-NOW");
//         return;
//     }

//     esp_now_register_send_cb(OnDataSent);

//     // register peer
//     peerInfo.channel = 0;
//     peerInfo.encrypt = false;
//     // register first peer
//     memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
//     if (esp_now_add_peer(&peerInfo) != ESP_OK)
//     {
//         Serial.println("Failed to add peer");
//         return;
//     }

//     display.display();
//     delay(2000);
//     display.clearDisplay();
//     display.setTextSize(1);
//     display.setTextColor(SSD1306_WHITE);
//     display.setCursor(0, 0);
//     // display menu items
//     display.println("1.Tea     Rs.100");
//     display.println("2.Coffee  Rs.150");
//     display.println("3.Biscuits Rs.40");
//     display.println("4.Cookies Rs.250");
//     display.println("5.Roti    Rs.30");
//     display.println("6.Lunch   Rs.200");
//     display.println("C for Confirm");
//     display.println("D for Cancel");
//     display.display();
// }

// int millisdelay = millis();
// esp_err_t result;
// void loop()
// {
//     test.key = keypad.getKey();

//     if (test.key)
//     {
//         if (millis() - millisdelay > 1000)
//         {
//             millisdelay = millis();
//             result = esp_now_send(broadcastAddress1, (uint8_t *)&test, sizeof(test));
//         }
//         else
//         {
//             millisdelay = millis();
//         }
//     }

//     if (result == ESP_OK)
//     {
//         Serial.println("Sent with success");
//     }
//     else
//     {
//         Serial.println("Error sending the data");
//     }
// }