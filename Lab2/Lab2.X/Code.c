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
#include "ADC.h"
#include "DISPLAY.h"
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

#define  _XTAL_FREQ 4000000 //Necesario para la función de Delay
uint8_t VAR_ADC = 0;
uint8_t DISPLAY_HIGH = 0;
uint8_t DISPLAY_LOW = 0;
uint8_t FLAGD = 0;
uint8_t FLAGI = 0;
uint8_t TOGGLE = 0;
uint8_t contador = 0;

//******************************************************************************
//INSTANCIACION DE FUNCIONES
//******************************************************************************
void Setup(void);
void mux(void);
void alarma(void);

//******************************************************************************
//ISR
//******************************************************************************
void __interrupt() isr(void) { 
    di();
    if(INTCONbits.RBIF == 1){
        if(PORTBbits.RB0 == 0){
            FLAGI = 1;
        }
        else{
            if(FLAGI == 1){
                FLAGI = 0;
                PORTD++;
            }
        }
        if(PORTBbits.RB1 == 0){
            FLAGD = 1;
        }
        else{
            if(FLAGD == 1){
                FLAGD = 0;
                PORTD--;
            }
        }
        
        INTCONbits.RBIF = 0;
    }    
    if(PIR1bits.ADIF == 1){
        VAR_ADC = ADRESH;
        PIR1bits.ADIF = 0;
        //ADCON0bits.GO_nDONE = 1;
    }
    if(INTCONbits.T0IF == 1){
        TMR0 = 236; //248 PARA 2ms | 236 PARA 5ms
        TOGGLE = ~TOGGLE;
        contador++;
        INTCONbits.T0IF = 0;
    }
    ei();
}

//******************************************************************************
//MAIN LOOP
//******************************************************************************
void main(void) {
    Setup();    
    while(1){ 
        if(contador>10){
            ADCON0bits.GO_nDONE = 1;
            contador = 0;
        }
        mux();
        alarma();
    }
}

//******************************************************************************
//SETUP
//******************************************************************************
void Setup(void) {
    //GLOBAL
    PORTA = 0;  //LIMPIO PUERTOS
    PORTB = 0;  //BOTONES PARA INTERRUPT ON CHANGE
    PORTC = 0;  //DISPLAYS
    PORTD = 0;  //CONTADOR
    PORTE = 0;  //TRANSISTORES Y ALARMA
    
    ANSEL = 0;
    ANSELbits.ANS0 = 1;  //POT EN A0
    ANSELH = 0;
    
    TRISA = 0b00000001;  //UN INPUT --> POTENCIOMETRO
    TRISB = 0b00000011; //DOS BOTONES --> DOS INPUTS
    TRISC = 0;  //TODOS OUTPUTS --> DISPLAY
    TRISD = 0;  //TODOS OUTPUTS --> CONTADOR
    TRISE = 0;  //TODOS OUTPUTS --> ALARMA Y TRANSISTORES
    
    WPUB = 0b00000011;  //CONFIGURACION PULL UPS INTERNOS
    OPTION_REG = 0b01010111;
    
    //OSCILADOR
    initOsc(6);    
    //ADC
    initADC(2,0);
    
    //INTERRUPCIONES
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.T0IE = 1;
    INTCONbits.T0IF = 0;
    PIE1bits.ADIE = 1;
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    
    //INTERRUPT ON CHANGE
    IOCB = 0b00000011;  //RB0 Y RB1
    //TIMER 0
    WDTCON = 0;
    OPTION_REG = 0b01010111;
    
}

//******************************************************************************
//SUBRUTINAS
//******************************************************************************

void mux(void){
    PORTEbits.RE0 = 0;
    PORTEbits.RE1 = 0;
    if(TOGGLE == 0){
        DISPLAY_LOW = VAR_ADC & 0b00001111;
        display_anodo(DISPLAY_LOW);
        PORTEbits.RE0 = 1;
    }
    else{
        DISPLAY_HIGH = VAR_ADC & 0b11110000;
        DISPLAY_HIGH = DISPLAY_HIGH>>4;
        display_anodo(DISPLAY_HIGH);
        PORTEbits.RE1 = 1;
    }
}

void alarma(void){
    if(VAR_ADC >= PORTD){
        PORTEbits.RE2 = 1;
    }
    else{
        PORTEbits.RE2 = 0;
    }
}