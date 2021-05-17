#include <xc.h>


// **** GLOBAL VARIABLES **** //
unsigned int ADCResult=0;

// **** PROTOTYPES **** //
unsigned int ADC_Read();

// **** FUNCTIONS **** //
unsigned int ADC_Read(){
    ADCInit();
   ADCON0=0x11;
    __delay_ms(1);  
    ADCON0bits.GO=1;
    while(ADCON0bits.GO == 1);

   return ((((unsigned int)ADRESH)<<2)|(ADRESL>>6));
}