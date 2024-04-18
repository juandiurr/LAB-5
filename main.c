/*
 * LAB 5.c
 *
 * Created: 14/04/2024 13:59:00
 * Author : diego
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Librerias/ADC.h"
#include "Librerias/PWM1.h"
#include "Librerias/TIMERS.h"
uint8_t hola = 0;
uint8_t toggle_led = 0;
uint8_t lol = 0;

void servo1(void);
void servo2(void);
void led(void);

int main(void)
{
	cli();
	CLKPR |= (1<<CLKPCE);
	CLKPR |= (1<<CLKPS0) | (1<<CLKPS1);//FRECUENCIA 2M
	DDRD |= (1<<PORTD6);
	PORTD &= ~(1<<PORTD6); //PONE EN 0 PD6 (LED)
	ADC_(32);
	timer0(1024);
	TCNT0 = 100; //valor de comienzo
	sei();
    while (1) 
    {
		servo1();
		servo2();
		led();
    }
}
void servo1(void){
	ADC1(); //servo 1
	habilitar_conversion();
	salidaPWM1(1, 1024, 38);
	update_duty_cycle1(1, hola*1.9/255+1.9);
	_delay_ms(5);
}
void servo2(void){
	ADC2(); //servo 1
	habilitar_conversion();
	salidaPWM2(1, 256);
	update_duty_cycle2(1, hola);
	_delay_ms(5);
}
void led(void){
	ADC3(); //servo 1
	habilitar_conversion();
	lol = hola;
	_delay_ms(5);
}

ISR(TIMER0_OVF_vect){
	//TCNT0 = lol/3; 
	TIFR0 |= (1<<TOV0); //apagar bandera
	if (toggle_led == 0){
		PORTD |= (1<<PORTD6);
		toggle_led = 1;
		TCNT0 = 255 - lol;
	}else if(toggle_led == 1){
		PORTD &= ~(1<<PORTD6);
		toggle_led = 0;
		TCNT0 = lol;
	}
}
ISR(ADC_vect){
	hola = ADCH;
	ADCSRA |= (1<<ADIF); //apagar bandera
}