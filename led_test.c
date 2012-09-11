#include <p18f27j53.h>
#include <timers.h>
#include <adc.h>
#include <delays.h>
#include "main.h"

//******************************
//              MAIN
//******************************
void main(void)
{
    init();

    // Makes an LED plugged into RA0 pwm at 50kHz (once every 20msec).
    while(1)
    {
        LATAbits.LATA0 = 1;
        Delay10KTCYx(2);
        LATAbits.LATA0 = 0;
        Delay10KTCYx(2);
    }
}

void init(void)
{
    /* TRIS tells you whether or not it's input or output (1 or 0), while PORT tells
	what you're outputting if you are outputting something. */
    TRISA = 0x00; //0x01 means PORTA is an input
    PORTA = 0x00; //0xFF for all pins on.

    // Set the reported oscillator control to 4 MHz (actual: 1 MHz).
    OSCCON = OSCCON | 0x60; 
}

