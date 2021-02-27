 /*
 * File            : I2C.c
 * Author          : Ligo George
 * Company         : electroSome
 * Project         : I2C Library for MPLAB XC8
 * Microcontroller : PIC 16F877A
 * Created on April 15, 2017, 5:59 PM
 * Link: https://electrosome.com/i2c-pic-microcontroller-mplab-xc8/
 * Modificada por: Pablo Mazariegos con la ayuda del auxiliar Gustavo Ordoñez 
 * Basado en Link: http://microcontroladores-mrelberni.com/i2c-pic-comunicacion-serial/
 * Obtenido de: https://github.com/pdmazariegos-uvg/ie3027/blob/master/Ejemplos/I2C/I2C-Master.X/I2C.c
 */
#include "I2C.h"

void I2C_Master_Init(const unsigned long c){
    SSPCON = 0b00101000;
    SSPCON2 = 0;
    SSPADD = (_XTAL_FREQ/(4*c))-1;
    SSPSTAT = 0;
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
}

void I2C_Master_Wait(){
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
    /*ESTO HACE UN OR ENTRE R/nW BIT DEL SSPASTAT Y ALGUNO DE LOS PRIMEROS 
    5 BITS DEL SSPCON2. ESTO VIENE DE:
    "OR-ing this bit with SEN, RSEN, PEN, RCEN, or ACKEN will indicate if 
    the MSSP is in Idle mode" (PAG. 176).
    EN OTRAS PALABRAS, REVISA SI LA COMUNICACIÓN ESTÁ ACTIVA O NO
    */
}

void I2C_Master_Start(){
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.SEN = 1;    //inicia la comunicación i2c
}

void I2C_Master_RepeatedStart(){
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.RSEN = 1;   //reinicia la comunicación i2c
}

void I2C_Master_Stop(){
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.PEN = 1;    //detener la comunicación i2c
}

void I2C_Master_Write(unsigned d){
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPBUF = d;             
}

unsigned short I2C_Master_Read(unsigned short a){
    unsigned short temp;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.RCEN = 1;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    temp = SSPBUF;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    if(a == 1){
        SSPCON2bits.ACKDT = 0;
    }else{
        SSPCON2bits.ACKDT = 1;
    }
    SSPCON2bits.ACKEN = 1;          // Iniciar sequencia de Acknowledge
    return temp;                    // Regresar valor del dato leído
}

void I2C_Slave_Init(uint8_t address){ 
    SSPADD = address;
    SSPCON = 0x36;      // 0b00110110 -> I2C Slave mode, 7-bit address 
    SSPSTAT = 0x80;     // 0b10000000 -> Sampled at the end
    SSPCON2 = 0x01;     // 0b00000001 -> Start condition
    TRISC3 = 1;
    TRISC4 = 1;
    // INTERRUPCIONES
    GIE = 1;
    PEIE = 1;
    SSPIF = 0;
    SSPIE = 1;
}

