/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef DISPLAY7SEG_H
#define	DISPLAY7SEG_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

void display_anodo(uint8_t VAR, uint8_t *PORT);

#endif	/* DISPLAY7SEG_H */