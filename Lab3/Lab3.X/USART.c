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