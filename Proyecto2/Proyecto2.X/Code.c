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
#include "Oscilador.h"
#include "USART.h"
#include "I2C.h"

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
uint8_t CONTADOR = 0;
uint8_t FLAGTX = 0;
uint8_t FLAGRX = 0;
uint8_t FLAGI2C = 0;
uint8_t SECONDS = 0;
uint8_t MINUTES = 0;
uint8_t HOUR = 0;
uint8_t DAY = 0;
uint8_t DATE = 0;
uint8_t MONTH = 0;
uint8_t YEAR = 0;
uint8_t RECEPCION_LED = 0;
uint8_t RECEPCION_ACTIVACION = 0;
uint8_t RECEPCION_ENTER = 0;

//******************************************************************************
//INSTANCIACION DE FUNCIONES
//******************************************************************************
void Setup(void);
uint8_t Envio(void);
void I2C_RTC_Init(void);

//******************************************************************************
//ISR
//******************************************************************************

void __interrupt() isr(void) {
    di(); //DESACTIVA LAS INTERRUPCIONES
    //INTERUPCION DEL TIMER0
    if (INTCONbits.T0IF == 1) {
        TMR0 = 236; //248 PARA 2ms | 236 PARA 5ms
        CONTADOR++; //CONTADOR PARA HABILITAR LA LECTURA DEL ADC
        INTCONbits.T0IF = 0;
    }
    //INTERUPCION DEL TX
    if (PIR1bits.TXIF == 1) {
        TXREG = Envio();
        PIE1bits.TXIE = 0;
    }
    //INTERUPCION DEL RX
    if (PIR1bits.RCIF == 1) {
        /*
        Recibo el estado de las dos LEDs y un indicador (el enter) 
        */
        switch (FLAGRX) {
            case 0:
                FLAGRX++;
                PORTA = RCREG;
                break;
            case 1:
                FLAGRX++;
                PORTB = RCREG;
                break;
            case 2:
                FLAGRX = 0;
                RECEPCION_ENTER = RCREG;
                break;
        }
    }
    ei(); //VUELVE A HABILITAR LAS INTERRUPCIONES
}

//******************************************************************************
//MAIN LOOP
//******************************************************************************

void main(void) {
    Setup();
    I2C_RTC_Init();     //CONFIGURACIÓN
    while (1) {
        //START to Read 
        I2C_Master_Start();
        I2C_Master_Write(0xD0); //COLOCO EL POINTER EL RELOJ EN 00h
        I2C_Master_Write(0);

        //READ
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xD1); //COMIENZO LA LECTURA
        SECONDS = I2C_Master_Read(1); //COMIENZO A GUARDAR VALORES
        MINUTES = I2C_Master_Read(1); //EL POINTER DEL RTC AUMENTA SOLO
        HOUR = I2C_Master_Read(1);
        DAY = I2C_Master_Read(1);      
        DATE = I2C_Master_Read(1);
        MONTH = I2C_Master_Read(1);
        YEAR = I2C_Master_Read(0);
        I2C_Master_Stop();

        //**********************************************************************
        //COMIENZA A MANDAR SÍ Y SOLO SÍ EL INDICADOR RECIBIDO ES UN ENTER
        if (RECEPCION_ENTER == 0x0A) {
            INTCONbits.T0IE = 1;
            if (CONTADOR > 10) {
                PIE1bits.TXIE = 1; //HABILITA EL ENVIO NUEVAMENTE  
                CONTADOR = 0; //SE REINICIA EL CONTADOR  
            }
        }
        else {
            INTCONbits.T0IE = 0;
            INTCONbits.T0IF = 0;
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
    TRISC = 0b10011000; //TODOS OUTPUTS --> TX Y RX
    TRISD = 0; //TODOS OUTPUTS --> EN, RS Y RW
    TRISE = 0; //2 INPUTS --> POTENCIOMETROS

    //INTERRUPCIONES
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.T0IE = 1;
    INTCONbits.T0IF = 0;
    PIE1bits.TXIE = 1;
    PIE1bits.RCIE = 1;
    //OSCILADOR
    initOsc(6);
    //RX Y TX
    initUART();
    //TIMER0
    WDTCON = 0;
    OPTION_REG = 0b11010111;
    //I2C
    I2C_Master_Init(100000);
}

//******************************************************************************
//SUBRUTINAS
//******************************************************************************

//MULTIPLEXACION PARA ENVIO DE POTENCIOMETROS

uint8_t Envio(void) {
    switch (FLAGTX) {
        case 0:
            FLAGTX++;
            return ASCII((YEAR & 0xF0) >> 4);
            break;
        case 1:
            FLAGTX++;
            return ASCII(YEAR & 0x0F);
            break;
        case 2:
            FLAGTX++;
            return 0x2F; //DIAGONAL
            break;
        case 3:
            FLAGTX++;
            return ASCII((MONTH & 0x10) >> 4);
            break;
        case 4:
            FLAGTX++;
            return ASCII(MONTH & 0x0F);
            break;
        case 5:
            FLAGTX++;
            return 0x2F; //DIAGONAL
            break;
        case 6:
            FLAGTX++;
            return ASCII((DATE & 0x30) >> 4);
            break;
        case 7:
            FLAGTX++;
            return ASCII(DATE & 0x0F);
            break;
        case 8:
            FLAGTX++;
            return 0x09; //TAB
            break;
        case 9:
            FLAGTX++;
            return ASCII((HOUR & 0x10) >> 4);
            break;
        case 10:
            FLAGTX++;
            return ASCII(HOUR & 0x0F);
            break;
        case 11:
            FLAGTX++;
            return 0x3A; //DOS PUNTOS
            break;
        case 12:
            FLAGTX++;
            return ASCII((MINUTES & 0xF0) >> 4);
            break;
        case 13:
            FLAGTX++;
            return ASCII(MINUTES & 0x0F);
            break;
        case 14:
            FLAGTX++;
            return 0x3A; //DOS PUNTOS
            break;
        case 15:
            FLAGTX++;
            return ASCII((SECONDS & 0xF0) >> 4);
            break;
        case 16:
            FLAGTX++;
            return ASCII(SECONDS & 0x0F);
            break;
        case 17:
            RECEPCION_ENTER = 0x00; //REINICIO EL INDICADOR PARA VOLVER A MANDAR
                                    //SOLO CUANDO EL ESP32 LO PIDA
            FLAGTX = 0;
            return 0x0A; //ENTER -> 0x0D PARA PROTEUS | 0x0A ASCII
            break;
    }
}
//INICIALIZACION DE DATOS DEL RELOJ

void I2C_RTC_Init(void) {
    I2C_Master_Start();
    I2C_Master_Write(0b11010000); // SLAVE | WRITE
    I2C_Master_Write(0x00); // POINTER EN 00h
    I2C_Master_Write(0x00); // SEGUNDOS
    I2C_Master_Write(0x22); // MINUTOS
    I2C_Master_Write(0x41); // HORAS | FORMATO
    I2C_Master_Write(0x01); // DIA DE LA SEMANA
    I2C_Master_Write(0x01); // FECHA (DIA)
    I2C_Master_Write(0x03); // FECHA (MES)
    I2C_Master_Write(0x21); // AÑO
    I2C_Master_Stop();
    __delay_ms(20);
}