#ifndef __MAIN_H
#define __MAIN_H

#define USE_OR_MASKS

void init(void);
void initTimers(void);
void initADC(void);

void InitializeUART(void);
void SendUART(char*);
void SendUARTchar(char);


/* ---------------------------------------
 * INTERRUPT SETUP - the code below assigns
 * function names to their respective interrupt
 * routines
 * ---------------------------------------*/
void int_low(void);
void int_high(void);
void high_isr(void);
void low_isr(void);


/* CONFIG3H pragma declaration.
 * Select 10-bit conversion mode for the ADC. The 10-bit conversion mode is
 * useful for applications that favor conversion speed over conversion resolution.
 */
#pragma config ADCSEL = BIT10

// Interrupt pragma declarations.
#pragma code high_vector = 0x08 //setup the high ISR vector
void int_high(void) {_asm GOTO high_isr _endasm}
#pragma code
#pragma interrupt high_isr //the high ISR

#pragma code low_vector = 0x18 //setup the low ISR vector
void int_low(void) {_asm GOTO low_isr _endasm}
#pragma code
#pragma interruptlow low_isr //the low ISR


void test_pulse(void);

#endif