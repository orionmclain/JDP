// Orion McLain JDP ECE 297B Final Build

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <math.h>
#include "i2c.h"
#include "SSD1306.h"

// 7 Segment Pins
/*
A = PD0 (pin 2)
B = PD1 (pin 3)
C = PD2 (pin 4)
D = PD3 (pin 5)
E = PD4 (pin 6)
F = PD5 (pin 11)
G = PD6 (pin 12)
DP = PD7 (pin 13)

D1 = PC0 (pin 23)
D2 = PC1 (pin 24)
D3 = PC2 (pin 25)
D4 = PC3 (pin 26)
*/

// 7 Segment Definitions
#define D1 PC0 // (pin 23)
#define D2 PC1 // (pin 24)
#define D3 PC2 // (pin 25)
#define D4 PC3 // (pin 26)

// Sonar Definitions
#define TRIG PB1 // pin 15

// LED Definititions
#define green PB2 // pin 16
#define red PB5  // pin 19

void OLED(int inches);

int main() {
    // Set 7 Segment Pins as Output
    DDRD = 0xFF;
    DDRC |= ( (1 << D1) | (1 << D2) | (1 << D3) | (1 << D4) );

    // Sonar Code
    DDRB = 0x00;
    DDRB |= ( (1<<TRIG) | (1 << green) | (1 << red));  //  Trigger output, sends the pulse

    // Sonar Variables
    int ttRising, ttFalling, pwm, distance, distanceFeet;
    int minInches = 120;

    TCCR1A = 0;  // for the sonar timar

    OLED_Init();  //initialize the OLED

    while(1)
    {
        // Get distance from sonar sensor
        PORTB |= (1<<TRIG);
        _delay_us(10);
        PORTB &= ~(1<<TRIG);
        TCNT1 = 0x00;
        TCCR1B = 0x45;
        while ((TIFR1 & (1<<ICF1)) == 0);
        ttRising = ICR1;
        TIFR1 = (1<<ICF1);
        TCCR1B = 0x05;
        while ((TIFR1 & (1<<ICF1)) == 0);
        ttFalling = ICR1;
        TIFR1 = (1<<ICF1);
        pwm = ttFalling - ttRising;

        distance = (pwm * 1.098)/2.54;  // convert to inches

        distanceFeet = distance / 12;  // convert to feet

        if (distanceFeet < 10){  // only measure within 10 feet

            if (distance < minInches){  // update OLED minimum distance
                minInches = distance;
            }

            OLED(minInches);  // display minimum distance to OLED 

            if (distanceFeet < 6){  // red led
                PORTB &= ~(1 << green);  // turns off green light
                PORTB |= (1 << red);  // turns on red light
            }

            else{  // green led
                PORTB &= ~(1 << red);  // turns off red light
                PORTB |= (1 << green);  // turns on green light
            }

            // Activate D1
            PORTC |= ((1 << D2) | (1 << D3) | (1 << D4)); 
            PORTC &= ~(1 << D1);

            // Call correct digit function
            switch (distanceFeet) {
                case 0:
                    PORTD = 0x3F;
                    break;
                case 1:
                    PORTD = 0x06;
                    break;
                case 2:
                    PORTD = 0x5B;
                    break;
                case 3:
                    PORTD = 0x4F;
                    break;
                case 4:
                    PORTD = 0x66;
                    break;
                case 5:
                    PORTD = 0x6D;
                    break;
                case 6:
                    PORTD = 0x7D;
                    break;
                case 7:
                    PORTD = 0x07;
                    break;
                case 8:
                    PORTD = 0x7F;
                    break;
                case 9:
                    PORTD = 0x67;
                    break;
            }

            _delay_ms(5);

            // Activate D2, show F
            PORTC |= ((1 << D1) | (1 << D3) | (1 << D4)); 
            PORTC &= ~(1 << D2);
            PORTD = 0x71; 

            _delay_ms(5);

            // Activate D4
            PORTC |= ((1 << D1) | (1 << D2) | (1 << D3)); 
            PORTC &= ~(1 << D4);

            // Call correct digit function
            switch (distance % 12) {
                case 0:
                    PORTD = 0x3F;
                    break;
                case 1:
                    PORTD = 0x06;
                    break;
                case 2:
                    PORTD = 0x5B;
                    break;
                case 3:
                    PORTD = 0x4F;
                    break;
                case 4:
                    PORTD = 0x66;
                    break;
                case 5:
                    PORTD = 0x6D;
                    break;
                case 6:
                    PORTD = 0x7D;
                    break;
                case 7:
                    PORTD = 0x07;
                    break;
                case 8:
                    PORTD = 0x7F;
                    break;
                case 9:
                    PORTD = 0x67;
                    break;
                case 10:
                    PORTD = 0x67;
                    break;
                case 11:
                    PORTD = 0x67;
                    break;
            }
            _delay_ms(5);
        }
    }
}

// OLED function to print minimum distance
void OLED(int inches) {  
    OLED_GoToLine(1);
    OLED_DisplayNumber(10, inches, 3);
    OLED_DisplayString(" Inches ");
}