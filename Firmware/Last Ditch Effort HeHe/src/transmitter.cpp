#include "functions.h"
#include <stdio.h>

uint8_t swap = 0;
// interrupt handler for INT0
ISR(INT0_vect)
{
  swap = swap ? 0 : 1;
}

//=================================================================
//        Main Function
//=================================================================
int main(void)
{
  // Initialize UART
  uart_init();

  DDRC = 0x03F; // Set LCD Port Direction

  timer_init(); // Initialize timer

  // interrupt setup
  GICR |= (1 << INT0);                     // enable INT0;
  MCUCR &= ~((1 << ISC01) | (1 << ISC00)); // INT0 low level trigger
  sei();                                   // enable global interrupt
  DDRD = 0;                                // all PD pins configured as input
  PORTD = (1 << PD2);                      // enable pull-up on the INT0 pin

  // Set lower half of PORTB (PB0-PB3) as input and upper half (PB4-PB7) as output
  DDRB = 0xF0;
  PORTB = 0x0F; // Enable pull-up resistors for lower half

  delay_ms(500); // Initiaize LCD
  dispinit();

  delay_ms(500);

  display("Welcome to", 1);
  display("Canteen", 2);
  delay_ms(8000);

  while (1)
  {

    if (swap)
    {
      uart_transmit('!');
      clearDisplay();
      display("Order Total     ", 1);
      char total[16];
      sprintf(total, "Rs.%d", order_total);
      display(total, 2);
      delay_ms(6000);
    }
    else
    {
      displayMenu();
      timer_delay_ms(2000);
    }
  }

  return 0;
}