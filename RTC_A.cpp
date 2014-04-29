#include "RTC_A.h"
//#include <inttypes.h>
//#include <stdio.h>

#include "wiring_private.h"

static RTC_MODE _Mode;
static volatile voidFuncPtr intUserFunc[3];

void RTC_init(RTCSRC Source)
 {
  
  _Mode = RTCMODE_COUNTER;
  
  // if(_Mode==RTCMODE_CALENDAR) RTCCTL1 = RTCMODE + RTCHOLD ;
  // if(_Mode==RTCMODE_COUNTER) RTCCTL1 = RTCHOLD ;
// Enable 32kHz ACLK
                  // Select XIN, XOUT on P5.0 and P5.1
             // XT1 On, Highest drive strength
    UCSCTL6 |= XCAP_3;          // Internal load cap

	/////RTCCTL1 = BIT5;
    //RTCCTL1 = RTCMODE + RTCHOLD ;
	//RTCCTL01 &= ~RTCHOLD;  
	//RTCCTL1&= ~RTCHOLD;
}

void RTC_init(RTCSRC Source, uint8_t Second, uint8_t Minute, uint8_t Hour, uint8_t Day, uint8_t DOW, uint8_t Month, uint16_t Year) {

_Mode = RTCMODE_CALENDAR;

SelectSource(Source); // select clock source 

RTCCTL1 = BIT5; // enable calendar mode

RTCSEC = Second;
RTCMIN = Minute;
RTCHOUR = Hour;
RTCDAY = Day;
RTCDOW = DOW;
RTCMON = Month;
RTCYEARL = Year & 0xFF;
RTCYEARH = Year >> 8;
}

void SelectSource(RTCSRC Source) {

switch(Source) {

case RTCSRC_VLO:
break;

case RTCSRC_XT1:
P5SEL |= 0x03;
UCSCTL6 &= ~XT1OFF;
UCSCTL6 |= XCAP_3; 
break;

}

}

uint8_t RTC_GetSecond() {

if(_Mode==RTCMODE_CALENDAR) {
while(RTCCTL1 & RTCRDY); // wait for a safe reading
return RTCSEC;
} else return 0;

}

uint8_t RTC_GetMinute() {

if(_Mode==RTCMODE_CALENDAR) {
while(RTCCTL1 & RTCRDY); // wait for a safe reading
return RTCMIN;
} else return 0;

}

uint8_t RTC_GetHour() {

if(_Mode==RTCMODE_CALENDAR) {
while(RTCCTL1 & RTCRDY); // wait for a safe reading
return RTCHOUR;
} else return 0;
}

uint8_t RTC_GetDay() {

if(_Mode==RTCMODE_CALENDAR) {
while(RTCCTL1 & RTCRDY); // wait for a safe reading
return RTCDAY;
} else return 0;

}

uint8_t RTC_GetDayOfWeek() {

if(_Mode==RTCMODE_CALENDAR) {
while(RTCCTL1 & RTCRDY); // wait for a safe reading
return RTCDOW;
} else return 0;

}

uint8_t RTC_GetMonth() {

if(_Mode==RTCMODE_CALENDAR) {
while(RTCCTL1 & RTCRDY); // wait for a safe reading
return RTCMON;
}else return 0;

}

uint16_t RTC_GetYear() {

if(_Mode==RTCMODE_CALENDAR) {
while(RTCCTL1 & RTCRDY); // wait for a safe reading
return RTCYEARL + RTCYEARH<<8;
} else return 0;

}

void RTC_SetAlarm(int8_t Minute, int8_t Hour, int8_t Day, int8_t DOW, void (*userFunc)(void))
{

intUserFunc[RTC_INT_ALARM]=userFunc;

if (Minute!=-1) {
constrain(Minute,0,59);
RTCAMIN = Minute | 0x80;
} else RTCAMIN = 0;

if (Hour!=-1) {
constrain(Hour,0,23);
RTCAHOUR = Hour | 0x80;
} else RTCAHOUR = 0;

if (DOW!=-1) {
constrain(DOW,0,6);
RTCADOW = DOW | 0x80;
} else RTCADOW = 0;

if (Day!=-1) {
constrain(Day,1,31);
RTCADAY = Day | 0x80;
} else  RTCADAY = 0;
}

void RTC_EnableAlarm(){

RTCCTL0|= RTCAIE;

}

void RTC_DisableAlarm(){

RTCCTL0 &= ~RTCAIE;

}

void RTC_enableInterrupt(RTCFREQ Freq, void (*userFunc)(void)) {

intUserFunc[RTC_INT_TIME_EVENT]=userFunc;
RTCCTL1 = (RTCCTL1&0xFC) | Freq; // configure interrupt frequency
RTCCTL0|= RTCTEVIE;

}

void RTC_disableInterrupt(void){
RTCCTL0&= ~RTCTEVIE;
}

void RTC_enableRTCInterrupt(void (*userFunc)(void)) {

intUserFunc[RTC_INT_RTC]=userFunc;
RTCCTL0|= RTCRDYIE;

}

void RTC_disableRTCInterrupt(void){

RTCCTL0&= ~RTCRDYIE;

}

  __attribute__((interrupt(RTC_A_VECTOR)))
void RTC_int(void)
{

volatile uint8_t Value = RTCIV;

 if(Value & RTCIV_RTCRDYIFG && intUserFunc[RTC_INT_RTC]) intUserFunc[RTC_INT_RTC]();
 if(Value & RTCIV_RTCTEVIFG && intUserFunc[RTC_INT_TIME_EVENT]) intUserFunc[RTC_INT_TIME_EVENT]();
 if(Value & RTCIV_RTCAIFG && intUserFunc[RTC_INT_ALARM]) intUserFunc[RTC_INT_ALARM](); 

}  
