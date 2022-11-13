#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define RADIO_NSS 14
#define RADIO_BUSY 0
#define RADIO_DIO_1 11
#define RADIO_RESET 17
#define SPI_MISO 1
#define SPI_MOSI 4
#define SPI_CLK 3
#define RADIO_ANT_SWITCH_POWER PA_5

/*!
 * Defines the time required for the TCXO to wakeup [ms].
 */
#define BOARD_TCXO_WAKEUP_TIME                      0

#ifdef __cplusplus
}
#endif

#endif // __BOARD_CONFIG_H__