#include "DISPLAY.h"

/*
 INICIALIZACION DE DISPLAY
 */
void display_anodo(uint8_t VAR, uint8_t PORT){
    
    switch(VAR){
        case 0: //0
            PORT = 0b10001000;
            break;   
        case 1: //1
            PORT = 0b11101011;
            break;   
        case 2: //2
            PORT = 0b01001100;
            break;   
        case 3: //3
            PORT = 0b01001001;
            break;   
        case 4: //4
            PORT = 0b00101011;
            break;   
        case 5: //5
            PORT = 0b00011001;
            break;   
        case 6: //6
            PORT = 0b00011000;
            break;   
        case 7: //7
            PORT = 0b11001011;
            break;   
        case 8: //8
            PORT = 0b00001000;
            break;   
        case 9: //9
            PORT = 0b00001011;
            break;   
        case 10: //A
            PORT = 0b00000010;
            break;   
        case 11: //B
            PORT = 0b00110000;
            break;   
        case 12: //C
            PORT = 0b10010100;
            break;   
        case 13: //D
            PORT = 0b01100000;
            break;   
        case 14: //E
            PORT = 0b00010100;
            break;   
        case 15: //F
            PORT = 0b00010110;
            break;   
        default: //0
            PORT = 0b10001000;
            break;
    }    
}
