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
#include <stdio.h>
#include "Oscilador.h"
#include "MSSP.h"
#include "ADC.h"

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
uint8_t CONTADOR = 0;

//******************************************************************************
//INSTANCIACION DE FUNCIONES
//******************************************************************************
void Setup(void);

//******************************************************************************
//ISR
//******************************************************************************

void __interrupt() isr(void) {
    di(); //DESACTIVA LAS INTERRUPCIONES
    //INTERUPCION DEL TIMER0
    if(INTCONbits.T0IF == 1){
        TMR0 = 236; //248 PARA 2ms | 236 PARA 5ms
        CONTADOR++; //CONTADOR PARA HABILITAR LA LECTURA DEL ADC
        INTCONbits.T0IF = 0;
    }
    //INTERUPCION DE SPI
    if (PIR1bits.SSPIF == 1) {
        spiWrite(PORTB);
        PIR1bits.SSPIF = 0;
    }
    //INTERUPCION DEL ADC
    if(PIR1bits.ADIF == 1){
        PORTB = ADRESH;    
        PIR1bits.ADIF = 0;
    }
    ei(); //VUELVE A HABILITAR LAS INTERRUPCIONES
}

//******************************************************************************
//MAIN LOOP
//******************************************************************************

void main(void) {
    Setup();
    
    while (1) {
        //SIRVE PARA DARLE UN TIEMPO A LA LECTURA DEL ADC
        if(CONTADOR>10){
            ADCON0bits.GO_nDONE = 1;    //HABILITA LECTURA NUEVAMENTE
            CONTADOR = 0;               //SE REINICIA EL CONTADOR
        }
    }
}

//******************************************************************************
//SETUP
//******************************************************************************

void Setup(void) {
    //GLOBAL
    PORTA = 0; //LCD
    PORTB = 0; //NO SE USAN
    PORTC = 0; //TX Y RX
    PORTD = 0; //EN, RS Y RW
    PORTE = 0; //POTENCIOMETROS

    ANSEL = 0;
    ANSELbits.ANS5 = 1;  //POT EN E0
    ANSELH = 0;

    TRISA = 0b00100000; //TODOS OUTPUTS --> LCD
    TRISB = 0; //TODOS OUTPUTS --> SIN USAR
    TRISC = 0b00011000; //TODOS OUTPUTS --> TX Y RX
    TRISD = 0; //TODOS OUTPUTS --> EN, RS Y RW
    TRISE = 1; //2 INPUTS --> POTENCIOMETROS

    //INTERRUPCIONES
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.SSPIE = 1;
    INTCONbits.T0IE = 1;
    INTCONbits.T0IF = 0;
    PIE1bits.ADIE = 1;

    //OSCILADOR
    initOsc(6);
    //ADC
    initADC(2,5);
    //MSSP
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    //TIMER0
    WDTCON = 0;
    OPTION_REG = 0b11010111;
}

//******************************************************************************
//SUBRUTINAS
//******************************************************************************