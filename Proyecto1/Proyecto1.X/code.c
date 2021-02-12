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
#include "LCD.h"
#include "USART.h"

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
uint8_t FLAGTX = 0;
uint8_t FLAGSS = 0;
uint8_t COUNTER = 0;
uint8_t TEMPERATURA = 0;

//******************************************************************************
//INSTANCIACION DE FUNCIONES
//******************************************************************************
void Setup(void);
void Conversiones();
uint8_t Envio(void);
void Envio_Contador(void);

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
    //INTERUPCION DEL TX
    if(PIR1bits.TXIF == 1){
        TXREG = Envio();
        PIE1bits.TXIE = 0;
    }
    ei(); //VUELVE A HABILITAR LAS INTERRUPCIONES
}

//******************************************************************************
//MAIN LOOP
//******************************************************************************

void main(void) {
    Setup();
    //VALORES FIJOS DE LA LCD
    Lcd_Set_Cursor(1, 2);
    Lcd_Write_String("ADC");
    Lcd_Set_Cursor(1, 7);
    Lcd_Write_String("CONT");
    Lcd_Set_Cursor(1, 13);
    Lcd_Write_String("TEMP");
    while (1) {
        if(CONTADOR>10){
            switch(FLAGSS){
                case 0: //SLAVE 1  
                    PORTCbits.RC0 = 0;
                    PORTCbits.RC1 = 1;
                    PORTCbits.RC2 = 1;
                    spiWrite(0x00);
                    VAR_ADC = spiRead();
                    Conversiones();
                    FLAGSS++;
                    break;
                case 1: //SLAVE 2
                    PORTCbits.RC0 = 1;
                    PORTCbits.RC1 = 0;
                    PORTCbits.RC2 = 1;
                    spiWrite(0x00);
                    COUNTER = spiRead();
                    Envio_Contador();
                    FLAGSS++;
                    break;
                case 2: //SLAVE 3
                    PORTCbits.RC0 = 1;
                    PORTCbits.RC1 = 1;
                    PORTCbits.RC2 = 0;
                    spiWrite(0x00);
                    TEMPERATURA = spiRead();
                    FLAGSS = 0;
                    break;
            }          
            PIE1bits.TXIE = 1;          //HABILITA EL ENVIO NUEVAMENTE  
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
    ANSELH = 0;

    TRISA = 0; //TODOS OUTPUTS --> LCD
    TRISB = 0; //TODOS OUTPUTS --> SIN USAR
    TRISC = 0b10010000; //TODOS OUTPUTS --> TX Y RX
    TRISD = 0; //TODOS OUTPUTS --> EN, RS Y RW
    TRISE = 0; //2 INPUTS --> POTENCIOMETROS

    //INTERRUPCIONES
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.T0IE = 1;
    INTCONbits.T0IF = 0;
    PIE1bits.TXIE = 1;
    //OSCILADOR
    initOsc(6);
    //LCD
    Lcd_Init();
    //RX Y TX
    initUART();
    //MSSP
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    //TIMER0
    WDTCON = 0;
    OPTION_REG = 0b11010111;
}

//******************************************************************************
//SUBRUTINAS
//******************************************************************************
void Conversiones(){
    uint16_t temporal;
    uint8_t unidades;
    uint8_t decimas;
    uint8_t centimas;
    //MAPPEO
    temporal = (VAR_ADC*100)/51;
    //REDONDEO DE UNIDADES
    unidades = temporal/100;
    temporal = temporal - 100*unidades;
    //REDONDEO DE DECIMAS
    decimas = temporal/10;
    temporal = temporal - 10*decimas;
    //REDONDEO DE CENTIMAS
    centimas = temporal;
    //ENVIO
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String(Cambio(unidades));
    Lcd_Set_Cursor(2,2);
    Lcd_Write_String(".");   //PUNTO DECIMAL
    Lcd_Set_Cursor(2,3);
    Lcd_Write_String(Cambio(decimas));
    Lcd_Set_Cursor(2,4);
    Lcd_Write_String(Cambio(centimas));  
    Lcd_Set_Cursor(2,5);        
}

//MULTIPLEXACION PARA ENVIO DE POTENCIOMETROS
uint8_t Envio(void){
    uint8_t temporal;
    switch(FLAGTX){
        case 0:
            FLAGTX++;
            return 0x28;    //PARENTESIS IZQUIERDO
            break;
        case 1:
            temporal = (VAR_ADC & 0xF0)>>4;
            FLAGTX++;
            return ASCII(temporal);
            break;
        case 2:            
            temporal = VAR_ADC & 0x0F;
            FLAGTX++;
            return ASCII(temporal);
            break;
        case 3:            
            FLAGTX++;
            return 0x2C;    //COMA
            break;
        case 4:
            temporal = (COUNTER & 0xF0)>>4;
            FLAGTX++;
            return ASCII(temporal);
            break;
        case 5:            
            temporal = COUNTER & 0x0F;
            FLAGTX++;
            return ASCII(temporal);
            break;
        case 6:            
            FLAGTX++;
            return 0x2C;    //COMA
            break;
        case 7:
            temporal = (TEMPERATURA & 0xF0)>>4;
            FLAGTX++;
            return ASCII(temporal);
            break;
        case 8:            
            temporal = TEMPERATURA & 0x0F;
            FLAGTX++;
            return ASCII(temporal);
            break;
        case 9:
            FLAGTX++;
            return 0x29;    //PARENTESIS DERECHO
            break;
        case 10:
            FLAGTX = 0;
            return 0x0D;    //ENTER
            break;
    }    
}

void Envio_Contador(void){
    uint8_t temporal;
    uint8_t unidades;
    uint8_t decimas;
    uint8_t centimas;
    temporal = PORTD;
    //REDONDEO DE UNIDADES
    unidades = temporal/100;
    temporal = temporal - 100*unidades;
    //REDONDEO DE DECIMAS
    decimas = temporal/10;
    temporal = temporal - 10*decimas;
    //REDONDEO DE CENTIMAS
    centimas = temporal;
    
    Lcd_Set_Cursor(2,7);
    Lcd_Write_String(Cambio(unidades));
    Lcd_Set_Cursor(2,8);
    Lcd_Write_String(Cambio(decimas));
    Lcd_Set_Cursor(2,9);
    Lcd_Write_String(Cambio(centimas));  
    Lcd_Set_Cursor(2,10);   
}