// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
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

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "oscilador.h"
#include "LCD.h"
#include "I2C.h"


#define _XTAL_FREQ 4000000

#define RS RB7
#define E RD7
#define D0 RA0
#define D1 RA1
#define D2 RA2
#define D3 RA3
#define D4 RA4
#define D5 RA5
#define D6 RA6
#define D7 RA7

void configPorts(void);
int  BCD_2_DEC(int to_convert);
int DEC_2_BCD (int to_convert);
void Set_Time_Date(void);
void Update_Current_Date_Time(void);
int sec, min, hour, day, date, month, year;

//Este segmento sirve para configurar la hora y solo se activa cuando se desee configurar.
int sec = 00;
int min = 55;
int hour = 10;
int date = 06;
int day = 1;
int month = 05;
int year = 18;
    /*Time and Date Set*/

void main(void) {
    initOsc(6);
    configPorts();
    Lcd_Init();         //se inicializa la pantalla
    I2C_Master_Init(100000);        //se inicializa el modo maestro con una frecuencia de 100k
    
    Set_Time_Date();      //Solo se activa cuando se desee configurar la hora
    
    while(1){        
        Update_Current_Date_Time(); //Read the current date and time from RTC module
    
        //Separa los digitos en chars para mostrarlos en la lcd
         char sec_0 = sec%10;
         char sec_1 = (sec/10);
         char min_0 = min%10;
         char min_1 = min/10;
         char hour_0 = hour%10;
         char hour_1 = hour/10;
         /*char date_0 = date%10;           //se comenta porque solo se desea mostrar hora, minutos y segundos.
         char date_1 = date/10;
         char month_0 = month%10;
         char month_1 = month/10;
         char year_0 = year%10;
         char year_1 = year/10;
         */
         Lcd_Set_Cursor(1, 1);
         Lcd_Write_Char(hour_1+48);
         Lcd_Write_Char(hour_0+48);
         Lcd_Write_String(":");
         Lcd_Write_Char(min_1+48);
         Lcd_Write_Char(min_0+48);
         Lcd_Write_String(":");
         Lcd_Write_Char(sec_1+48);
         Lcd_Write_Char(sec_0+48);

  
    }
}

void configPorts(void){
    TRISA = 0;
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;
    TRISB = 0b00010001;        //los pines RB0 y RB4 son entradas analógicas
    
    PORTA = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    PORTB = 0;
    
    ANSEL = 0;
    ANSELH = 0;
    
}

//************************************Funciones para el RTC*********************************

//Convierte los numeros de binario a decimal
int  BCD_2_DEC(int to_convert){
   return (to_convert >> 4) * 10 + (to_convert & 0x0F); 
}

//Convierte los numeros de decimal a binario
int DEC_2_BCD (int to_convert){
   return ((to_convert / 10) << 4) + (to_convert % 10);
}


//Funcion para configurar la hora
void Set_Time_Date(){
    I2C_Master_Start();
    I2C_Master_Write(0xD0);                 //Direccion del RTC
    I2C_Master_Write(0);
    I2C_Master_Write(DEC_2_BCD(sec));       //actualizar los segundos
    I2C_Master_Write(DEC_2_BCD(min));       //actualizar los minutos
    I2C_Master_Write(DEC_2_BCD(hour));      //actualizar la hora
    I2C_Master_Write(1);                    //actualiza el dia con un 1 siempre, pues no importa
    I2C_Master_Write(DEC_2_BCD(date));      //actualizar la fecha
    I2C_Master_Write(DEC_2_BCD(month));     //actualizar el mes
    I2C_Master_Write(DEC_2_BCD(year));      //actualizar el año
    I2C_Master_Stop();

}


//Funcion para leer la hora.
void Update_Current_Date_Time(){
   //START to Read 
   I2C_Master_Start(); 
   I2C_Master_Write(0xD0);
   I2C_Master_Write(0);
   I2C_Master_Stop();
   
   //READ
   I2C_Master_Start(); 
   I2C_Master_Write(0xD1);                      //Bloque para guardar los datos del RTC en variables
   sec = BCD_2_DEC(I2C_Master_Read(0));         //De una vez se convierten de binario a decimal
   I2C_Master_Stop();
   
   I2C_Master_Start(); 
   I2C_Master_Write(0xD1);                      //Bloque para guardar los datos del RTC en variables
   min = BCD_2_DEC(I2C_Master_Read(0));         //De una vez se convierten de binario a decimal
   I2C_Master_Stop();
    
   I2C_Master_Start(); 
   I2C_Master_Write(0xD1);                      //Bloque para guardar los datos del RTC en variables
   hour = BCD_2_DEC(I2C_Master_Read(0));         //De una vez se convierten de binario a decimal
   I2C_Master_Stop();
   //min = BCD_2_DEC(I2C_Master_Read(1));
   //hour = BCD_2_DEC(I2C_Master_Read(1));
   /*
   day = BCD_2_DEC(I2C_Master_Read(1));
   date = BCD_2_DEC(I2C_Master_Read(1));
   month = BCD_2_DEC(I2C_Master_Read(1));
   year = BCD_2_DEC(I2C_Master_Read(1));
   */
   I2C_Master_Stop();
   
   //End reading
   I2C_Master_Start(); 
   I2C_Master_Write(0xD1);
   I2C_Master_Read(0);
   I2C_Master_Stop();

}
