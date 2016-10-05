/**
*	\file funciones.h
*	\brief Resumen del archivo
*	\details Descripcion detallada del archivo
*	\author Postemsky Marcos
*	\date 17-09-2016 11:39:08
*/

#include <i2c_Flex_LCD.h>


#ifndef FUNCIONES_H
#define FUNCIONES_H
//Declaracion de estados

#define 	PUNTO_TENS_CORR	0
#define 	CONVERSION_DESFASE	1
#define 	TENS_CORR_RMS	2
#define 	CALCULO_POT_ENER	3
#define 	MOSTRAR_DATOS	4

//Prototipos de los eventos
void maquina_estado(void);

#endif