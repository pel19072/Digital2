/*
 * File:   LCD.c
 * Author: Ricardo
 * Obtained from: electrosome.com
 * Created on February 4, 2021, 2:44 AM
 */

#include "LCD.h"
//VARIABLES GLOBALES DE LA LCD
#define EN PORTDbits.RD0
#define RS PORTDbits.RD2

void Lcd_Port(char a){
	PORTA = a;
}

void Lcd_Cmd(char a){
	RS = 0;     // => RS = 0
	Lcd_Port(a);
	EN  = 1;    // => E = 1
    __delay_ms(4);
    EN  = 0;    // => E = 0
}

void Lcd_Clear(void){
	Lcd_Cmd(0);
	Lcd_Cmd(1);
}

void Lcd_Set_Cursor(char a, char b){
	char temp;
	if(a == 1){
        temp = 0x80 + b - 1;
        Lcd_Cmd(temp);
	}
	else if(a == 2){
		temp = 0xC0 + b - 1;
		Lcd_Cmd(temp);
	}
}

void Lcd_Init(void){
    Lcd_Port(0x00);
     __delay_ms(10);
    Lcd_Cmd(0x30);
      __delay_ms(4);
    Lcd_Cmd(0x30);
      __delay_us(90);
    Lcd_Cmd(0x30);
    //**************************************************************************
    Lcd_Cmd(0x38);  //2 LINEAS | 5x8
    Lcd_Cmd(0x08);  
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x06);  //INCREMENTO | DESPLAZAMIENTO DESACTIVADO
}

void Lcd_Write_Char(char a){
    RS = 1;             // => RS = 1
    Lcd_Port(a);        //Data transfer
    EN = 1;
    __delay_us(40);
    EN = 0;
    RS = 0;
}

void Lcd_Write_String(char *a){
	for(int i=0;a[i]!='\0';i++){
        Lcd_Write_Char(a[i]);
    }        
}

void Lcd_Shift_Right(void){
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x0C);
}

void Lcd_Shift_Left(void){
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x08);
}

