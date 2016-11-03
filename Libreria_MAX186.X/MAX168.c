#include <18F4550.h>

#fuses NOWDT,HS,NOUSBDIV            //Selecciona el oscilador externo
#use delay(clock=48 Mhz, crystal= 48 MHz)   // Selecciona la velocidad del oscilador interno
#use i2c(Master,Fast=100000, sda=PIN_D6, scl=PIN_D7,force_sw)
#include <stdio.h>
#include <stdlib.h>
#include <i2c_Flex_LCD.h>
#include <math.h>
#include <Control_ADCs.h>

#use fast_io(A)
#use fast_io(B)



/*long leer_ADC(){
    long dato;
    BYTE config= 0b10001110;
    int i;
    output_low (ADC_CS);
    output_low (ADC_DIN);
    delay_us(1);
    
    for(i=0; i<8; i++) {
      output_bit(ADC_DIN, shift_left(&config,1,0));
      output_high(ADC_SCLK);
      delay_us(1);
      output_low(ADC_SCLK);	
      delay_us(1);
   }

    output_high(ADC_CS);
    
    while(input(ADC_SSTRB)=='0');   
        
        output_low (ADC_CS);
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
 
        
        output_high (ADC_CS);
    

    dato= (dato>>3)& 0x0FFF;
    return(dato);
}
*/

int main() {
    signed long valor; 
    float enviar;
    set_tris_a(0xE8);
    set_tris_a(0xFF);
    output_low(ADC_SCLK);
    output_high(ADC_CSCorriente);
    lcd_init();
    
    while(1){    
    valor=leer_Tension();
    enviar=valor;
    //enviar=enviar*2.5/2048;;
    //enviar=enviar*155.57;
    lcd_gotoxy(1,1);
    printf (LCD_PUTC, "Tens=\%f V     ",enviar);
    delay_ms(150);
    }
}