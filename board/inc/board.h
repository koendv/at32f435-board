/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-28     shelton      first version
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#include <rtthread.h>
#include "at32f435_437.h"
#include "at32_msp.h"
#include "drv_common.h"
#include "drv_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AT32_FLASH_START_ADRESS         ((uint32_t)0x08000000)
#define FLASH_PAGE_SIZE                 (4 * 1024)
#define AT32_FLASH_SIZE                 (4032 * 1024)
#define AT32_FLASH_END_ADDRESS          ((uint32_t)(AT32_FLASH_START_ADRESS + AT32_FLASH_SIZE))

/* internal sram memory size[kbytes] <384>, default: 384 without parity: 512 */
#define AT32_SRAM_SIZE                  512
#define AT32_SRAM_END                   (0x20000000 + AT32_SRAM_SIZE * 1024)

#if defined(__ARMCC_VERSION)
extern int Image$$RW_IRAM1$$ZI$$Limit;
#define HEAP_BEGIN                      ((void *)&Image$$RW_IRAM1$$ZI$$Limit)
#elif __ICCARM__
#pragma section="CSTACK"
#define HEAP_BEGIN                      (__segment_end("CSTACK"))
#else
extern int __bss_end;
#define HEAP_BEGIN                      ((void *)&__bss_end)
#endif

#define HEAP_END                        AT32_SRAM_END

/* for at32cxx package */
#define EE_TYPE                         AT24C256

/* for sdcard. suggested values: 12Mhz, 24MHz, 48MHz */
#define SDIO_MAX_FREQ                   (24000000)

void system_clock_config(void);

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H__ */
