/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ADC_H
#define	ADC_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

#define  _XTAL_FREQ 4000000 //Necesario para la función de Delay

void initADC(uint8_t ADCS,uint8_t CHS);

#endif	/* ADC_H */