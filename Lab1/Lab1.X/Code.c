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

//******************************************************************************
//CONFIGURACION
//******************************************************************************

// CONFIG1
#pragma config FOSC = EXTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
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

#define  _XTAL_FREQ 8000000

//BANDERAS PARA LOS ANTIREBOTES
unsigned char FLAG = 0;
unsigned char FLAG1 = 0;
unsigned char FLAG2 = 0;
//VARIABLE PARA EL INICIO DE LA CARRERA
unsigned char START = 0;

//******************************************************************************
//INSTANCIACION DE FUNCIONES
//******************************************************************************
void Setup(void);
void Lights(void);
//******************************************************************************
//MAIN LOOP
//******************************************************************************

void main(void) {
    Setup();
    while(1){
        //ANTIREBOTE DEL BOTON DE INICIO
        if (PORTBbits.RB0 == 0){
            FLAG = 1;
        }
        else{
            if (FLAG == 1){
                FLAG = 0;
                Lights();
            }
        }
        // JUGADOR 1:
        //ANTIREBOTE DEL BOTON DE AVANCE DEL JUGADOR 1
        if (PORTBbits.RB1 == 0 && START == 1){
            FLAG1 = 1;
        }
        else{
            if (FLAG1 == 1){
                FLAG1 = 0;
                if (PORTC == 0){
                    //LA DA EL VALOR INICIAL AL PUERTO PARA LA CARRERA
                    PORTC = 1; 
                }
                else if (PORTC == 0b01000000) {
                    //OPCION CUANDO UN JUGADOR GANA:
                    PORTC = PORTC<<1;
                    //BIT PARA INDICAR QUIEN GANA
                    PORTBbits.RB3 = 1;
                    //REINICIA LA VARIABLE DE START PARA LA CARRERA
                    START = 0;
                }
                else{
                    PORTC = PORTC<<1;
                }
            }
        }
        // JUGADOR 2
        //MISMA LOGICA QUE PARA EL JUGADOR 1
        if (PORTBbits.RB2 == 0 && START == 1){
            FLAG2 = 1;
        }
        else{
            if (FLAG2 == 1){
                FLAG2 = 0;
                if (PORTD == 0){
                    PORTD = 1;
                }
                else if (PORTD == 0b01000000) {
                    PORTD = PORTD<<1;
                    PORTBbits.RB4 = 1;
                    START = 0;
                }
                else{
                    PORTD = PORTD<<1;
                }
            }
        }
        
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
}

//******************************************************************************
//SUBRUTINAS
//******************************************************************************

void Lights(void){
    //REINICIA LOS JUGADORES
    PORTC = 0;
    PORTD = 0;
    //REINICIA LOS BITS DE GANADORES
    PORTBbits.RB3 = 0;
    PORTBbits.RB4 = 0;
    //DA UN VALOR INICIAL AL SEMAFORO
    PORTE = 1;
    //CAMBIA LAS LUCES DEL SEMAFORO CON UN DELAY DE 50ms
    for(int i = 0; i < 3; i++){
        __delay_ms(50);
        PORTE = PORTE<<1;
    }
    //PERMITE QUE LA CARRERA REINICIE
    START = 1;
}      