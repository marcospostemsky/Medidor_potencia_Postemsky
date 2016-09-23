/* 
 * File:   MAX191.h
 * Author: MARCOS
 *
 * Created on 23 de septiembre de 2016, 08:14
 */

#ifndef MAX192_H
#define MAX192_H

#define ADC_CS1 PIN_A1
#define ADC_SCLK PIN_A2
#define ADC_DOUT PIN_A3

unsigned long leer_ADC(ADC){
    unsigned long dato;
    int i;
    output_low (ADC_SCLK);
    output_low (ADC_CS1);
    
    
    if (ADC==2){
        
        output_low (ADC_CS1); // modo CPOL = 0, CPHA = 0
        delay_us(1);
        
        for(i=0;i<16;i++){
          output_high(ADC_SCLK);
          shift_left(&dato,2,input(ADC_DOUT)); 
          output_low(ADC_SCLK); 
        }
 
        output_low (ADC_SCLK);
        output_high (ADC_CS1);
    }

    dato= (dato>>2)& 0x0FFF;
    return(dato);
}
#endif

