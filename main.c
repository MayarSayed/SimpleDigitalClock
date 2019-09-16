/*
 * main.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: Mayar Sayed
 */


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned char g_tick = 0;

// flag will be set when the timer count 1 second
unsigned char count_second_flag = 0;

// variables to hold the clock time
unsigned char seconds_count = 0;
unsigned char minutes_count = 0;
unsigned char hours_count = 0;

// Interrupt Service Routine for timer1 compare mode channel A
ISR(TIMER1_COMPA_vect)
{
	count_second_flag = 1;
}


void timer1_init_CTC_mode(unsigned short countvalue)
{
	TCNT1 = 0; //timer initial value
	OCR1A  = countvalue; //compare value
	TIMSK |= (1<<OCIE1A); //enable compare interrupt for channel A

	TCCR1A = (1<<FOC1A) | (1<<FOC1B);

	TCCR1B = (1<<WGM12) | (1<<CS10) | (1<<CS12);
}

int main(void)
{
	DDRC |= 0x0F;
	DDRD |= 0X3F;

	PORTD |= 0x3F;
	PORTC &= 0xF0;


	SREG  |= (1<<7);

	timer1_init_CTC_mode(1000);
	while(1)
	{
		if(count_second_flag == 1)
		    	{

		    		seconds_count++;

		    		if(seconds_count == 60)
		    		{
		    			seconds_count = 0;
		    			minutes_count++;
		    		}
		    		if(minutes_count == 60)
		    		{
		    			minutes_count = 0;
		    			hours_count++;
		    		}
		    		if(hours_count == 24)
		    		{
		    			hours_count = 0;
		    		}


		    	    count_second_flag = 0;
		    	}
		    	else
		    	{
		    		// out the number of seconds
		    		PORTD = (PORTD & 0xC0) | 0x01;
		    		PORTC = (PORTC & 0xF0) | (seconds_count%10);

		    		// make small delay to see the changes in the 7-segment
		    		// 2Miliseconds delay will not effect the seconds count
		    		_delay_ms(2);

		    		PORTD = (PORTD & 0xC0) | 0x02;
		    		PORTC = (PORTC & 0xF0) | (seconds_count/10);

		    		_delay_ms(2);

		    		// out the number of minutes
		    		PORTD = (PORTD & 0xC0) | 0x04;
		    		PORTC = (PORTC & 0xF0) | (minutes_count%10);

		    		_delay_ms(2);

		    		PORTD = (PORTD & 0xC0) | 0x08;
		    	    PORTC = (PORTC & 0xF0) | (minutes_count/10);

		    	    _delay_ms(2);


		    	    PORTA = (PORTA & 0xC0) | 0x10;
		    	    PORTC = (PORTC & 0xF0) | (hours_count%10);

		    	    _delay_ms(2);

		    	    PORTA = (PORTA & 0xC0) | 0x20;
		    	    PORTC = (PORTC & 0xF0) | (hours_count/10);

		    	    _delay_ms(2);
		    	}
		   }

}


