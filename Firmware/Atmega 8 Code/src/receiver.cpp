//=================================================================
/* LCD Pin-5(R/W) must be connected to ground*/
//=================================================================
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>

#define E PC5
#define RS PC4

#define F_CPU 1000000UL

#define BAUD 4800
#define BAUDRATE (F_CPU / 16 * BAUD) - 1

// Define keypad layout
char keys[4][3] = {{'1', '2', '3'},
                   {'4', '5', '6'},
                   {'7', '8', '9'},
                   {'*', '0', '#'}};

// Function Prototypes
char keypad_scan(void);
void display(char string[16], char LineNo);
void displaybyte(char D);
void dispinit(void);
void epulse(void);
void delay_ms(unsigned int de);
char getKeyPad(void);

void uart_init(void);
void uart_transmit(unsigned char data);
unsigned char uart_recieve(void);
void usart_msg(char *c);

//=================================================================
//        UART Functions
//=================================================================
void uart_init(void)
{
    UBRRH = 0x00;                                                    // shift the register right by 8 bits
    UBRRL = BAUDRATE;                                                // set baud rate
    UCSRB |= (1 << TXEN) | (1 << RXEN);                              // enable receiver and transmitter
    UCSRC |= (1 << USBS) | (3 << UCSZ0) | (1 << UPM1) | (0 << UPM0); // 8bit data format,Asynchronous,Even parity
}

// function to send data
void uart_transmit(unsigned char data)
{

    while (!(UCSRA & (1 << UDRE))) // wait while register is free
    {
    }
    UDR = data; // load data in the register
}

// function to receive data
unsigned char uart_recieve()
{
    while (!(UCSRA & (1 << RXC))) // wait while data is being received
    {
    }
    return (UDR); // return 8-bit data
}

void usart_msg(char *c)
{
    while (*c != 0)
        uart_transmit(*c++);
}

//=================================================================
//        Main Function
//=================================================================
int main(void)
{
    DDRC = 0x03F; // Set LCD Port Direction

    char key;

    // Set lower half of PORTB (PB0-PB3) as input and upper half (PB4-PB7) as output
    DDRB = 0xF0;
    PORTB = 0x0F; // Enable pull-up resistors for lower half

    uart_init();

    delay_ms(500); // Initiaize LCD
    dispinit();
    delay_ms(200);
    display(" Hi There....", 1);
    display("Receiver", 2);
    delay_ms(2000);

    while (1)
    {
        key = uart_recieve();
        display("Key: ", 1);
        display(&key, 2);
        delay_ms(2000);
    }

    return 0;
}

//=================================================================
//        LCD Display Initialization Function
//=================================================================
void dispinit(void)
{
    int count;
    char init[] = {0x43, 0x03, 0x03, 0x02, 0x28, 0x01, 0x0C, 0x06, 0x02, 0x02};

    PORTC &= ~(1 << RS); // RS=0
    for (count = 0; count <= 9; count++)
    {
        displaybyte(init[count]);
    }
    PORTC |= 1 << RS; // RS=1
}

//=================================================================
//        Enable Pulse Function
//=================================================================
void epulse(void)
{
    PORTC |= 1 << E;
    delay_ms(5); // Adjust delay if required
    PORTC &= ~(1 << E);
    delay_ms(5); // Adjust delay if required
}

//=================================================================
//        Send Single Byte to LCD Display Function
//=================================================================
void displaybyte(char D)
{
    // data is in Temp Register
    char K1;
    K1 = D;
    K1 = K1 & 0xF0;
    K1 = K1 >> 4; // Send MSB

    PORTC &= 0xF0;
    PORTC |= (K1 & 0x0F);
    epulse();

    K1 = D;
    K1 = K1 & 0x0F; // Send LSB
    PORTC &= 0xF0;
    PORTC |= K1;
    epulse();
}

//=================================================================
//        Display Line on LCD at desired location Function
//=================================================================
void display(char string[16], char LineNo)
{
    int len, count;

    PORTC &= ~(1 << RS); // RS=0 Command Mode

    if (LineNo == 1)
    {
        displaybyte(0x80); // Move Coursor to Line 1
    }
    else
    {
        displaybyte(0xC0); // Move Coursor to Line 2
    }
    PORTC |= (1 << RS); // RS=1 Data Mode

    len = strlen(string);
    for (count = 0; count < len; count++)
    {
        displaybyte(string[count]);
    }
}

//=================================================================
//        Delay Function
//=================================================================
void delay_ms(unsigned int de)
{
    unsigned int rr, rr1;
    for (rr = 0; rr < de; rr++)
    {

        for (rr1 = 0; rr1 < 30; rr1++) // 395
        {
            asm("nop");
        }
    }
}

//=================================================================
//        Get Key from Keypad Function
//=================================================================
/*
We are using Atmega8 with 4x3 Keypad

pins a, b, c, d are connected to PORTB 0,1,2,3
pins 1, 2, 3 are connected to PORTB 4,5,6

*/

// Function Definitions
char keypad_scan(void)
{
    for (uint8_t c = 0; c < 3; c++)
    {
        PORTB = ~(1 << (c + 4)); // Enable one column at a time

        for (uint8_t r = 0; r < 4; r++)
        {
            if (!(PINB & (1 << r))) // Check each row
            {
                while (!(PINB & (1 << r)))
                    ;          // Wait for key release
                _delay_ms(50); // Debounce delay
                return keys[r][c];
            }
        }
    }
    return 0; // No key pressed
}