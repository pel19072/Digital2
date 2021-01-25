/*
 * File:   Code.c
 * Author: Ricardo
 *
 * Created on January 18, 2021, 2:23 PM
 */

//******************************************************************************
//LIBRERIAS
//******************************************************************************

#include <xc.h>
#include <stdint.h>
#include "Oscilador.h"
//******************************************************************************
//CONFIGURACION
//******************************************************************************

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


//******************************************************************************
//VARIABLES
//******************************************************************************

#define  _XTAL_FREQ 8000000 //Necesario para la funci�n de Delay


//******************************************************************************
//INSTANCIACION DE FUNCIONES
//******************************************************************************
void Setup(void);
//******************************************************************************
//MAIN LOOP
//******************************************************************************

void main(void) {
    Setup();
    while(1){
                
    }
}

//******************************************************************************
//SETUP
//******************************************************************************

void Setup(void) {
    PORTA = 0;  //LIMPIO PUERTOS
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    
    ANSEL = 0;  //TODO DIGITAL
    ANSELH = 0;
    
    TRISA = 0; 
    TRISB = 0b00000111; //TRES BOTONES --> TRES INPUTS
    TRISC = 0;  //TODOS OUTPUTS --> JUGADOR 1
    TRISD = 0;  //TODOS OUTPUTS --> JUGADOR 2
    TRISE = 0;  //TODOS OUTPUTS --> SEMAFORO
    
    WPUB = 0b00000111;  //CONFIGURACION PULL UPS INTERNOS
    OPTION_REG = 0b01010111;
    
    initOsc(6);
}

//******************************************************************************
//SUBRUTINAS
//******************************************************************************

