/*
JosTask lets the build in LED blink, just for testing purpose and to get experience 
in programming these libraries.The blinking led does not 
influence the general functioning of ESP32NMEA2000 progam (I hope).
*/
#include "GwApi.h"
#include "GwJostask.h"

#define LED_BUILTIN 19

DECLARE_USERTASK(Jostask);   
//DECLARE_USERTASK_PARAM(Jostask,4000);

// an init function that ist being called before other initializations from the core
void JosInit(GwApi *api){
   api->getLogger()->logDebug(GwLog::LOG,"Jos init run");
   pinMode(LED_BUILTIN, OUTPUT);           // initialize digital pin LED_BUILTIN as an output.
    digitalWrite(LED_BUILTIN, HIGH);
 }


void Jostask(GwApi *api)
  {
    GwLog *logger=api->getLogger();
    //initialization goes here
    int ledState = LOW;                     // ledState used to set the LED
    unsigned long previousMillis = 0;       // will store last time LED was updated
    int interval= 1000;                     // interval at which to change the led (milliseconds)
    pinMode(LED_BUILTIN, OUTPUT);           // initialize digital pin LED_BUILTIN as an output.
    digitalWrite(LED_BUILTIN, LOW);
    

  while (1)
  {
    delay(1); //js necessary in order to change over to other tasks !!!!!!!!!!!!!!!
    
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) 
    {
      // save the last time you changed the LED
      previousMillis = currentMillis;
      // if the LED is off turn it on and vice-versa:
      if (ledState == LOW) 
      {
        interval = 100;
        ledState = HIGH;
      } else 
      {
        interval = 3537;
        ledState = LOW;
      }
      // set the LED with the ledState of the variable:
      digitalWrite(LED_BUILTIN, ledState);
    } 
    
  }  
  vTaskDelete(NULL);
}
