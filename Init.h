#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include "I2C_EEPROM.h"

#define OnButton PORTBbits.RB0
#define Up PORTBbits.RB4
#define Down PORTBbits.RB5
#define Led PORTBbits.RB1
#define Sensor PORTAbits.RA2
#define HEATER PORTCbits.RC5
#define FAN PORTCbits.RC2
#define ON 1
#define OFF 0

#define SBIT_T2CKPS1  1
#define SBIT_PS1  5
#define SBIT_PS0  4

char binary_pattern[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

// ***** PROTOTYPES ***** //
void ButtonsInit();
void LEDInit();
void SevenSegInit();
void ADCInit();
void ElementInit();
void Timer1Init();
void Timer2Init();
void OnOff_Int_Init();
void SettingModeInt();
void SysInit();

// ***** FUNCTIONS ***** //
void ButtonsInit(){
    TRISB0 = 0x01; //B0 input pin => ON/OFF button
    TRISB4 = 0x01; //B4 input pin => UP button
    TRISB5 = 0x01; //B5 input pin => DOWN button
}
void LEDInit(){
    TRISB1 = 0x00; //B1 output pin => ?Heating Element Led?
}
void SevenSegInit(){
    ADCON1 = 6;
    PORTAbits.RA4 = 0;
    TRISA4 = 0;
    PORTAbits.RA5 = 0;
    TRISA5 = 0;
    TRISD = 0; 
    PORTD = 255;
}
void ADCInit(){
    ADCON1=0x02;
    ADCON0=0x41; 
}
void ElementInit(){
    TRISC5 = 0x00;    //RC5 is output (Heater)
    TRISC2 = 0x00;    //RC2 is output (Fan)
    HEATER = OFF;
    FAN = OFF;
}
void Timer1Init(){
    T1CON = (1<<SBIT_PS1) | (1<<SBIT_PS0); // Timer0 with external freq and 8 as prescalar
    TMR1H=0x0B;     // Load the time value(0xBDC) for 100ms delay
    TMR1L=0xDC;
    TMR1IE=1;       //Enable timer interrupt bit in PIE1 register
    GIE=1;          //Enable Global Interrupt
    PEIE=1;         //Enable the Peripheral Interrupt
    TMR1ON = 1;     //Start Timer1
}
void Timer2Init(){
    T2CON = (1<<SBIT_T2CKPS1);  // Timer2 with external frequency and 16 as pre-scalar
    TMR2=100;       // Load the timer value for 500us delay
    TMR2IE=1;       //Enable timer interrupt bit in PIE1 register
    GIE=1;          //Enable Global Interrupt
    PEIE=1;         //Enable the Peripheral Interrupt
    TMR2ON = 1;
}
void OnOff_Int_Init(){
    OPTION_REG = 0x00;        
    INTCON |= 0xd0;
}
void SettingModeInt(){
    GIE=1;
    PEIE=1;
    RBIE=1;
    OPTION_REG = 0x00;
}
void SysInit(){
    ButtonsInit();
    LEDInit();
    SevenSegInit();
    ElementInit();
    Timer1Init();
    Timer2Init();
    OnOff_Int_Init();
    //SettingModeInt();
}