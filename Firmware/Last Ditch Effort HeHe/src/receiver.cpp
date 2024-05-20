#include <Arduino.h>

// ESP Board MAC Address : 9C : 9C : 1F : C4 : FA : 80

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


void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);


    comportInit(9600);

    // driver = RH_ASK(uint16_t speed = 2000, uint8_t rxPin = 11, uint8_t txPin = 12, uint8_t pttPin = 10, bool pttInverted = false);

    // RMT.begin();


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