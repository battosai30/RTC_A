#include "RTC_A.h"

void setup()
{
  
  RTC_init(0,30,12,3, MONDAY,6,1987); // enable calendar mode, 12:30 3/6/1987
  
  RTC_enableRTCInterrupt(Update); // each seconde it will call Update()
  
  RTC_setAlarm(30, 18, NO_ALARM, NO_ALARM, Alarm); // each day at 18:30, Alarm will be call
  
}

void Update() {
  
  // place here the code you want to be executed each second. 
  // Typically, refresh the screen which shows the hour
  
}


void Alarm() {
  
  // place here the code you want to be executed on alarm
  // ring a bell ? :)
    
}

void loop()
{
  __bis_SR_register(LPM3_bits + GIE); // enable low power
  
}
