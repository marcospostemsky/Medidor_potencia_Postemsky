/**
*	\file funciones.c
*	\brief 
*	\details Descripcion detallada del archivo
*	\author Postemsky Marcos
*	\date 17-09-2016 11:39:08
*/

#include <18F4550.h>
#use delay(clock=12 Mhz, crystal= 12 MHz)   // Selecciona la velocidad del oscilador interno
#use i2c(Master,Fast=100000, sda=PIN_D6, scl=PIN_D7,force_sw)
int contador = 0 ;int pulso_timer = 0 ;
#include "funciones.h" 



//Implementacion Switch-Case

/**
*	\fn void maquina_estado()
*	\brief Implementacion Switch-Case
*	\details 
*	\author Postemsky Marcos
*	\date 17-09-2016 11:39:08
 **/
unsigned long punto1, punto2;
int8 pos_V,pos_I,pos_V_A,pos_I_A;
int control_V, control_I;
int desfase;
float tension, corriente, tension_RMS,corriente_RMS, t_desfase, potencia_ins,angulo;


#INT_RTCC                // interrupcion para demora de 500 us
void interrtimer_0(){
    set_timer0(0x1D);   // se carga 29 para un desborde en 500 us teniendo en cuenta los tiempos en C
    pulso_timer++;
   }

void maquina_estado()
{
		static int estado = PUNTO_TENS_CORR;

		switch(estado)
		{
			case PUNTO_TENS_CORR:
                set_adc_channel(0);           //Habilitación canal
                punto1= leer_ADC(2);
                punto2= read_adc(); // comprobar si funciona con el tiempo de demora de la lectura del externo
                // convierte los valores de long a float
                tension=punto1;
                corriente=punto2;
                contador++;
					estado = CONVERSION_DESFASE;
		
				

				break;
			
			case CONVERSION_DESFASE:
                tension= (tension*5)/4096;
                corriente= (corriente*5)/1024;
                
                // Analisis del punto POSITVO O NEGATIVO
                // valor positivo estado=0-- valor negativo estado=1
                
                //TENSION
                #asm
                clrf &pos_V;
                btfsc (&tension + 0x01), 7;
                bsf &pos_V,0;
                #endasm
				
                //CORRIENTE
                #asm
                clrf &pos_I;
                btfsc (&corriente + 0x01), 7;
                bsf &pos_I,0;
                #endasm
					
                if (pos_V==1 && contador!=0 && pos_V_A==0 && control_V != 0){
                 control_V= contador; // tiempo en el que cruzó la tension 
                 desfase=1;
                }
                
                if (pos_I==1 && contador!=0 && pos_I_A==0 && control_I != 0){
                    
                 control_I= contador; // tiempo en el que cruzó la corriente
                 desfase=2;
                }
                
                pos_V_A=pos_V;
                pos_I_A=pos_I;
                
                
					estado = TENS_CORR_RMS;
		
				

				break;
			
			case TENS_CORR_RMS:
			
				if((contador<60)&& (pulso_timer==1))
				{
					tension_RMS=tension_RMS+ tension * tension;       //calcula tension eficaz
                    corriente_RMS= corriente_RMS+ corriente * corriente; // calcula corriente eficaz
                    pulso_timer=0;
                    
					estado = PUNTO_TENS_CORR;
		
				}
				if((contador==60))
				{
					disable_interrupts(INT_RTCC);
					estado = CALCULO_POT_ENER;
		
				}

				break;
			
			case CALCULO_POT_ENER:
			
				tension_RMS= SQRT(tension_RMS/60);
                corriente_RMS= SQRT(corriente_RMS/60);
                
                if (desfase==2){
                    t_desfase= (control_I-control_V);
                    angulo= (t_desfase*pi)/20;      // angulo de desfase en radianes.
                }                
               
                potencia_ins= tension_RMS*corriente_RMS* cos(angulo);
                //se limpian las variables para la próxima tanta de muestreo
                control_V=0;      
                control_I=0;
                angulo=0;
                
					estado = MOSTRAR_DATOS;
				break;
			
			case MOSTRAR_DATOS:
                lcd_gotoxy(1,1);
                printf(LCD_PUTC,"Potencia= \%f W",potencia_ins);
                lcd_gotoxy(1,2);
                printf (LCD_PUTC, "T=\%f V  I=\%f A",tension_RMS,corriente_RMS);
                delay_ms(750);
                enable_interrupts(INT_RTCC);
                set_timer0(0x1D);   // se carga 29 para un desborde en 500 us teniendo en cuenta los tiempos en C
					
					estado = PUNTO_TENS_CORR;
		

				break;
			
			default: estado = PUNTO_TENS_CORR;
		}


}

//Funciones asociadas a los eventos

/**
*	\fn int -(void)
*	\brief Resumen
*	\details Detalles
*	\author Postemsky Marcos
*	\date 17-09-2016 11:39:08
*/


