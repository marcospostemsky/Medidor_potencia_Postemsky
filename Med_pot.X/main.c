/**
*	\file main.c
*	\brief Resumen del contenido del archivo
*	\details Descripcion detallada del archivo
*	\author Postemsky Marcos
*	\date 17-09-2016 11:39:08
*/
#include <18F4550.h>

#fuses NOWDT,MCLR,HS,NOUSBDIV,NOIESO,            //Selecciona el oscilador externo
#use delay(clock=12 Mhz, crystal= 12 MHz)   // Selecciona la velocidad del oscilador interno
#use i2c(Master,Fast=100000, sda=PIN_D6, scl=PIN_D7,force_sw)

#include <stdio.h>
#include <stdlib.h>

#include "funciones.h"
#use fast_io(A)
#use fast_io(B)

const long carga= 0xF485;
extern int contador;
extern long pulso_timer;

void inicializar();

void main()
{

//declaracion de variables
//inicializacion de perifericos

	inicializar() ;
    
    set_timer0(carga);
	while(1)
	{

		maquina_estado();

	}
}

void Inicializar(){

    setup_adc_ports(AN0);                                   //Canal 0 analógico
    setup_adc(ADC_CLOCK_DIV_16);   
    setup_timer_0( RTCC_INTERNAL | T0_DIV_2);
    set_tris_a(0xF9);
    contador=1;
    lcd_init();
    lcd_gotoxy(1,1);
    printf(LCD_PUTC,"Primer prueba");
    lcd_backlight=ON;
    enable_interrupts (INT_RTCC);
    enable_interrupts (GLOBAL);
    output_high (PIN_A0); //pone en alto el chipselect del AD7450
    output_high (PIN_A1); // pone en alto el chipselect del MAX186
}
