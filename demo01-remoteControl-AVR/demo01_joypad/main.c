/*
 * remoteControl01.c
 *
 * Created: 3/3/2021 22:54:58
 * Author : chanw
 */ 

enum keyState {
	wait,
	send
};

#include <avr/io.h>

#include "avr/interrupt.h"
// #include "function.c"

#define KEY_INPUT_PORT PORTD	// keyboard PORT
#define KEY_INPUT_DDR DDRD	// keyboard DDR
#define KEY_INPUT_PIN	PIND	// keyboard PIN

#define KEY_OUTPUT_PORT PORTB
#define KEY_OUTPUT_DDR DDRB
#define FREQ 16000000

enum keyState state = wait;

void usart_init (void)
{
	UCSR0B = (1 << TXEN0) |(1 << RXEN0) | (1 << UDRIE0);
	UCSR0C = 0x06;
	UBRR0 = 103;  // 115200,9
}

void delay_ms(unsigned int d) {
	OCR1A = d /1000 * FREQ /256;
	sei();
}

//unsigned char keypad[4][2] = {
	//'0', '4', 
	//'1', '5',
	//'2', '6',
	//'3', '7'
//};

char keypad[4][2] = {
	'4', '0',
	'5', '1',
	'6', '2',
	'7', '3'
};

unsigned char key_value;
unsigned char send_char;
unsigned char checkSW00 = 0;

int main(void)
{
	unsigned char colloc;	// column location
	unsigned char rowloc;	// row location
	usart_init();
	
	DDRB = 0x07;
	PORTB = 0;
	KEY_INPUT_DDR = 0;
	KEY_INPUT_PORT = 0X0C;
	
	OCR1A = 1250; // 20ms
	TCCR1A = 0;
	TCCR1B = (1 << WGM12)|(1 << CS12); //CTC mode, prescaler = 256
	TIMSK1 = (1<<OCIE1A);//enable Timer1 compare match
	
    /* Replace with your application code */
    while (1) 
    {
	    checkSW00 = 0;
		do {
			PORTB &= 0xF8;
			colloc = (KEY_INPUT_PIN & 0x0C);
		} while (colloc != 0x0C); // check until all keys released
		
		do {
			do 
			{
				delay_ms(20); // call delay
				colloc = (KEY_INPUT_PIN & 0x0C);
			} while (colloc == 0x0C);
				delay_ms(20); // call delay
			colloc = (KEY_INPUT_PIN & 0x0C);
			
		} while(colloc == 0x0C);
		
		while(1) {
			KEY_OUTPUT_PORT = 0x0E; // ground row 0
			colloc = (KEY_INPUT_PIN & 0x0C);
			
			if(colloc != 0x0C) {
				checkSW00++;
			}
			
			KEY_OUTPUT_PORT = 0x0D; // ground row 1
			colloc = (KEY_INPUT_PIN & 0x0C);
			
			if(colloc != 0x0C) {
				checkSW00++;
			}
			
			KEY_OUTPUT_PORT = 0x0B; // ground row 2
			colloc = (KEY_INPUT_PIN & 0x0C);
			if(colloc != 0x0C) {
				checkSW00++;
			}
			
			if (checkSW00 == 3) {
				rowloc = 3;
				break;
			}
			
			KEY_OUTPUT_PORT = 0x0E; // ground row 0
			colloc = (KEY_INPUT_PIN & 0x0C);
			
			if(colloc != 0x0C) {
				rowloc = 0;
				break;
			}
			
			KEY_OUTPUT_PORT = 0x0D; // ground row 1
			colloc = (KEY_INPUT_PIN & 0x0C);
			
			if(colloc != 0x0C) {
				rowloc = 1;
				break;
			}
			
			KEY_OUTPUT_PORT = 0x0B; // ground row 2
			colloc = (KEY_INPUT_PIN & 0x0C);
			if(colloc != 0x0C) {
				rowloc = 2;
				break;
			}
		}
		
		// check column and 
		if (colloc ==  0x04) {
			key_value = keypad[rowloc][0];
		}
		else if(colloc == 0x08) {
			key_value = keypad[rowloc][1];
		}
		
		send_char = key_value;
		state = send;
    }
	return 0;
}

ISR(USART_UDRE_vect) {
	//UDR0 = send_char;
	if ( state == send){
		UDR0 = send_char;
		state = wait;
	}
}

ISR(TIMER1_COMPA_vect) {
	cli();
}
