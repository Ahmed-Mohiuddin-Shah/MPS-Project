#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define E PC5
#define RS PC4

#define F_CPU 1000000UL

#define BAUD 9600
#define BAUDRATE (F_CPU / 16 * BAUD) - 1

#define DELAY_FOR_MENU 200

// Define keypad layout
char keys[4][3] = {{'1', '2', '3'},
                   {'4', '5', '6'},
                   {'7', '8', '9'},
                   {'*', '0', '#'}};

// Function Prototypes

// LCD Functions
void dispinit(void);
void epulse(void);
void displaybyte(char D);
void display(char string[16], int LineNo);
void clearDisplay(void);
void displayMenu(void);

// Keypad Functions
void processOrder(char key);
char keypad_scan(void);
char getKeyPad(void);

// Delay Functions
void delay_ms(unsigned int de);
void timer_delay_ms(unsigned int de);

// UART Functions
void uart_init(void);
void uart_transmit(unsigned char data);
unsigned char uart_recieve(void);
void usart_msg(char *c);

//=================================================================
//        UART Functions
//=================================================================

// function to initialize UART
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
//        Clear Display Function
//=================================================================
void clearDisplay(void)
{
    PORTC &= ~(1 << RS); // RS=0 Command Mode
    displaybyte(0x01);   // Clear Display
    delay_ms(5);
}

//=================================================================
//        Process Order Function
//=================================================================
void processOrder(char key)
{
    char order[16];
    switch (key)
    {
    case '1':
        strcpy(order, "Tea  Rs.10");
        break;
    case '2':
        strcpy(order, "Coffee  Rs.15");
        break;
    case '3':
        strcpy(order, "Vadapav  Rs.20");
        break;
    case '4':
        strcpy(order, "Idli  Rs.25");
        break;
    case '5':
        strcpy(order, "Dosa  Rs.25");
        break;
    case '6':
        strcpy(order, "Lunch  Rs.60");
        break;
    case '#':
        strcpy(order, "Order Confirmed");
        break;
    case '*':
        strcpy(order, "Order Cancelled");
        break;
    default:
        return;
        break;
    }

    clearDisplay();
    display(order, 1);
    uart_transmit(key);
}

//=================================================================
//        Display Menu Function
//=================================================================
void displayMenu(void)
{

    delay_ms(200);
    display("Welcome to", 1);
    display("Cato Restaurant", 2);
    timer_delay_ms(DELAY_FOR_MENU);

    clearDisplay();

    display("Press Key to", 1);
    display("Select Order", 2);
    timer_delay_ms(DELAY_FOR_MENU);

    clearDisplay();

    display("# to Confirm", 1);
    display("* to Cancel", 2);
    timer_delay_ms(DELAY_FOR_MENU);

    clearDisplay();

    display("1.Tea  Rs.10   ", 1);
    display("2.Coffee  Rs.15", 2);
    timer_delay_ms(DELAY_FOR_MENU);

    display("3.Vadapav  Rs.20", 1);
    display("4.Idli  Rs.25   ", 2);
    timer_delay_ms(DELAY_FOR_MENU);

    display("5.Dosa  Rs.25   ", 1);
    display("6.Lunch  Rs.60  ", 2);
    timer_delay_ms(DELAY_FOR_MENU);

    clearDisplay();
    display("Place Order     ", 1);
    delay_ms(200);
}

//=================================================================
//        Display Line on LCD at desired location Function
//=================================================================
void display(char string[16], int LineNo)
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
//        Timer0 Delay Function
//=================================================================

uint8_t timerOverflowCount = 0;

void timer_delay_ms(unsigned int de)
{

    de = de / 1000;
    de = de * 261;

    while (1)
    {
        while ((TIFR & 0x01) == 0)
            ;
        TCNT0 = 0x00;
        TIFR = 0x01; // clear timer1 overflow flag
        timerOverflowCount++;
        processOrder(keypad_scan());
        if (timerOverflowCount >= de)
        {
            timerOverflowCount = 0;
            break;
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

// Function to scan the keypad 
// Returns the key pressed
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