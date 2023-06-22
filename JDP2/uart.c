/* uart.c */

#include "uart.h"

// initialize the UART
void uart_init(void){
    UCSR0B = (1 << TXEN0);    // enable uart transmitter
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  // set 8 bit character size
    UBRR0L = 103;   // set baud rate to 9600 for 16 MHz crystal
}

// send a single character to serial monitor via UART
void uart_send(unsigned char ch){
    while( !(UCSR0A & (1 << UDRE0)) );  // wait until tx buffer is empty
    UDR0 = ch;      // write character to usart data register
}

// send a string char by char to UART via uart_send()
void send_string(char *stringAddress){
    for(unsigned char i = 0; i < strlen(stringAddress); i++){
        uart_send(stringAddress[i]);
    }
}