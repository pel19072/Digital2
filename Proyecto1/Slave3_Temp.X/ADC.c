#include "ADC.h"

/*
 INICIALIZACION DE ADC: PARAMETROS: PAG 104
 */
void initADC(uint8_t ADCS, uint8_t CHS){
    //CANAL DE LECTURA
    switch(CHS){
        case 0: //AN0
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;
            break;   
        case 1: //AN1
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 1;
            break;   
        case 2: //AN2
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 0;
            break;   
        case 3: //AN3
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 1;
            break;   
        case 4: //AN4
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;
            break;   
        case 5: //AN5
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 1;
            break;   
        case 6: //AN6
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 0;
            break;   
        case 7: //AN7
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 1;
            break;   
        case 8: //AN8
            ADCON0bits.CHS3 = 1;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;
            break;   
        case 9: //AN9
            ADCON0bits.CHS3 = 1;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 1;
            break;   
        case 10: //AN10
            ADCON0bits.CHS3 = 1;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 0;
            break;   
        case 11: //AN11
            ADCON0bits.CHS3 = 1;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 1;
            break;   
        case 12: //AN12
            ADCON0bits.CHS3 = 1;
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;
            break;   
        case 13: //AN13
            ADCON0bits.CHS3 = 1;
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 1;
            break;   
        case 14: //CVREF
            ADCON0bits.CHS3 = 1;
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 0;
            break;   
        case 15: //FIXED REF
            ADCON0bits.CHS3 = 1;
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 1;
            break;   
        default: //AN0
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;
            break;
    }
    //VELOCIDAD DE LECTURA
    switch(ADCS){
        case 0: //FOSC/2
            ADCON0bits.ADCS0 = 0;
            ADCON0bits.ADCS0 = 0;
            break;
        case 1: //FOSC/8
            ADCON0bits.ADCS0 = 0;
            ADCON0bits.ADCS0 = 1;
            break;
        case 2: //FOSC/32
            ADCON0bits.ADCS0 = 1;
            ADCON0bits.ADCS0 = 0;
            break;
        case 3: //FRC
            ADCON0bits.ADCS0 = 1;
            ADCON0bits.ADCS0 = 1;
            break;
        default: //FOSC/2
            ADCON0bits.ADCS0 = 0;
            ADCON0bits.ADCS0 = 0;
            break;
    }
    //ACTIVACION DEL ADC
    ADCON0bits.ADON = 1;
    //DA EL AVISO PARA COMENZAR A LEER
    ADCON0bits.GO_nDONE = 1;
    //JUSTIFICACION A LA IZQUIERDA Y VOLTAJES DE REFERENCIA
    ADCON1 = 0b00010000;
    //DELAY PARA QUE PASE EL TIEMPO DE ADQUISICION 11 Tads
    __delay_ms(11); 
}
