/* 
 * File:   MAX191.h
 * Author: MARCOS
 *
 * Created on 23 de septiembre de 2016, 08:14
 */

#ifndef Control_ADCs_H
#define Control_ADCs_H

#define ADC_CSCorriente PIN_A0
#define ADC_CSTension PIN_A1
#define ADC_SCLK PIN_A2
#define ADC_DOUT PIN_A3  // por aqui ingresa la info de los ADC, SDATA en AD7450
#define ADC_DIN PIN_A4 // se envia la informacion al MAX186
#define ADC_SSTRB PIN_A5
#define ADC_SDATA PIN_A6

//control AD7450
long leer_Tension(){
    signed long dato=0x00,dato_t;
    int i;
    output_low (ADC_CSTension);
    delay_us(1);
    output_low (ADC_SCLK);
    delay_us(1);
    output_high (ADC_SCLK);
    delay_us(1);
    
        for(i=0;i<15;i++){
          shift_left(&dato,2,input(ADC_DOUT));
          output_low(ADC_SCLK);
          delay_us(1); 
          output_high(ADC_SCLK); 
          delay_us(1);
        }

       // output_high (ADC_SCLK);
       // delay_us(1);
        output_high (ADC_CSTension);
        
       dato_t=(dato&0x0800);
        
      if (dato_t==0x0800){
            dato=-2048+(dato&0xF7FF);
        }
       
    return(dato);
}

//control MAX186
long leer_Corriente(){
    long dato;
    BYTE config= 0b10001110;
    int i;
    output_low (ADC_CSCorriente);
    output_low (ADC_DIN);
    delay_us(1);
    
    for(i=0; i<8; i++) {
      output_bit(ADC_DIN, shift_left(&config,1,0));
      output_high(ADC_SCLK);
      delay_us(1);
      output_low(ADC_SCLK);	
      delay_us(1);
   }

    output_high(ADC_CSCorriente);
    
    while(input(ADC_SSTRB)=='0');   
        
        output_low (ADC_CSCorriente);
        output_high (ADC_SCLK);
        delay_us(1);
        output_low (ADC_SCLK);
        delay_us(1);
        
        for(i=0;i<15;i++){
          output_high(ADC_SCLK);
          delay_us(1);
          shift_left(&dato,2,input(ADC_DOUT)); 
          output_low(ADC_SCLK);
          delay_us(1);
        }
 
        
        output_high (ADC_CSCorriente);
    

    dato= (dato>>3)& 0x0FFF;
    return(dato);
}


#endif

