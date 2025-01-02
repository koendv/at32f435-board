/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-05-16     shelton      first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <rtconfig.h>
#include "drv_common.h"
#include "drv_gpio.h"

#include "pinout.h"

void blink_led()
{
    rt_pin_write(LED1_PIN, PIN_LOW);
    rt_thread_mdelay(250);
    rt_pin_write(LED1_PIN, PIN_HIGH);
    rt_thread_mdelay(4750);
}

int main(void)
{
    rt_kprintf("boot\r\n");
    /* set led1 pin mode to output */
    rt_pin_mode(LED1_PIN, PIN_MODE_OUTPUT);

    while (1) blink_led();
}
