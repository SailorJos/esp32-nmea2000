/*
JosTask.h is for the JosTask.ccp and 
for defining the ports etc for the ttgo-t7-v14-mini32 board.
*/


#pragma once
#include "GwApi.h"


void Jostask(GwApi *param);
void JosInit(GwApi *param);
DECLARE_USERTASK(Jostask);
//DECLARE_USERTASK_PARAM(Jostask,4000);
DECLARE_INITFUNCTION(JosInit);

//we only compile for some boards
#ifdef ttgo-t7-v14-mini32

    #define ESP32_CAN_TX_PIN 4    
    #define ESP32_CAN_RX_PIN 33  

    #define USBSerial  Serial      //js ??

    #define GWSERIAL_RX 22   
    #define GWSERIAL_TX 21 
    #define GWSERIAL_MODE "BI"

    #define GWSERIAL2_RX 32    
    #define GWSERIAL2_TX 23     
    #define GWSERIAL2_MODE "BI"

    #define GWIIC_SCL 26
    #define GWIIC_SDA 18

    //example for a user defined help url that will be shown when clicking the help button
    DECLARE_STRING_CAPABILITY(HELP_URL,"https://www.wellenvogel.de");
#endif
