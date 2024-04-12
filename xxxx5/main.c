/*
 * xxxx5.c
 *
 * Created: 12/04/2024 12:40:20
 * Author : asrol
 */ 


#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

uint8_t adc_value = 0;

void initADC(void);
void setup(void);
int main(void)
{
	
	setup();
	sei();
	while (1)
	{
		ADCSRA |= (1<<ADSC);
	}
}

void initADC(void){
	ADMUX = 0;
	// Selecciona ADC6 como entrada analógica (MUX2:0 = 0b110)
	ADMUX |= (1 << MUX2) | (1 << MUX1);
	// Configura referencia de voltaje AVCC con capacitor en AREF
	ADMUX |= (1 << REFS0);
	ADMUX &= ~(1<<REFS1);
	// Configura el ADC para operar en modo de 8 bits
	ADMUX |= (1 << ADLAR);
	ADCSRA = 0;
	// Habilita el ADC y configura la prescaler a 128 (Frecuencia de muestreo = F_CPU / 128)
	ADCSRA |= (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	ADCSRA |= (1 << ADEN); // habilitar interrupción
	
}
void setup(){
	cli();
	DDRB |= (1<<DDB1);	//salida
	TCCR1A = 0;
	TCCR1B = 0;
	
	TCCR1A |= (1<<COM1A1)| (1<<WGM10);				//OC1A modo fast no invertido
	TCCR1B |= (1<<WGM12);
	
	TCCR1B |= (1<<CS12)|(1<<CS10);		//1024 de preescaler
	//inicializacion de timer y adc
	initADC();
	
	sei();
}

/*ISR (ADC_vect){
	
	adc_value = ADCH;					//actualizar valor
	ADCSRA |= (1<<ADIF);			//apagar bandera 
}*/
ISR(ADC_vect) {
	adc_value = ADCH; // Actualizar valor del ADC
	OCR1A = (adc_value * 360) / 255; // Mapear el valor del ADC al rango de 0 a 180 grados
	ADCSRA |= (1<<ADIF);			//apagar bandera 
}