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
#include <stdint.h> //LIBRERIA CON INTS DE 8 BITS ENTRE OTRAS COSAS
#include "Oscilador.h"  //CONFIGURACION DEL OSCILADOR
#include "MSSP.h"
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
uint8_t FLAGD = 0;
uint8_t FLAGI = 0;
uint8_t TOGGLE = 0;
uint8_t CONTADOR = 0;
uint8_t ESCRITURA_REVISION = 0;

//******************************************************************************
//INSTANCIACION DE FUNCIONES
//******************************************************************************
void Setup(void);

//******************************************************************************
//ISR
//******************************************************************************
void __interrupt() isr(void) { 
    di();   //DESACTIVA LAS INTERRUPCIONES
    //INTERRUPT ON CHANGE
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
    //INTERUPCION DE SPI
    if (PIR1bits.SSPIF == 1) {
        spiWrite(PORTD);
        ESCRITURA_REVISION = spiRead();
        PIR1bits.SSPIF = 0;
    }
    ei(); //VUELVE A HABILITAR LAS INTERRUPCIONES
}

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
    //GLOBAL
    PORTA = 0;  //LIMPIO PUERTOS
    PORTB = 0;  //BOTONES PARA INTERRUPT ON CHANGE
    PORTC = 0;  //DISPLAYS
    PORTD = 0;  //CONTADOR
    PORTE = 0;  //TRANSISTORES Y ALARMA
    
    ANSEL = 0;
    ANSELH = 0;
    
    TRISA = 0b00100000;  //UN INPUT --> POTENCIOMETRO
    TRISB = 0b00000011; //DOS BOTONES --> DOS INPUTS
    TRISC = 0b00011000;  //TODOS OUTPUTS --> DISPLAY
    TRISD = 0;  //TODOS OUTPUTS --> CONTADOR
    TRISE = 0;  //TODOS OUTPUTS --> ALARMA Y TRANSISTORES
    
    WPUB = 0b00000011;  //CONFIGURACION PULL UPS INTERNOS
    OPTION_REG = 0b01010111;
    
    //OSCILADOR
    initOsc(6);   
    //INTERRUPT ON CHANGE
    IOCB = 0b00000011;  //RB0 Y RB1  
    //MSSP
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
 
    //INTERRUPCIONES
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    PIE1bits.SSPIE = 1;       
}

//******************************************************************************
//SUBRUTINAS
//******************************************************************************
