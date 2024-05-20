#include <Arduino.h>

#include <SPI.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Keypad.h>

#include <HardwareSerial.h>

#define SERIAL1 12
#define SERIAL2 2

HardwareSerial ESPSerial1(SERIAL1);

#define COMPORT ESPSerial1

void comportInit(long baudrate);
int comportReceive(byte *data, int length);
int comportTransmit(byte *data, int length);

#define MSGBUFFERSIZE 80
byte pData[MSGBUFFERSIZE];

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define UART_NUM UART_NUM_1

#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define ROW_NUM 4    // four rows
#define COLUMN_NUM 4 // four columns

char keys[ROW_NUM][COLUMN_NUM] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

byte pin_rows[ROW_NUM] = {19, 18, 5, 17};    // GPIO19, GPIO18, GPIO5, GPIO17 connect to the row pins
byte pin_column[COLUMN_NUM] = {16, 4, 0, 2}; // GPIO16, GPIO4, GPIO0, GPIO2 connect to the column pins

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }

    comportInit(9600);

    // driver = RH_ASK(uint16_t speed = 2000, uint8_t rxPin = 11, uint8_t txPin = 12, uint8_t pttPin = 10, bool pttInverted = false);

    // RMT.begin();

    display.display();
    delay(2000);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Hello, world!");
    display.display();
}

void loop()
{

    comportReceive(pData, 2);

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    // pData contents

    display.println(pData[0]);
    display.println(pData[1]);

    char key = keypad.getKey();

    if (key)
    {
        Serial.println(key);
    }
}

void comportInit(long baudrate)
{
    COMPORT.begin(baudrate);
}

int comportReceive(byte *data, int length)
{
    int i = 0;
    while (COMPORT.available() > 0 && i < length)
    {
        data[i] = COMPORT.read();
        i++;
    }
    return i;
}

int comportTransmit(byte *data, int length)
{
    COMPORT.write(data, length);
    return length;
}