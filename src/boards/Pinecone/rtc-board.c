#include "rtc-board.h"
#include "bflb_core.h"
#include "utilities.h"

uint64_t rtcOffset;
uint64_t rtcAlarm;
uint64_t rtcTimerContext;
bool alarmRunning = false;

uint64_t RtcGetAbsoluteNowUs() {
  return bflb_mtimer_get_time_us();
}

uint32_t RtcGetNowUs() {
  uint32_t ret = RtcGetAbsoluteNowUs() - rtcOffset;
  return ret;
}

void RtcInit() {
  rtcOffset = RtcGetAbsoluteNowUs();
}

TimerTime_t RtcTick2Ms( uint32_t tick ) {
  return tick / 1000;
}

TimerTime_t RtcTempCompensation( TimerTime_t period, float temperature ) {
  return 0;
}

uint32_t RtcMs2Tick( TimerTime_t milliseconds ) {
  return milliseconds *  1000;
}

uint32_t RtcGetTimerValue( void ) {
  return RtcGetNowUs();
}

uint32_t RtcGetMinimumTimeout( void ) {
  return 1000;
}

uint32_t RtcGetTimerElapsedTime( void ) {
  uint32_t elapsed = RtcGetNowUs() -  rtcTimerContext;
  return (elapsed);
}

void RtcStopAlarm( void ) {
    alarmRunning = false;
}

void RtcSetAlarm( uint32_t timeout ) {
  rtcAlarm = timeout;
  alarmRunning = true;
}

uint32_t RtcGetTimerContext( void ) {
  return rtcTimerContext;
}

uint32_t RtcSetTimerContext( void ) {
  rtcTimerContext = RtcGetNowUs();

  return rtcTimerContext;
}

void RtcBkupRead( uint32_t* data0, uint32_t* data1 ) {
  *data0 = RtcGetCalendarTime(data1);
}

uint32_t RtcGetCalendarTime( uint16_t *milliseconds ) {
  // TODO
  return 0;
}

void RtcBkupWrite( uint32_t data0, uint32_t data1 ) {

}

void RtcProcess( void )
{
  CRITICAL_SECTION_BEGIN( );
  if( (  alarmRunning ))
  {
    if( RtcGetTimerElapsedTime( ) >= rtcAlarm )
    {
      alarmRunning = false;
      TimerIrqHandler( );
    }
  }
  CRITICAL_SECTION_END( );
}