#include <p18f27j53.h>
#include <timers.h>
#include <adc.h>
#include <usart.h>
#include <delays.h>
#include "main.h"

#define WAITTIME(X) (0xFFFF - (X))
#define SERVO LATBbits.LATB6
#define MOTOR1 LATAbits.LATA0
#define MOTOR2 LATAbits.LATA3
#define ENCODERLED LATBbits.LATB0
#define ENCODEROUT PORTCbits.RC0
#define SPEED PORTCbits.RC1

// Clock for controlling the IR LEDs. LEDs are trigged on rising edge.
// Only one LED is on at a time. There is a resting stage where the output pin
// is connected to a 10k Ohm resistor. This stage allows us to not turn any of
// the LEDs on and save power. 
#define CLK LATBbits.LATB4
// Left-most sensor.
#define SENSOR1 LATBbits.LATB3
#define SENSOR2 LATBbits.LATB2
#define SENSOR3 LATBbits.LATB1
#define SENSOR4 LATBbits.LATB0
// Right-most sensor.
#define SENSOR5 LATCbits.LATC1
#define SENSOR (((SENSOR1 << 4) & 0x10) | ((SENSOR2 << 3) & 0x8) | ((SENSOR3 << 2) & 0x4) | ((SENSOR4 << 1) & 0x2) | SENSOR5)

// Sensor positions. Read from left to right as 5 bits. 1 indicates line, 0 indicates no line. 
#define POS00000 0x0
#define POS00001 0x1
#define POS00011 0x3
#define POS00010 0x2
#define POS00100 0x4
#define POS01100 0xC
#define POS01000 0x8
#define POS11000 0x18
#define POS10000 0x10
#define POS11111 0x1F

char servoFlag;
unsigned short servoDur, time; // Max of a char is 255 (8 bits).
int day, week;

    int test;

//******************************
//              MAIN
//******************************

void main(void)
{
    test=0;
    init();
    initTimers();
    initADC();
    //InitializeUART();

    //SendUART(str);

    while(1)
    {
        if(0 == day)
        {

//            ADCON0bits.ADON = 1; // turn on ADC sys
//            ADCON0 = (ADCON0 & 0xC3) | (10 << 2); //select channel
//            ADCON0bits.GO = 1; //start

            OpenADC(ADC_FOSC_2 & ADC_20_TAD& ADC_13ANA & ADC_REF_VDD_VSS & ADC_RIGHT_JUST,
                    ADC_INT_OFF,
                    0,
                    0);
            SetChanADC(ADC_CH9);
            ConvertADC();
            while(BusyADC())
                ;
            test = ReadADC();
            if (test > 512)//51)
            {
                LATCbits.LATC1 = 0;
            }
            else
            {
                LATCbits.LATC1 = 1;
            }
            CloseADC();
        }
    }

}

void init(void)
{
    // Initialize global variables.
    servoFlag = 0;
    day = 0;
    week = 1;

    ///// A
    // 0x01 means PORTA bit 0 is an input.
    TRISA = 0x00;
    // Initialization: all pins start as off. 0xFF for all pins on at start.
    PORTA = 0x00;
    
    ///// B
    // PORTB bit 1 is an input.
    TRISB = 0x0E; //0x0F
    PORTB = 0x00;
    
    ///// C
    // 0x01 means PORTC bit 0 is an input.
    TRISC = 0x00;
    // Initialization: all pins start as off. 0xFF for all pins initially on.
    PORTC = 0x00; 


    // Set the reported oscillator control to 4 MHz (actual: 1 MHz, since we have Fosc/4).
    /* TRIS tells you whether or not it's input or output (1 or 0), while PORT tells
     * what you're outputting if you are outputting something.
     */
    OSCCON = OSCCON | 0x70;

    //ADCON0bits.ADON = 1;
    Open1USART( USART_TX_INT_OFF &
                USART_RX_INT_OFF &
                USART_ASYNCH_MODE &
                USART_EIGHT_BIT &
                USART_BRGH_HIGH,
                25);
    putc1USART('a');


}

void initADC(void)
{
     // 10-bit Analog-to-Digital Converter Module - initial specifications.

     /* init specs for ADCON0
     * VCFG1: AVss
     * VCFG0: AVdd
     * CHS<3:0>: Channel  (Actual read channel will be selected later.)
     * GO: idle (Typically read-only. Status bit given by ADON.)
     * ADON: disabled
     */
    ADCON0 = 0x0C;
    
    /* init specs for ADCON1
     * ADFM: left justified (truncate excess 2 bits from the total of 10 bits. )
     * ADCAL: normal ad converter operation
     * ACQT<2:0>: 0 Tad
     * ADCS<2:0>: FOSC/2 (equivalent to 2 * Tosc. Since we are running at 8MHz, each Tosc is equal to .125u Hz. Each Tad (adc acquisition time, selected above) is equal to whatever we choose here.)
     */
    ADCON1 = 0x38;

    // Uninitialized registers.
    // ANCON0 = 0x0;
    // ANCON1 = 0x0;
    // ADRESH = 0x0;
    // ADRESL = 0x0;
    // ADCTRIG = 0x0;
}

void initTimers(void)
{
    // Turn off timers.
    T0CONbits.TMR0ON = 0;
    T1CONbits.TMR1ON = 0;

    // Initializing Timer 0 settings.
    // Turned off in the below code, so we can ignore this section for now.
    OpenTimer0(	TIMER_INT_ON 	& 
        	T0_16BIT	&
                T0_SOURCE_INT 	&
		T0_EDGE_RISE	&
		T0_PS_1_16);
    T0CONbits.PSA = 0; // Prescale off.

    //Initializing Timer 1 settings.
    // Changing the bits using a single word should be slightly faster than using the OpenTimer function.
    T1CON = 0x06;

    //Initializing Interrupts
    RCONbits.IPEN = 1;      //Initialize the IP

    INTCONbits.TMR0IE = 0;  //Disable TMR0 interrupt
    INTCON2bits.TMR0IP = 1; //Set Timer0 to HighP

    PIE1bits.TMR1IE = 1;    //Enable TMR1 interrupt
    IPR1bits.TMR1IP = 0;    //Set Timer1 to LowP

    INTCONbits.PEIE = 1;    //Turn on LP interrupts
    INTCONbits.GIE = 1;     //Turn on HP interrupts

    WriteTimer0(0xFFFF - 0x4E20);//dur);
    WriteTimer1(0xFFFF - 0x4E20);//invert(dur));

    // Start the timers (if 1).
    T0CONbits.TMR0ON = 0;
    T1CONbits.TMR1ON = 1;

}

void high_isr(void)
{
    INTCONbits.TMR0IF = 0;
    PIR1bits.SSP1IF = 0;
}

void low_isr(void)
{
    if(day < 100)
    {
        // Reset interrupt to trigger every .1m sec while
        WriteTimer1(WAITTIME(100));
    }
    else
    {
        // Completed week 1, but since week 2 is reserved for calculations, we have essentially completed 1 month. Reset all counters since the period is 1 month. (1 month = 200 days = 20m sec = 2 weeks).
        WriteTimer1(WAITTIME(10000));
        // Reset back to 0 days.
        day = 0;
    }

    if(0 == day)
    {
        // Turn everything on.

        // Motor.
        MOTOR1 = 1; // Set RA0 (PMOS) on.
	MOTOR2 = 0; // Set RA3 (NMOS) on.

        // Servo.
        SERVO = 1;
    }
    else if(50 == day)
    {
        // Turn Motor off.
        MOTOR1 = 0; // Set RA0 (PMOS) off.
	MOTOR2 = 1; // Set RA3 (NMOS) off.
    }
    else if(15 == day)
    {
        // Turn Servo off.
        SERVO = 0;
    }

    // Using '%' as mod causes this code to fail. 
    if(0 == (day & 0x1))
    {
        ENCODERLED = 1;
    }
    else
    {
        ENCODERLED = 0;
    }
    
    day++;
}

void InitializeUART()
{
	SPBRG = 25;				//Baud Rate 9600 for 4MHz
	TRISC = TRISC | 0xC0;	//RX & TX TRIS controls to 1
	TXSTAbits.SYNC = 0;		//asynchronous operation
	TXSTAbits.BRGH = 1;
	RCSTAbits.SPEN = 1;		//TX/CK I/O pins as output
	TXSTAbits.TX9 = 0;		//8-bit transmission
	BAUDCONbits.TXCKP = 0;	//no data polarity
	TXSTAbits.TXEN = 1;		//enables transmitter circuitry

}
void SendUART(char *c)
{
	char temp;
	int i = 0;

	do
	{
		temp = c[i++];
		TXREG = temp;
		Delay1KTCYx(5);
	} while( c[i] != '\0' );
}
void SendUARTchar(char c)
{
	TXREG = c;
	Delay1KTCYx(5);
}