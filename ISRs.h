#include "Temperature.h"

// ****** GLOBAL VARIABLES ****** //
int value = 0;
int count = 0;
int Flag = 0;

// ***** PROTOTYPES ****** //
void __interrupt() ISR();

// ***** FUNCTIONS **** //
void __interrupt() ISR(){
    //Sensor ISR
    if(TMR1IF==1){
        TempSensor();
        TMR1H=0x0B;                 // Load the time value(0xBDC) for 100ms delay
        TMR1L=0xDC;
        TMR1IF=0;       // Clear timer interrupt flag
        GIE = 1;        //Enable global interrupt
    }
    
    //LED ISR
    if(TMR2IF==1){
        TMR2 = 101;
        if(count>=200){ //5ms *200 = 1000 ms = 1sec
            count=0;
            value=~value;
            Led = value;
        }
        else{
            count++;  // Keep incrementing the count till it reaches 200 to generate 1sec delay           
        }
        TMR2IF=0;       // Clear timer interrupt flag
        GIE = 1;
    }
    
     //ON/OFF Button ISR
    if(INTF==1){
        Flag = ~Flag;
        if(Flag){
            TMR1ON=OFF;
            TMR2ON =OFF;
            PORTA=OFF;
            PORTB=OFF;
            PORTC=OFF;
            PORTD=OFF;
            INTF=0;
            GIE = 1;
            SLEEP();
        }
        else if(!Flag){
            TMR1ON=ON;
            TMR2ON =ON;
            PORTA=ON;
            PORTB=ON;
            PORTC=ON;
            PORTD=ON;
        }
        INTF=0;
        GIE = 1;
    }
    /*if(RBIF==1){
        RBIE=0;
        TMR1ON=OFF;
        TMR2ON =OFF;
        SettingMode();
        TMR1ON=ON;
        TMR2ON =ON;
        RBIE=1;
        RBIF=0;
        GIE = 1;
    }
    if(Up == 0 || Down == 0){   
            __delay_ms(10);
            SettingMode();
        }*/
}
