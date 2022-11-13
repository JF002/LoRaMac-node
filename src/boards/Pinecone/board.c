#include "board.h"
#include "rtc-board.h"
#include "sx126x-board.h"
#include <stddef.h>
#include "delay.h"
#include "mcu/bl602/bl_mcu_sdk/bsp/board/bl602dk/board.h"

void BoardInitMcu( void ) {
  board_init();
  SX126xIoInit();
  SpiInit( &SX126x.Spi, SPI_1, 0, 0, 0, NC );
  RtcInit();
}

void BoardResetMcu( void ) {
}

void BoardInitPeriph( void ) {
}

uint8_t BoardGetBatteryLevel( void ) {
  return 0;
}

uint32_t BoardGetRandomSeed( void ) {
  return 0;
}

void BoardGetUniqueId( uint8_t *id ) {
  id = NULL;
}

void BoardLowPowerHandler( void ) {
}

void BoardCriticalSectionBegin( uint32_t *mask ) {
}

void BoardCriticalSectionEnd( uint32_t *mask ) {
}
