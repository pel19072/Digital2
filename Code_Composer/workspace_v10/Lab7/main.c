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

//Prototipado
extern void Timer0IntHandler(void);
extern void UARTIntHandler(void);

int main(void) {
    // Utiliza el oscilador principal, con el cristal de 16MHz y el PLL y divido dentro de 5 para llegar a 40
    SysCtlClockSet(SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_SYSDIV_5);

    // Activa el puerto F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

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

    // Activa UART
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0));
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    UARTIntRegister(UART0_BASE,UARTIntHandler);

    // Coloca como salida los pines 1, 2 y 3 del puerto F (los pines de los LEDs)
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    while(1);
}

void Timer0IntHandler(void) {
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // Read the current state of the GPIO pin and
    // write back the opposite state
    GPIOPinWrite(GPIO_PORTF_BASE, LED, ~GPIOPinRead(GPIO_PORTF_BASE, LED));
}

void UARTIntHandler(void) {
    // Clear the timer interrupt
    UARTIntClear(UART0_BASE, UART_INT_RX | UART_INT_RT);
    if (UARTCharsAvail(UART0_BASE)){
        if(UARTCharGetNonBlocking(UART0_BASE) == 'r') {
            LED = GPIO_PIN_1;
        }
        else if(UARTCharGetNonBlocking(UART0_BASE) == 'g') {
            LED = GPIO_PIN_2;
        }
        else {
            LED = GPIO_PIN_3;
        }
    }
    else{
        LED = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
    }
}
