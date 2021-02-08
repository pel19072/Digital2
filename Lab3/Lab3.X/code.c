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
#include "ADC.h"
#include "LCD.h"
#include "Oscilador.h"
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
uint8_t VAR_ADC1 = 0;
uint8_t VAR_ADC2 = 0;
uint8_t CONTADOR = 0;
uint8_t TOGGLE = 0;
uint8_t RECEPCION = 0;
uint8_t FLAGADC = 0;
uint8_t FLAGTX = 0;
uint8_t FLAGD = 0;
uint8_t FLAGI = 0;

//******************************************************************************
//INSTANCIACION DE FUNCIONES
//******************************************************************************
void Setup(void);
void Prueba(void);
uint8_t Envio(void);
void Conversiones();
void Envio_Contador(void);

//******************************************************************************
//ISR
//******************************************************************************
void __interrupt() isr(void) { 
    di();   //DESACTIVA LAS INTERRUPCIONES
    //INTERUPCION DEL TIMER0
    if(INTCONbits.T0IF == 1){
        TMR0 = 236; //248 PARA 2ms | 236 PARA 5ms
        CONTADOR++; //CONTADOR PARA HABILITAR LA LECTURA DEL ADC
        INTCONbits.T0IF = 0;
    }
    //INTERUPCION DEL ADC
    if(PIR1bits.ADIF == 1){
        //MULTIPLEXACION
        if(FLAGADC == 0){
            VAR_ADC1 = ADRESH;
            initADC(2,5);   //CAMBIO DE CANAL
            FLAGADC = 1;
        }
        else{
            VAR_ADC2 = ADRESH;
            initADC(2,6);   //CAMBIO DE CANAL
            FLAGADC = 0;
        }        
        TOGGLE = 1;
        PIR1bits.ADIF = 0;
    }
    //INTERUPCION DEL RX
    if(PIR1bits.RCIF == 1){
        RECEPCION = RCREG;
        //ANTIREBOTE INCREMENTO CON ENTER
        if(RECEPCION == 0x2B){
            FLAGI = 1;
        }
        else if(FLAGI == 1 & RECEPCION == 0x0D){
            FLAGI = 0;
            PORTB++;
        }
        //ANTIREBOTE DECREMENTO CON ENTER
        else if(RECEPCION == 0x2D){
            FLAGD = 1;
        }
        else if(FLAGD == 1 & RECEPCION == 0x0D){
            FLAGD = 0;
            PORTB--;
        }
        //PARA QUE SOLO FUNCIONE CUANDO SE PRESIONA + O - Y UN ENTER
        else if(RECEPCION != 0x0D & RECEPCION != 0x2B & RECEPCION != 0x2D){
            FLAGD = 0;
            FLAGI = 0;
        }
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
    Lcd_Write_String("Sen 1");
    Lcd_Set_Cursor(1, 7);
    Lcd_Write_String("Sen 2");
    Lcd_Set_Cursor(1, 13);
    Lcd_Write_String("Cont");
    while(1){ 
        //SIRVE PARA DARLE UN TIEMPO A LA LECTURA DEL ADC
        if(CONTADOR>10){
            ADCON0bits.GO_nDONE = 1;    //HABILITA LECTURA NUEVAMENTE
            PIE1bits.TXIE = 1;          //HABILITA EL ENVIO NUEVAMENTE
            CONTADOR = 0;               //SE REINICIA EL CONTADOR
        }
        Conversiones();     //ENVIO DE LOS VALORES DE SENSORES
        Envio_Contador();   //ENVIO DEL CONTADOR
    }
}

//******************************************************************************
//SETUP
//******************************************************************************
void Setup(void) {
    //GLOBAL
    PORTA = 0;  //LCD
    PORTB = 0;  //NO SE USAN
    PORTC = 0;  //TX Y RX
    PORTD = 0;  //EN, RS Y RW
    PORTE = 0;  //POTENCIOMETROS
    
    ANSEL = 0;
    ANSELbits.ANS5 = 1;  //POT EN E0
    ANSELbits.ANS6 = 1;  //POT EN E1
    ANSELH = 0;
    
    TRISA = 0;  //TODOS OUTPUTS --> LCD
    TRISB = 0; //TODOS OUTPUTS --> SIN USAR
    TRISC = 0b10000000;  //TODOS OUTPUTS --> TX Y RX
    TRISD = 0;  //TODOS OUTPUTS --> EN, RS Y RW
    TRISE = 3;  //2 INPUTS --> POTENCIOMETROS
    
    //INTERRUPCIONES
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.T0IE = 1;
    INTCONbits.T0IF = 0;
    PIE1bits.ADIE = 1;
    PIE1bits.RCIE = 1; 
    PIE1bits.TXIE = 1; 
    
    //OSCILADOR
    initOsc(6);    
    //ADC
    initADC(2,5);
    //RX Y TX
    initUART();
    //LCD
    Lcd_Init();
    //TIMER0
    WDTCON = 0;
    OPTION_REG = 0b11010111;
}

//******************************************************************************
//SUBRUTINAS
//******************************************************************************
//MULTIPLEXACION PARA ENVIO DE POTENCIOMETROS
uint8_t Envio(void){
    uint8_t temporal;
    switch(FLAGTX){
        case 0:
            FLAGTX++;
            return 0x28;    //PARENTESIS IZQUIERDO
            break;
        case 1:
            temporal = (VAR_ADC1 & 0xF0)>>4;
            FLAGTX++;
            return ASCII(temporal);
            break;
        case 2:            
            temporal = VAR_ADC1 & 0x0F;
            FLAGTX++;
            return ASCII(temporal);
            break;
        case 3:            
            FLAGTX++;
            return 0x2C;    //COMA
            break;
        case 4:
            temporal = (VAR_ADC2 & 0xF0)>>4;            
            FLAGTX++;
            return ASCII(temporal);
            break;
        case 5:
            FLAGTX++;
            temporal = VAR_ADC2 & 0x0F;
            return ASCII(temporal);
            break;
        case 6:
            FLAGTX++;
            return 0x29;    //PARENTESIS DERECHO
            break;
        case 7:
            FLAGTX = 0;
            return 0x0D;    //ENTER
            break;
    }    
}

void Conversiones(){
    uint16_t temporal;
    uint8_t unidades;
    uint8_t decimas;
    uint8_t centimas;
    //MULTIPLEXACION DE CONVERSIONES
    if(FLAGADC == 0){
        //MAPPEO
        temporal = (VAR_ADC1*100)/51;
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
    else{
        //MAPPEO
        temporal = (VAR_ADC2*100)/51;
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
        Lcd_Write_String(".");   //PUNTO DECIMAL
        Lcd_Set_Cursor(2,9);
        Lcd_Write_String(Cambio(decimas));
        Lcd_Set_Cursor(2,10);
        Lcd_Write_String(Cambio(centimas));  
        Lcd_Set_Cursor(2,11); 
    }
}

void Envio_Contador(void){
    uint8_t temporal;
    uint8_t unidades;
    uint8_t decimas;
    uint8_t centimas;
    temporal = PORTB;
    //REDONDEO DE UNIDADES
    unidades = temporal/100;
    temporal = temporal - 100*unidades;
    //REDONDEO DE DECIMAS
    decimas = temporal/10;
    temporal = temporal - 10*decimas;
    //REDONDEO DE CENTIMAS
    centimas = temporal;
    
    Lcd_Set_Cursor(2,13);
    Lcd_Write_String(Cambio(unidades));
    Lcd_Set_Cursor(2,14);
    Lcd_Write_String(Cambio(decimas));
    Lcd_Set_Cursor(2,15);
    Lcd_Write_String(Cambio(centimas));  
    Lcd_Set_Cursor(2,16);   
}

/*
const char* string_converter(char sens1, char sens2){
    char temporal[];
    temporal[0] = sens1;
    temporal[1] = sens2;
    return temporal;
}
 */