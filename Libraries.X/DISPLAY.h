/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef DISPLAY_H
#define	DISPLAY_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

void display_anodo(uint8_t VAR, uint8_t PORT);

#endif	/* DISPLAY_H */

