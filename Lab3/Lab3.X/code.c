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
#define  _XTAL_FREQ 4000000 //Necesario para la funci�n de Delay
uint8_t VAR_ADC1 = 0;
uint8_t VAR_ADC2 = 0;
uint8_t CONTADOR = 0;
uint8_t TOGGLE = 0;
uint8_t RECEPCION = 0;
uint8_t FLAGADC = 0;
uint8_t FLAGTX = 0;

//******************************************************************************
//INSTANCIACION DE FUNCIONES
//******************************************************************************
void Setup(void);
void Prueba(void);
uint8_t Envio(void);
void Conversiones(uint8_t sensor);

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
        if(FLAGADC == 0){
            VAR_ADC1 = ADRESH;
            initADC(2,5);
            FLAGADC = 1;
        }
        else{
            VAR_ADC2 = ADRESH;
            initADC(2,6);
            FLAGADC = 0;
        }        
        TOGGLE = 1;
        PIR1bits.ADIF = 0;
    }
    //INTERUPCION DEL RX
    if(PIR1bits.RCIF == 1){
        RECEPCION = RCREG;
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
    
    while(1){ 
        //SIRVE PARA DARLE UN TIEMPO A LA LECTURA DEL ADC
        if(CONTADOR>10){
            ADCON0bits.GO_nDONE = 1;    //HABILITA LECTURA NUEVAMENTE
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
    TRISC = 0;  //TODOS OUTPUTS --> TX Y RX
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
uint8_t Envio(void){
    uint8_t temporal;
    switch(FLAGTX){
        case 0:
            temporal = VAR_ADC1 & 0x0F;
            FLAGTX++;
            return ASCII(temporal);
            break;
        case 1:
            temporal = (VAR_ADC1 & 0xF0)>>4;
            FLAGTX++;
            return ASCII(temporal);
            break;
        case 2:            
            FLAGTX++;
            return 0x2C;
            break;
        case 3:
            temporal = VAR_ADC2 & 0x0F;
            FLAGTX++;
            return ASCII(temporal);
            break;
        case 4:
            FLAGTX++;
            temporal = (VAR_ADC2 & 0xF0)>>4;
            return ASCII(temporal);
            break;
        case 5:
            FLAGTX = 0;
            return 0x0A;
            break;
    }    
}

void Conversiones(uint8_t sensor){
    uint16_t temporal;
    uint8_t unidades;
    uint8_t decimas;
    uint8_t centimas;
    //MAPPEO
    temporal = (sensor/51)*100;
    //REDONDEO DE UNIDADES
    unidades = temporal/100;
    temporal = temporal - 100*unidades;
    //REDONDEO DE DECIMAS
    decimas = temporal/10;
    temporal = temporal - 10*decimas;
    //REDONDEO DE CENTIMAS
    centimas = temporal;
    //ENVIO
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_Char(unidades);
    /*
    Lcd_Write_Char(0x2E);   //PUNTO DECIMAL
    Lcd_Write_Char(decimas);
    Lcd_Write_Char(centimas);    
    */
    
}

void Prueba(void){
    /*
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("LCD Library for");
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String("MPLAB XC8");
    __delay_ms(2000);
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("Developed By");
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String("electroSome");
    __delay_ms(2000);
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("www.electroSome.com");

    for(int a=0;a<15;a++)
    {
        __delay_ms(300);
        Lcd_Shift_Left();
    }

    for(int a=0;a<15;a++)
    {
        __delay_ms(300);
        Lcd_Shift_Right();
    }
     */
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_Char('e');
    Lcd_Write_Char('S');
    __delay_ms(2000);
}