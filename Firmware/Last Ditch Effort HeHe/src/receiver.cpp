#include "functions.h"

//=================================================================
//        Main Function
//=================================================================
int main(void)
{
    // Initialize UART
    uart_init();

    DDRC = 0x03F; // Set LCD Port Direction

    char messageChar;

    // Set lower half of PORTB (PB0-PB3) as input and upper half (PB4-PB7) as output
    DDRB = 0xF0;
    PORTB = 0x0F; // Enable pull-up resistors for lower half

    delay_ms(500); // Initiaize LCD
    dispinit();

    delay_ms(500);

    display("Order System    ", 1);
    display("                ", 2);

    while (1)
    {
        messageChar = uart_recieve();
        processOrder(messageChar);
        delay_ms(500);
    }

    return 0;
}