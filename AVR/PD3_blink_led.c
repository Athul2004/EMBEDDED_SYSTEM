/*
 * PD3_BLINK_LED.c
 *
 * Created: 10-09-2025 20:43:08
 * Author : KICHU
 */ 
//1. Write a program to blink LED Connected to PD3 pin of an ATMEGA 328. Simulate in Siumul ide


#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRD = 0b00001000;    // Set PD3 as output (bit3 = 1)

	while (1)
	{
		PORTD = 0b00001000;  // LED ON (PD3 = High)
		_delay_ms(1000);

		PORTD = 0b00000000;  // LED OFF (PD3 = Low)
		_delay_ms(1000);
	}
}



