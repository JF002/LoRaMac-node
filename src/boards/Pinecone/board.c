#include "board.h"
#include "bflb_irq.h"
#include "bl602_common.h"
#include "bl602_glb.h"
#include "delay.h"
#include "mcu/bl602/bl_mcu_sdk/bsp/board/bl602dk/board.h"
#include "rtc-board.h"
#include "sx126x-board.h"

void EepromMcuInit(void);

uint8_t uniqueId[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void BoardInitMcu( void ) {
  board_init();
  EepromMcuInit();
  SX126xIoInit();
  SpiInit( &SX126x.Spi, SPI_1, 0, 0, 0, NC );
  RtcInit();
}

void BoardResetMcu( void ) {
  GLB_SW_POR_Reset();
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
  //EF_Ctrl_Read_Chip_ID(id); // Not (yet?) available

  id[0] = uniqueId[0];
  id[1] = uniqueId[1];
  id[2] = uniqueId[2];
  id[3] = uniqueId[3];
  id[4] = uniqueId[4];
  id[5] = uniqueId[5];
  id[6] = uniqueId[6];
  id[7] = uniqueId[7];
}

void BoardLowPowerHandler( void ) {
}

void BoardCriticalSectionBegin( uint32_t *mask ) {
  *mask = bflb_irq_save();
  __disable_irq();
}

void BoardCriticalSectionEnd( uint32_t *mask ) {
  bflb_irq_restore(*mask);
}
