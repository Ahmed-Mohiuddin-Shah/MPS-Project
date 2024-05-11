# Restaurant Management System

This project implements a keypad-based restaurant management system using an Atmega8 microcontroller. It allows customers to send orders through a keypad to a receiver displaying the orders on an LCD screen.

## Features

- Keypad for order entry (1-9, *, 0, #)
- LCD display for order viewing
- UART communication for data transmission between transmitter and receiver

## Requirements

- Atmega8 microcontroller
- 4x3 Keypad
- LCD display
- UART module

## Software

The system is written in C and compiled using an AVR compiler. The code includes functionalities for:

- Initialization of keypad, LCD display, and UART module
- Scanning keypad for customer input
- Processing orders based on keypad input
- Displaying orders on the receiver's LCD screen

## How to Use

1. Connect the keypad, LCD display, and UART module to the Atmega8 microcontroller according to the provided circuit diagram.
2. Build and load the provided software (transmitter.hex and receiver.hex) onto the Atmega8 microcontroller using an appropriate programmer.
3. Customers can then use the keypad to enter their orders. The orders will be displayed on the LCD screen of the receiver.