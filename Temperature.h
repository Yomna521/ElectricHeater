#include "Init.h"
#include "ADC.h"

// ****** GLOBAL VARIABLES ****** //
unsigned int Temp = 60;     //initial temperature is 60
unsigned int currentTemp;
unsigned int averageTemp = 0;
unsigned int Address = 0x0020; // Some Random Address
float Readings[10];
int i = 0, x = 0, s = 0;    //indeces

// ***** PROTOTYPES ****** //
void TempDisplay();
void CurrentTempDisplay();
int  TempSet();
void SaveTemp();
void FetchTemp();
void AverageTemp();
void TempSensor();
void SettingMode();
void HeatingCoolingElement();

// ***** FUNCTIONS **** //
void SaveTemp() {
	I2C_Master_Init(100000);
    EEPROM_Write(Address, Temp, 2);
}
void FetchTemp() {
    EEPROM_Read(Address, Temp, 2);
}
void TempDisplay(){    
    unsigned int a,b;
    if(s==0){
        SaveTemp();
        s = 1;
    }
    FetchTemp();
    a = ((Temp/10)%10);  // holds 10th digit
    b = (Temp%10);  // holds unit digit value
    
    RA4 = ON;   //  turn on the display unit
    PORTD = binary_pattern[a]; // send 10's place data to first digit
    __delay_ms(1);
    RA4 = OFF;   //  turn off the display unit
    RA5 = ON;    //  turn on the display unit
    PORTD = binary_pattern[b];  // send unit's place data to second digit
    __delay_ms(1);
    RA5 = OFF;   //  turn off the display unit
}
void CurrentTempDisplay(){     
    unsigned int a,b;
    a = ((currentTemp/10)%10);  // holds 10th digit
    b = (currentTemp%10);  // holds unit digit value
    
    RA4 = ON;   //  turn on third display unit
    PORTD = binary_pattern[a]; // send 10's place data to first digit
    __delay_ms(1);
    RA4 = OFF;   //  turn off third display unit
    RA5 = ON;    //  turn on fourth display unit
    PORTD = binary_pattern[b];  // send unit's place data to second digit
    __delay_ms(1);
    RA5 = OFF;   //  turn off fourth display unit
}
int  TempSet(){
    if(Up == 0){
        __delay_ms(5);
        if(Temp < 75){
            __delay_ms(5);
            Temp = Temp + 5;
            __delay_ms(5);
        }
        return 1;
    }
    if(Down == 0){
        __delay_ms(5);
        if(Temp > 35){
            __delay_ms(5);
            Temp = Temp - 5;
            __delay_ms(5);
        }
        return 1;
    }
    return 0;
}
void AverageTemp(){
   float reading = currentTemp;
   float tmp = 0;
   if(i<10){				//i is index for the first ten readings
        Readings[i] = reading;
        i++;
        for(int j = 0 ; j < (i+1) ; j++)
            tmp += Readings[j];
        averageTemp = (tmp/i);
    }
    else if(x<10){  //x is index for the rest of readings
        Readings[x] = reading;
        x++;
        for(int j = 0 ; j < 10 ; j++)
            tmp += Readings[j];
        averageTemp = (tmp/10);
    }
    else if(x >= 10){
        Readings[0] = reading;
        x = 1;
        for(int j = 0 ; j < 10 ; j++)
            tmp += Readings[j];
        averageTemp = (tmp/10);
    }
   HeatingCoolingElement();    //Compare the new avg temp with the reference temp
}
void TempSensor(){
    ADCResult = ADC_Read();                        //Read the converted data
    currentTemp = (ADCResult*5.0) / 10.24;                //convert data into temperature (LM35 produces 10mV per degree)
    AverageTemp();
}
void SettingMode(){
    GIE = 0;
    HEATER = OFF,
    FAN = OFF;
    TempDisplay();
    __delay_ms(10);
    int y = 0;    //index for the 5 second period
    
    while(y<5){
        for(int k= 0;k<100;k++ ){
            TempDisplay();
            int z = TempSet();
            if(z)
                y=0;
        }
        __delay_ms(100);
        RA4 = OFF;
        RA5 = OFF;
        y++;
    }
    GIE = 1;
     HeatingCoolingElement();
}
void HeatingCoolingElement(){
    //Compare reference Temp with current Temp
    if ((Temp - averageTemp) >= 5){
        HEATER = ON;  
        FAN = OFF;
        TMR2ON =ON;   //Timer on .. blinking on
    }
    else if ((averageTemp - Temp) >= 5){
        HEATER = OFF,
        FAN = ON;
        TMR2ON =OFF;  // Timer off ... blinking off
        Led = ON;
    }
    else if ((averageTemp - Temp) <= 5 || (Temp - averageTemp) >= 5){
        HEATER = OFF,
        FAN = OFF;
        TMR2ON =OFF;  // Timer off ... blinking off
        Led = OFF;
    }
    __delay_ms(10);
}
