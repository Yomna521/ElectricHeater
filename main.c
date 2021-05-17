#include "ISRs.h"

void main(){
    SysInit();
    while(1){ 
        CurrentTempDisplay();
        if(Up == 0 || Down == 0){          
            SettingMode();
        }
    }
}
