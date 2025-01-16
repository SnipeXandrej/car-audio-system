#include "lcd.c"
#include "lcd.h"
#include "uart.c"
#include "uart.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <stdbool.h>
#include <util/delay.h>
// #include <unistd.h>

#define P0 0b00000001
#define P1 0b00000010
#define P2 0b00000100
#define P3 0b00001000
#define P4 0b00010000
#define P5 0b00100000
#define P6 0b01000000
#define P7 0b10000000

void removeSubstr (char *string, char *sub) {
    char *match;
    int len = strlen(sub);
    while ((match = strstr(string, sub))) {
        *match = '\0';
        strcat(string, match+len);
    }
}

void f_pc_power(bool power) {
    if (power) {
        PORTB = PORTB | P0; // turns ON relay that powers the computer on pin 0
    } else {
        uart_puts("PC_POWER_OFF\n");
        // timer that waits 15 seconds
        _delay_ms(12000);
        PORTB = PORTB ^ P0; // turns OFF relay that powers the computer pin 0
    }
}

//ADC
volatile uint16_t adcValues[8];  // Array to store ADC values for 8 channels
volatile uint8_t currentChannel = 0;  // Current ADC channel being read

ISR(ADC_vect) {
  adcValues[currentChannel] = ADC;  // Store the ADC result
  currentChannel = (currentChannel + 1) % 8;  // Move to the next channel (0 to 7)

  // Update ADMUX to select the next channel
  ADMUX = (ADMUX & 0xF8) | currentChannel;  // Clear MUX0-MUX2 bits and set new channel

  // Start the next conversion
  ADCSRA |= (1 << ADSC);
}

void ADC_Init() {
  ADMUX = 0b01000000;  // AVCC with external capacitor at AREF pin, start with ADC0
  ADCSRA = 0b11101111;  // Enable ADC, enable ADC interrupt, set prescaler to 128, start first conversion
}

int main(void) {
  //ADC
  // ADMUX = 0b01000101;
  // ADCSRA = 0b11101111;
  ADC_Init();

  char pole[32];
  char text[32];
  // char lcd_text[32];
  int cislo = 0;
  uart_init(4800);
  // lcd_init(LCD_DISP_ON);
  // int smer = 1; // 1 = vpravo, 0 = vlavo
  // int rychlost = 6;
  DDRB = 255;
  PORTB = 0;

  DDRD = 0b00000000;
  PORTD = 0b00000100;

  sei(); // enables interrupts, needed for uart

  bool p2_pressed = false;
  bool pc_powerswitch = false;

  // PORTB = 255;           0b11111111
  // PORTB = PORTB ^ P0;    0b01111111 // High to Low
  // PORTB = PORTB ^ P5;    0b01111011
  // PORTB = PORTB | P0;    0b11111011 // Low to High

  while (1) {
      // Power Button
      // if (((PIND & P2) == 0)) {
      //     if (!p2_pressed) {
      //       p2_pressed = true;
      //       uart_puts("oof\n");
      //       // f_power_on();
      //     }
      // } else {
      //     p2_pressed = false;
      // }

      // Power Switch
      if (((PIND & P2) == 0)) {
          if (!p2_pressed) {
            p2_pressed = true;
            uart_puts("Power on...\n");
            f_pc_power(true);
          }
      } else {
          if (p2_pressed) {
            p2_pressed = false;
            uart_puts("Power off...\n");
            f_pc_power(false);
          }
      }

      // lcd_gotoxy(0, 0);
      // sprintf(text,"pole:%2d", pole);
      // lcd_puts(text);
      // lcd_puts("testt");

        // uart_puts("ACHTUNG!");
        // delay(500000);
        // sprintf(text, "%d\n", adcValues[0]);
        // uart_puts(text);

        // lcd_gotoxy(0, 0);
        // sprintf(text,"Text :)");
        // lcd_puts(text);


    if (uart_gets(pole)) {
      // cislo=atoi(pole);
      // uart_puts("ruchlust");
      // blik(cislo);

        sprintf(text,"pole:%2d", pole);
        uart_puts(pole);


        // lcd_gotoxy(0, 0);
        // lcd_clrscr();
        sprintf(text,"%s", pole);
        // lcd_puts(text);


        // if (strstr(pole, "Song:") != NULL) {
        //     removeSubstr(pole, "Song: ");
        //     lcd_gotoxy(0, 0);
        //     sprintf(text,"%s", pole);
        //     lcd_puts(text);
        // }

        if (strstr(pole, "Peak:") != NULL) {
            removeSubstr(pole, "Peak: ");
            cislo=atoi(pole);

            // updateProgressBar(progressBar, cislo, barMaxValue, barLength);
            // sprintf(lcd_text, "[%s] %d%%", progressBar, (cislo * 100) / total);
            // sprintf(lcd_text, "[%s] %d%%", progressBar, cislo / barMaxValue);
            // lcd_gotoxy(0, 1);
            // lcd_puts(lcd_text);

            // *pole = bar(20, 0, 30);
            // char *pole = "[Wooow  !]";
            // char *pole = bar(cislo, 0, 30);
            // // lcd_gotoxy(0, 0);
            // // print_progress(cislo, 10);
            // // sprintf(text,"%s", pole);
            // // lcd_puts(text);

            switch (cislo) {
                case 0: PORTB = 0b00000000; break;
                case 1: PORTB = 0b00000001; break;
                case 2: PORTB = 0b00000011; break;
                case 3: PORTB = 0b00000111; break;
                case 4: PORTB = 0b00001111; break;
                case 5: PORTB = 0b00011111; break;
                case 6: PORTB = 0b00111111; break;
                case 7: PORTB = 0b01111111; break;
                case 8: PORTB = 0b11111111; break;
            }
        }
    }
  }
}
