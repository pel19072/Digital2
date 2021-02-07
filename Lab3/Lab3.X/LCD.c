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
    __delay_ms(10);
    EN  = 0;    // => E = 0
}

void Lcd_Clear(void){
	Lcd_Cmd(0x01);
}

void Lcd_Set_Cursor(char a, char b){
	if(a == 1){
        Lcd_Cmd(0x80 + b - 1);
	}
	else if(a == 2){
		Lcd_Cmd(0xC0 + b - 1);
	}
}

void Lcd_Init(void){
    __delay_ms(15);
    Lcd_Port(0x00);
    __delay_ms(20);
    Lcd_Cmd(0x30);
    __delay_ms(5);
    Lcd_Cmd(0x30);
    __delay_us(160);
    Lcd_Cmd(0x30);
    //**************************************************************************
    Lcd_Cmd(0x38);  //2 LINEAS | 5x8
    Lcd_Cmd(0x10);  
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x06);  //INCREMENTO | DESPLAZAMIENTO DESACTIVADO
    //Lcd_Cmd(0x0F);
    Lcd_Cmd(0x0C);
}

void Lcd_Write_Char(char a){
    RS = 1;             // => RS = 1
    Lcd_Port(a);        //Data transfer
    EN = 1;
    __delay_us(40);
    EN = 0;
}

void Lcd_Write_String(char *a){
	for(int i=0;a[i]!='\0';i++){
        Lcd_Write_Char(a[i]);
    }        
}

void Lcd_Shift_Right(void){
	Lcd_Cmd(0x1C);
}

void Lcd_Shift_Left(void){
	Lcd_Cmd(0x18);
}

const char* Cambio(uint8_t numero){
    switch(numero){
        case 0:
            return "0";
            break;
        case 1:
            return "1";
            break;
        case 2:
            return "2";
            break;
        case 3:
            return "3";
            break;
        case 4:
            return "4";
            break;
        case 5:
            return "5";
            break;
        case 6:
            return "6";
            break;
        case 7:
            return "7";
            break;
        case 8:
            return "8";
            break;
        case 9:
            return "9";
            break;
    }
}