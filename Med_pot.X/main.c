/**
*	\file main.c
*	\brief Resumen del contenido del archivo
*	\details Descripcion detallada del archivo
*	\author Postemsky Marcos
*	\date 17-09-2016 11:39:08
*/
#include <18F4550.h>
#device HIGH_INTS = TRUE
#fuses NOWDT,MCLR,HS,NOUSBDIV,NOIESO,            //Selecciona el oscilador externo
#use delay(clock=48 Mhz, crystal= 48 MHz)   // Selecciona la velocidad del oscilador interno
#use i2c(Master,Fast=100000, sda=PIN_D6, scl=PIN_D7,force_sw)

#include <stdio.h>
#include <stdlib.h>

#include "funciones.h"
#use fast_io(A)
#use fast_io(B)

const long carga= 0xE877;
extern int contador;
extern long pulso_timer;

void inicializar();

void main()
{

//declaracion de variables
//inicializacion de perifericos

	inicializar() ;
    
    set_timer1(carga);
	while(1)
	{

		maquina_estado();

	}
}

void Inicializar(){
    setup_timer_1( T1_INTERNAL|T1_DIV_BY_2);
    setup_timer_0(T0_INTERNAL |T0_DIV_32);
    set_timer0(0x0000);
    set_tris_a(0xE8);// configuracion portA necesaria para la libreria control_ADC
    set_tris_b(0xFE);
    contador=0;

    lcd_init();
    lcd_gotoxy(1,1);
    printf(LCD_PUTC,"Primer prueba");
    lcd_backlight=ON;
    enable_interrupts (INT_TIMER1);
    enable_interrupts (INT_EXT1);
    //ext_int_edge(L_TO_H); //configura interrupcion RB0 control de pantalla
    enable_interrupts (GLOBAL);
    output_high (PIN_A0); //pone en alto el chipselect del AD7450
    output_high (PIN_A1); // pone en alto el chipselect del MAX186
}
