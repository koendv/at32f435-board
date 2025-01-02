#ifndef PINOUT_H
#define PINOUT_H

#include <rtthread.h>

#define LED0_PIN             LED1_PIN
#define TARGET_SWCLK_DIR_PIN GET_PIN(A, 2)
#define TARGET_SWDIO_DIR_PIN GET_PIN(A, 3)
#define TARGET_SWCLK_PIN     GET_PIN(A, 4)
#define TARGET_SWDIO_PIN     GET_PIN(A, 5)
#define TARGET_TDI_PIN       GET_PIN(A, 6)
#define TARGET_SWO_PIN       GET_PIN(A, 7)
#define TARGET_RST_IN_PIN    GET_PIN(C, 4)
#define TARGET_RST_PIN       GET_PIN(C, 5)

#define TOUCH_INT_PIN        GET_PIN(C, 13)
#define TOUCH_RST_PIN        GET_PIN(C, 0)
#define DISP_RST_PIN         GET_PIN(C, 1)
#define ADC_LIGHT_PIN        GET_PIN(C, 2)
#define DISP_CS_PIN          GET_PIN(C, 3)
#define FLASH_CS_PIN         GET_PIN(B, 12)
#define SPI2_CLK_PIN         GET_PIN(B, 13)
#define SPI2_MISO_PIN        GET_PIN(B, 14)
#define SPI2_MOSI_PIN        GET_PIN(B, 15)
#define LED1_PIN             GET_PIN(C, 6)
#define SD_ON_PIN            GET_PIN(C, 7)
#define SD_D0_PIN            GET_PIN(C, 8)
#define SD_D1_PIN            GET_PIN(C, 9)
#define DISP_DC_PIN          GET_PIN(A, 8)
#define CONSOLE_TXD_PIN      GET_PIN(A, 9)
#define CONSOLE_RXD_PIN      GET_PIN(A, 10)
#define SD_DETECT_PIN        GET_PIN(A, 15)
#define SD_D2_PIN            GET_PIN(C, 10)
#define SD_D3_PIN            GET_PIN(C, 11)
#define SD_CLK_PIN           GET_PIN(C, 12)
#define SD_CMD_PIN           GET_PIN(D, 2)
#define DISP_SPI_SCK_PIN     GET_PIN(B, 3)
#define DISP_LED_PIN         GET_PIN(B, 4)
#define DISP_SPI_MOSI_PIN    GET_PIN(B, 5)
#define DISP_SCL_PIN         GET_PIN(B, 6)
#define DISP_SDA_PIN         GET_PIN(B, 7)
#define CAN1_RX_PIN          GET_PIN(B, 8)
#define CAN1_TX_PIN          GET_PIN(B, 9)

#endif
