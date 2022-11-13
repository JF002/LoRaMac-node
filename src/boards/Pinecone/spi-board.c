#include "spi-board.h"
#include "bflb_core.h"
#include "bflb_gpio.h"
#include "bflb_spi.h"
#include "bl602_common.h"
#include "bl602_glb.h"
#include "board-config.h"
#include "delay.h"
#include "gpio.h"
#include "sx126x-board.h"
#include "utilities.h"

void SpiInit( Spi_t *obj, SpiId_t spiId, PinNames mosi, PinNames miso, PinNames sclk, PinNames nss )
{
  CRITICAL_SECTION_BEGIN( );

  struct bflb_device_s *gpio;
  gpio = bflb_device_get_by_name("gpio");
  bflb_gpio_init(gpio, SPI_MISO, GPIO_FUNC_SPI0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1); // SDI
  bflb_gpio_init(gpio, SPI_MOSI, GPIO_FUNC_SPI0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1); // SDO
  bflb_gpio_init(gpio, SPI_CLK, GPIO_FUNC_SPI0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1); //CLK

  GLB_GPIO_Type pins[1];
  pins[0] = (GLB_GPIO_Type)RADIO_NSS;
  GLB_GPIO_Func_Init(GPIO_FUN_SWGPIO, //  Configure the pins as GPIO
                     pins,            //  Pins to be configured
                     1);
  GLB_GPIO_OUTPUT_Enable(RADIO_NSS);
  GLB_GPIO_Write(RADIO_NSS, 1);

  GLB_Swap_SPI_0_MOSI_With_MISO(DISABLE);

  struct bflb_device_s *spi0;
  spi0 = bflb_device_get_by_name("spi0");

  struct bflb_spi_config_s spi_cfg = {
    .freq = 200 * 1000,
    .role = SPI_ROLE_MASTER,
    .mode = SPI_MODE0,
    .data_width = SPI_DATA_WIDTH_8BIT,
    .bit_order = SPI_BIT_MSB,
    .byte_order = SPI_BYTE_LSB,
    .tx_fifo_threshold = 0,
    .rx_fifo_threshold = 0,
  };
  bflb_spi_init(spi0, &spi_cfg);

  DelayMs( 10 );
  GpioInit( &SX126x.Reset, RADIO_RESET, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
  DelayMs( 20 );
  GpioInit( &SX126x.Reset, RADIO_RESET, PIN_ANALOGIC, PIN_PUSH_PULL, PIN_NO_PULL, 0 ); // internal pull-up
  DelayMs( 10 );

  CRITICAL_SECTION_END( );
}

void SpiDeInit( Spi_t *obj )
{
  // TODO
}

void SpiFormat( Spi_t *obj, int8_t bits, int8_t cpol, int8_t cpha, int8_t slave )
{
  // TODO
}

void SpiFrequency( Spi_t *obj, uint32_t hz )
{
  // TODO
}

uint16_t SpiInOut( Spi_t *obj, uint16_t outData )
{
  struct bflb_device_s *spi0;
  spi0 = bflb_device_get_by_name("spi0");

  uint8_t rx = 0;
  bflb_spi_poll_exchange(spi0, &outData, &rx, 1);
  return rx;
}
