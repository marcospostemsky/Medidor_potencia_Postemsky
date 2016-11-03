/**
*	\file funciones.c
*	\brief 
*	\details Descripcion detallada del archivo
*	\author Postemsky Marcos
*	\date 17-09-2016 11:39:08
*/

#include <18F4550.h>
#fuses NOWDT,MCLR,HS,NOUSBDIV,NOIESO,            //Selecciona el oscilador externo
#use delay(clock=48 Mhz, crystal= 48 MHz)   // Selecciona la velocidad del oscilador interno
#use i2c(Master,Fast=100000, sda=PIN_D6, scl=PIN_D7,force_sw)
int contador = 0 ;int pulso_timer = 0 ;
#include <math.h>
#include "funciones.h"
#include <Control_ADCs.h>



//Implementacion Switch-Case

/**
*	\fn void maquina_estado()
*	\brief Implementacion Switch-Case
*	\details 
*	\author Postemsky Marcos
*	\date 17-09-2016 11:39:08
 **/
signed long punto1;
long punto2;
int16 tiempo_potencia;
BYTE pos_V,pos_I,pos_V_A,pos_I_A;
int control_V;
int control_I;
int desfase;
int puntos=20;//puntos por periodo
float tension, corriente, tension_RMS,corriente_RMS, t_desfase, potencia_ins,angulo;
float Energia_Wms=0, Energia_Wh=0, Energia_kWh=0;

const long carga= 0xE8AB;

#INT_TIMER1               // interrupcion para demora de 1 ms
void interrtimer_1(){
    set_timer1(carga);   // interrupcion cada 1 ms
    pulso_timer++;

   }

void maquina_estado()
{
		static int estado = PUNTO_TENS_CORR;

		switch(estado)
		{
			case PUNTO_TENS_CORR:
                punto1= leer_Tension();
                punto2= leer_Corriente(); // comprobar si funciona con el tiempo de demora de la lectura del externo
                // convierte los valores de long a float
                tension=punto1;
                corriente=punto2;
                contador++;
					estado = CONVERSION_DESFASE;

				break;
			
			case CONVERSION_DESFASE:
                tension= (tension)*2.5/2048;
                corriente= (corriente)/1000-2.510;
                //se convierte a la tension y corriente real
                tension= tension*155.57; // conversion con 2 V igual a 311.13 V
                corriente= corriente*12; // 2.5 V es igual a +30 A (recordar que el sensor mide ±30A)
                
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
                btfsc (&corriente + 0x01), 7; // comprueba signo en un bit de los 4 byte del float
                bsf &pos_I,0;
                #endasm

                
                
				
                // si el punto actual de tension es positivo, el anterior es negativo 
                //y se tiene mas de un punto (contador distinto de cero)
                if (pos_V==1 && contador>=2 && pos_V_A==0 && desfase!=1){ 
                 control_V= contador; // tiempo en el que cruzó la tension 
                 desfase=1;
                }
                
                // si el punto actual de tension es positivo, el anterior es negativo 
                //y se tiene mas de un punto (contaodr distinto de cero)
                if (pos_I==1 && contador>=2 && pos_I_A==0 && desfase!=2){
                    
                 control_I= contador; // tiempo en el que cruzó la corriente
                 desfase=2;
                }
                // guarda el signo de los puntos anteriores
                pos_V_A=pos_V;
                pos_I_A=pos_I;
                
                
					estado = TENS_CORR_RMS;
		
				

				break;
//----------------------------------------------------------------------------------------------------
                
			case TENS_CORR_RMS:
			
				if((contador<30)&& (pulso_timer==1))
				{
					tension_RMS=tension_RMS+ tension * tension;       //calcula tension eficaz
                    corriente_RMS= corriente_RMS+ corriente * corriente; // calcula corriente eficaz
                    pulso_timer=0;
                    
					estado = PUNTO_TENS_CORR;
                    
                    if((contador== 29)){
					disable_interrupts(INT_TIMER1);// deshabilita la interrupcion para no entrar al timer
                    contador=0; //se reinicia el contador, para comenzar nuevamente 
					estado = CALCULO_POT_ENER;
				}
				}
				

				break;
                
//--------------------------------------------------------------------------------------------------------------------                
			
			case CALCULO_POT_ENER: // falta calculo de energia
                //calcula las raices para completar el calculo RMS
				tension_RMS= sqrt(tension_RMS/30);
                corriente_RMS= sqrt(corriente_RMS/30);
                
                // se controla si se pudo calcular desfase en el estado anterior 
                // se realiza las diferencia de cruce por cero y se convierte de tiempo a radianes
                angulo=0;
                if (desfase==2){
                    t_desfase= (control_I-control_V);
                    angulo= (t_desfase*pi)/10;      // angulo de desfase en radianes. 20 puntos por periodo
                   /*  lcd_gotoxy(1,1);
                     printf(LCD_PUTC,"desfase= %f       ",t_desfase);
                     lcd_gotoxy(1,2);
                     printf(LCD_PUTC,"angulo=%f        ",angulo);
                     delay_ms(1000);*/
                }                
                
                angulo=cos(angulo);
               // calculo de potencia 
                potencia_ins= tension_RMS*corriente_RMS*angulo;
                //se limpian las variables para la próxima etapa de muestreo
                control_V=0;      
                control_I=0;
                angulo=0;
                desfase=0; 
                
                //Calculo de Energia
                tiempo_potencia=65536 - get_timer0();    //variable para calcular energia segun tiempo de potencia
                Energia_Wms=Energia_Wms+potencia_ins*tiempo_potencia/37500000;             //Energia en Watt por milisegundo
                
                if(Energia_Wms>=3600000){
                    Energia_Wh=Energia_Wh+1;  // rekacion watt ms a watt hora
                    Energia_Wms=0;                
                }
                
                if (Energia_Wh==1000){
                    Energia_kWh=Energia_kWh+1;
                    Energia_Wh=0;
                }
                
                
                
                // Se reinicia timer 0 nuevamente, para volver a calcular energia
                set_timer0(0x0000);
                
					estado = MOSTRAR_DATOS;
				break;
                
//-------------------------------------------------------------------------------------
                
			case MOSTRAR_DATOS:
                //este estado solo muestra los datos en la pantalla LCD
                lcd_gotoxy(1,1);
                printf(LCD_PUTC,"pot= \%f W",potencia_ins);
                lcd_gotoxy(1,2);
                printf (LCD_PUTC, "T=\%f  I=\%f  ",tension_RMS,corriente_RMS);
                delay_ms(1000);
                enable_interrupts(INT_TIMER1);
                enable_interrupts(GLOBAL);
                pulso_timer=0;
                set_timer1(carga);   
				//reinicia todo
                corriente_RMS=0;
                tension_RMS=0;
                
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


