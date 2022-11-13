#include "delay-board.h"
#include "bflb_core.h"

void DelayMsMcu( uint32_t ms )
{
  bflb_mtimer_delay_ms(ms);
}