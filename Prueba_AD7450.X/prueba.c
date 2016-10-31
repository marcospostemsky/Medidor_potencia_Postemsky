/* 
 * File:   prueba.c
 * Author: MARCOS
 *
 * Created on 21 de octubre de 2016, 09:27
 */
#include <18F4550.h>
#use delay(clock=32 Mhz, crystal=32 MHz)   // Selecciona la velocidad del oscilador interno
#fuses NOWDT,MCLR,HS,NOUSBDIV,NOIESO,            //Selecciona el oscilador externo
#use i2c(Master,Fast=100000, sda=PIN_D6, scl=PIN_D7,force_sw)
#include <stdio.h>
#include <stdlib.h>
#include <i2c_Flex_LCD.h>
#include <math.h>


#use fast_io(A)
#define ADC_CS PIN_A2
#define ADC_SCLK PIN_A4
#define ADC_SDATA PIN_A3
signed long valor=0;
float enviar=0;


long leer_ADC(){
    signed long dato=0x00,dato_t;
    int i;
    output_low (ADC_CS);
    delay_us(1);
    output_low (ADC_SCLK);
    delay_us(1);
    output_high (ADC_SCLK);
    delay_us(1);
    
        for(i=0;i<15;i++){
          shift_left(&dato,2,input(ADC_SDATA));
          output_low(ADC_SCLK);
          delay_us(1); 
          output_high(ADC_SCLK); 
          delay_us(1);
        }

       // output_high (ADC_SCLK);
       // delay_us(1);
        output_high (ADC_CS);
        
       dato_t=(dato&0x0800);
        
      if (dato_t==0x0800){
            dato=-2048+(dato&0xF7FF);
        }
       
    return(dato);
}

int main() {
    set_tris_a(0x08);
    output_high(ADC_SCLK);
    output_high(ADC_CS);
    lcd_init();
    
    while(1){
    valor=leer_ADC();
    enviar=valor;
    enviar=enviar*194.45/2048;
    lcd_gotoxy(1,1);
    printf (LCD_PUTC, "Valor=\%f      ",enviar);
    delay_ms(800);
    }
    
}

