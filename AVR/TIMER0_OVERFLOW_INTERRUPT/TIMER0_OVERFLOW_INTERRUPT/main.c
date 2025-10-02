/*
 * TIMER0_OVERFLOW_INTERRUPT.c
 *
 * Created: 02-10-2025 21:35:50
 * Author : KICHU
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
volatile uint16_t count=0;
ISR(TIMER0_OVF_vect) //INTERRUPT SERVICE ROUTINE (TIMER0_OVF_vect = INTERRUPT VECTOR TABLE "ADDRESS" FOR TIMER OVERFLOW INTERRUPT)
{
	if(count>=977)
	{
		PORTB^=(1<<PORTB5);
		count=0;
	}
	count++;
}
int main(void)
{
	DDRB|=(1<<DDB5);
	
	TCCR0A=0;//NORMAL MODE
	TCCR0B|=(1<<CS00)|(1<<CS01);//64 PRESCALAR
	TIMSK0|=(1<<TOIE0);//ENABLINH TIMER OVERFLOW INTERRUPT FOR TIMER 0 IN NORMAL MODE
	sei();
	
    while (1) 
    {
    }
}

