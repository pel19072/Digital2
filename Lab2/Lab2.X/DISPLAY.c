#include "DISPLAY.h"

/*
 INICIALIZACION DE DISPLAY
 */
void display_anodo(uint8_t VAR){
    
    switch(VAR){
        case 0: //0
            PORTC = 0b10001000;
            break;   
        case 1: //1
            PORTC = 0b11101011;
            break;   
        case 2: //2
            PORTC = 0b01001100;
            break;   
        case 3: //3
            PORTC = 0b01001001;
            break;   
        case 4: //4
            PORTC = 0b00101011;
            break;   
        case 5: //5
            PORTC = 0b00011001;
            break;   
        case 6: //6
            PORTC = 0b00011000;
            break;   
        case 7: //7
            PORTC = 0b11001011;
            break;   
        case 8: //8
            PORTC = 0b00001000;
            break;   
        case 9: //9
            PORTC = 0b00001011;
            break;   
        case 10: //A
            PORTC = 0b00000010;
            break;   
        case 11: //B
            PORTC = 0b00110000;
            break;   
        case 12: //C
            PORTC = 0b10010100;
            break;   
        case 13: //D
            PORTC = 0b01100000;
            break;   
        case 14: //E
            PORTC = 0b00010100;
            break;   
        case 15: //F
            PORTC = 0b00010110;
            break;   
        default: //0
            PORTC = 0b10001000;
            break;
    }    
}
