#ifndef _RTC_A_H_
#define _RTC_A_H_

#include <msp430.h>
#include "Energia.h"

#ifndef __MSP430_HAS_RTC__ 
#error "Your microcontroller have not a RTC peripheral"
#endif

typedef enum RTCSRC
{
  RTCSRC_VLO = 0,
  RTCSRC_XT1
} RTCSRC;

typedef enum RTC_MODE
{
  RTCMODE_CALENDAR = 0,
  RTCMODE_COUNTER
} RTC_MODE;

typedef enum RTCFREQ
{
  MINUTE_CHANGED = 0,
  HOUR_CHANGED,
  AT_MIDNIGHT,
  AT_NOON
} RTCFREQ;

typedef enum RTCFREQC
{
  OVERFLOW_8BIT = 0,
  OVERFLOW_16BIT,
  OVERFLOW_24BIT,
  OVERFLOW_32BIT
} RTCFREQC;

typedef enum RTCINT
{
  RTC_INT_ALARM = 0,
  RTC_INT_TIME_EVENT,
  RTC_INT_RTC,
} RTCINT;

void RTC_init(RTCSRC Source);

void RTC_StopCounter();
void RTC_StartCounter();
void RTC_ResetCounter();

void RTC_init(RTCSRC Source, uint8_t Second, uint8_t Minute, uint8_t Hour, uint8_t Day, uint8_t DOW, uint8_t Month, uint16_t Year);

uint8_t RTC_GetSecond(); 
uint8_t RTC_GetMinute(); 
uint8_t RTC_GetHour();
uint8_t RTC_GetDayOfWeek();  
uint8_t RTC_GetDay();  
uint8_t RTC_GetMonth(); 
uint16_t RTC_GetYear(); 

void RTC_SetAlarm(int8_t Minute, int8_t Hour, int8_t Day, int8_t DOW, void (*userFunc)(void));
void RTC_EnableAlarm();
void RTC_DisableAlarm();

void RTC_enableInterrupt(RTCFREQ Freq, void (*userFunc)(void)); 
void RTC_disableInterrupt(void); 

void RTC_enableRTCInterrupt(void (*userFunc)(void));
void RTC_disableRTCInterrupt(void);

void SelectSource(RTCSRC Source); 

#endif