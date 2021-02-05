#include "USART.h"

/*
 INICIALIZACION DE RX Y TX: PAG 158
 */
void initUART(void){
    //ENVIO
    TXSTAbits.TX9 = 0;
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    BAUDCTLbits.BRG16 = 0;
    SPBRG = 25;
    SPBRGH = 0;
    TXSTAbits.TXEN = 1;
    //**************************************************************************
    //RECEPCION
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;    
}

uint8_t ASCII(uint8_t valor){
    switch(valor){
        case 0:
            return 0x30;
            break;
        case 1:
            return 0x31;
            break;
        case 2:
            return 0x32;
            break;
        case 3:
            return 0x33;
            break;
        case 4:
            return 0x34;
            break;
        case 5:
            return 0x35;
            break;
        case 6:
            return 0x36;
            break;
        case 7:
            return 0x37;
            break;
        case 8:
            return 0x38;
            break;
        case 9:
            return 0x39;
            break;
        case 10:
            return 0x41;
            break;
        case 11:
            return 0x42;
            break;
        case 12:
            return 0x43;
            break;
        case 13:
            return 0x44;
            break;
        case 14:
            return 0x45;
            break;
        case 15:
            return 0x46;
            break;
    }
}