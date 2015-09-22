/*
 * uart.c
 *
 *  Created on: Jun 30, 2015
 *      Author: marko
 */

#include "uart.h"
#include "util.h"
#include "gpio.h"
#include "stdlib.h"
#include <string.h>
#include <avr/interrupt.h>

static volatile char uart0_buff[UART0_SIZE];
static volatile char uart0_sent;
static volatile char uart0_received;
static volatile uint8_t uart0_index;

/*	Inicijalizacija uarta	*/
void init_uart(uint8_t baud)
{
	// Inicijalizacija promenljivih
	uart0_sent = 0;
	uart0_received = 0;
	uart0_index = 0;
	uart0_buff[uart0_index] = '\0';

	// Postavljanje smera pinovima
	SBIT(DDRD, 1);
	CBIT(DDRD, 0);

	// Inicijalizacija registara za komunikaciju
	UCSR0B |= (1 << RXEN0 | 1 << TXEN0 | 1 << RXCIE0);
	UCSR0C |= (1 << UCSZ00 | 1 << UCSZ00);
	UBRR0 = baud;
	
	// Globalna dozvola prekida
	sei();
}

/*	Slanje jednog bajta	*/
void send_char(char c)
{
	uart0_sent = 0;		// Resetovanje promenljive
	UDR0 = c;			// Upis bajta u registar
	while (!(UCSR0A & (1 << UDRE0)));	// Cekanje potvrde da je bajt poslat
}

/*	Slanje broja velicine 1 bajt	*/
void send_one_char(char a)
{
	char temp_answer [4];	// Cuva string koji ce biti poslat
	char temp_str[4];		// Cuva string koji oznacava vrednost bajta
	
	temp_answer[0] = '\0';	// Inicijalizacija
	
	itoa(a, temp_str, 10);	// Pretvaranje iz broja u string
	strcat(temp_answer, temp_str);	// Kopiranje broja (string-a)
	strcat(temp_answer, "\r\n");	// Finisiranje stringa

	send_string((char*) temp_answer);	// Slanje stringa
}

/*	Slanje stringa	*/
void send_string(char *str)
{
	while (*str != '\0') {	// Slanje jednog po jednog bajta dok se ne dodje do terminatora (nula)
		send_char(*str);
		str++;
	}
}

/*	Kopiranje uart bafera	*/
void get_buffer(char *dest, uint8_t size)
{
	int i = 0;
	while(i < size && uart0_buff[i] != '\0') {	// Kopiranje bafera dok se ne dodje do kraja string-a
		(*dest) = uart0_buff[i];
		dest++;
		i++;
	}
	*dest = '\0';	// Zatvaranje stringa terminatorom
}

/*	Ciscenje uart bafera	*/
void clear_buffer(void)
{
	uart0_index = 0;
	uart0_buff[uart0_index] = '\0';
}

/*	Ciscenje uart bafera	*/
void analyze_uart(void)
{
	if (uart0_received) {
		char temp[256];

		uart0_received = 0;
		get_buffer(temp, uart0_index);
		interpret_uart(temp);	//Korisnicka funkcija u kojoj se vrsi obrada dobijene komande
		clear_buffer();
	}
}

/*	Obrada pristigle komande	*/
/*	Napomena: ovde uneti korisnicki kod	*/
void interpret_uart(char *temp)
{
	char arg[10];
	char val[5];
	uint8_t i;
	
	i = 0;
	while (*temp != '=') {
		arg[i] = *temp;
		i++;
		temp++;
	}
	arg[i] = '\0';
	
	temp++;
	i = 0;
	while (*temp != '\r') {
		val[i] = *temp;
		i++;
		temp++;
	}
	val[i] = '\0';
	
	if (!strcmp(arg, "AT&D")) {
		if (!strcmp(val, "1")) {
			set_pin(&PORTC, 0);
			clear_pin(&PORTC, 1);
			clear_pin(&PORTC, 3);
		} else if (!strcmp(val, "2")) {
			clear_pin(&PORTC, 0);
			set_pin(&PORTC, 1);
			clear_pin(&PORTC, 3);
		} else if (!strcmp(val, "3")) {
			clear_pin(&PORTC, 0);
			clear_pin(&PORTC, 1);
			set_pin(&PORTC, 3);			
		} else if (!strcmp(val, "ALL")) {
			set_pin(&PORTC, 0);
			set_pin(&PORTC, 1);
			set_pin(&PORTC, 3);
		} else if (!strcmp(val, "0")) {
			clear_pin(&PORTC, 0);
			clear_pin(&PORTC, 1);
			clear_pin(&PORTC, 3);
		} else {
			send_string("Neispravan parametar\n");		
		}
	}  else {
		send_string("Komanda nije pronadjena\n");
	}
}

//ISR(USART_TX_vect)
//{
//	uart0_sent = 1;
//}

/*	Prekidna rutina za obradu primljenog bajta	*/
ISR(USART_RX_vect)
{
	char temp;

	temp = UDR0;
	if (uart0_index < (UART0_SIZE - 2)) {
		/* There is room for more characters. */
		uart0_buff[uart0_index] = temp;
		uart0_buff[uart0_index + 1] = '\0';
		uart0_index++;
		if (temp == '\0' || temp == '\n' || temp == '\r') {	// Ovi znakovi oznacavaju kraj string-a
			uart0_received = 1;	// Stigao je podatak -> setuj fleg za interpretaciju
		}
	} else {
		clear_buffer();	// Ciscenje bafera ako je prepunjen
	}
}
