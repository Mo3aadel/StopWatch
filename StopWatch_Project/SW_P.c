/*
 * SW_P.c
 *
 *  Created on: Apr 12, 2023
 *      Author: Mohamed Adel Amin
 */
#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>

unsigned char seconds=0;
unsigned char minutes=0;
unsigned char hours=0;


ISR(TIMER1_COMPA_vect)
{
	seconds++;
	if(seconds == 60)
	{
	seconds = 0;
	minutes++;
	}
	if(minutes == 60)
	{
	minutes = 0;
	hours++;
	}
	if(hours > 23)
	hours = 0;
}
ISR(INT0_vect)
{
	seconds=0;
	minutes=0;
	hours=0;

}
ISR(INT1_vect)
{
	TCCR1B &= ~(1<<CS10) &~(1<<CS12);

}
ISR(INT2_vect)
{
	TCCR1B = (1<<WGM12) | (1<<CS10)| (1<<CS12);

}
void INT0_Init(void)
{
	GICR|=(1<<INT0);
	MCUCR|=(1<<ISC01);//falling edge
}
void INT1_Init(void)
{
	GICR|=(1<<INT1);
	MCUCR|=(1<<ISC10)|(1<<ISC11);//Rising edge
}
void INT2_Init(void)
{
	GICR|=(1<<INT2);

}
void Timer1_Init(unsigned int CompareValue)
{
	TCNT1 = 0;

	OCR1A = CompareValue;  // compare value = 1000ms

	TIMSK |= (1<<OCIE1A); // Enabling Timer1 CompareA Interrupt

	TCCR1A = (1<<FOC1A)|(1<<FOC1B);

	TCCR1B = (1<<WGM12) | (1<<CS10)| (1<<CS12);// CTC Mode 4

}
int main(void)
{
	DDRC |= 0x0F;
	PORTC|=0x0F;
	DDRA |= 0x3F;
	PORTA|= 0x3F;//enabling 6 control pins in portA
	SREG |= (1<<7); // Enable global interrupts in MC
	DDRD &= 0xFC;//PD2 and PD3 as input pins
	DDRB &= 0xFB;//PB2 as input pin
	PORTD |=(1<<PD2);//internal pull up resistor configuration
	PORTB |=(1<<PB2);//internal pull up resistor configuration


	INT0_Init();// Enable external INT0
	INT1_Init();// Enable external INT1
	INT2_Init();// Enable external INT2

	Timer1_Init(1000);


	while(1)
	{
    	PORTC =(PORTC&0xF0)|(seconds%10);//display second 1
		PORTA &=0xC0;//disable initialized PORTA pins
		PORTA |= 0x20;//enable PA5 for second 1
		_delay_us(10);
		PORTA &=0xC0;//disable initialized PORTA pins
    	PORTC =(PORTC&0xF0)|(seconds/10);//display second 2
    	PORTA |= 0x10;//enable PA4 for second 2
    	_delay_us(10);
		PORTA &=0xC0;//disable initialized PORTA pins
    	PORTC =(PORTC&0xF0)|(minutes%10);//displaying minute 1
    	PORTA |=0x08;//enable PA3 for minute 1
    	_delay_us(10);
		PORTA &=0xC0;//disable initialized PORTA pins
    	PORTC =(PORTC&0xF0)|(minutes/10);//display minute 2
    	PORTA |=0x04;//enable PA2 for minute 2
    	_delay_us(10);
		PORTA &=0xC0;//disable initialized PORTA pins
    	PORTC =(PORTC&0xF0)|(hours%10);//display hour 1
    	PORTA |=0x02;//enable PA1 for hour 1
    	_delay_us(10);
		PORTA &=0xC0;//disable initialized PORTA pins
    	PORTC =(PORTC&0xF0)|(hours/10);//display hour 2
    	PORTA |=0x01;//enable PA0 for hour 2
    	_delay_us(10);
		PORTA &=0xC0;//disable initialized PORTA pins

	}
}

