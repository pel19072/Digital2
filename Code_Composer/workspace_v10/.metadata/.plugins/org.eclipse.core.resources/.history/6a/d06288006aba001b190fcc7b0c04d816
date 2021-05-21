#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line) {
    while(1);
}
#endif

//Variables Globales
uint32_t LED = GPIO_PIN_1;
unsigned char temporal = 'a';
unsigned char temporal2 = 'f';
char time = 0;
char FLAG1 = 0;
char FLAG2 = 0;
char FLAG3 = 0;
char FLAG4 = 0;

//Prototipado
extern void Timer0IntHandler(void);
extern void UARTIntHandler(void);

int main(void) {
    // Utiliza el oscilador principal, con el cristal de 16MHz y el PLL y divido dentro de 5 para llegar a 40
    SysCtlClockSet(SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_SYSDIV_5);

    // Activa el puerto F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

    // Activa el puerto B
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));

    // Activa el puerto C
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC));

    // Activa el puerto D
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));

    //Activa interrupciones generales
    IntMasterEnable();

    // Activa Timer0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/2 - 1);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0IntHandler);
    IntEnable(INT_TIMER0A);
    TimerEnable(TIMER0_BASE, TIMER_A);

    // Activa UART0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0));
    GPIOPinConfigure(0x00000001); // RX
    GPIOPinConfigure(0x00000401); // TX --> ver pin_map.h
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
    UARTFIFOEnable(UART0_BASE);
    UARTEnable(UART0_BASE);

    // Activa UART1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART1));
    GPIOPinConfigure(GPIO_PB0_U1RX); // RX
    GPIOPinConfigure(GPIO_PB1_U1TX); // TX --> ver pin_map.h
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
    UARTFIFOEnable(UART1_BASE);
    UARTEnable(UART1_BASE);

    // Coloca como salida para LEDs
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3 );
    // Coloca como salida para LEDs
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_3);
    // Coloca como salida para LEDs
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    // Coloca como salida para LEDs
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_6);
    // Coloca como entradas los pines 0, 1, 2 y 3 del puerto D
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    while(1){
        if(GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_0) == 1){
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
            FLAG1 = 0X31;
        }
        else{
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 8);
            FLAG1 = 0X30;
        }

        if(GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_1) == 2){
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 8);
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0);
            FLAG2 = 0X31;
        }
        else{
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0);
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 16);
            FLAG2 = 0X30;
        }

        if(GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_2) == 4){
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 32);
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0);
            FLAG3 = 0X31;
        }
        else{
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0);
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 64);
            FLAG3 = 0X30;
        }
        if(GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_3) == 8){
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 128);
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, 0);
            FLAG4 = 0X31;
        }
        else{
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0);
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, 64);
            FLAG4 = 0X30;
        }
    }
}
void Timer0IntHandler(void) {
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    /*
    UARTCharPut(UART0_BASE, FLAG1);
    UARTCharPut(UART0_BASE, FLAG2);
    UARTCharPut(UART0_BASE, FLAG3);
    UARTCharPut(UART0_BASE, FLAG4);
    UARTCharPut(UART0_BASE, 0X0A);
    */

    UARTCharPut(UART1_BASE, FLAG1);
    UARTCharPut(UART1_BASE, FLAG2);
    UARTCharPut(UART1_BASE, FLAG3);
    UARTCharPut(UART1_BASE, FLAG4);
    UARTCharPut(UART1_BASE, 0X0A);
}
