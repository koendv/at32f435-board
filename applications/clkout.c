#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#include "drv_gpio.h"

/*
 * clkout_32k outputs the 32.768kHz LSE crystal on pin PA8.
 * Use to accurately measure LSE oscillator frequency.
 *
 * clkout_1m outputs the 8MHz HSE crystal on pin PC9.
 * The clock is divided by 8 to give 1MHz exactly.
 * Use to accurately measure HSE oscillator frequency.
 *
 * Note: Do not measure oscillator touching the crystal with 
 * an oscilloscope probe; the capacitance of the probe will 
 * change the oscillator frequency slightly.
 */

/*
 * code from AT32 Workbench.
 * In PinOut Configuration: CRM->Clock Output
 * In Clock Configuration: Choose lext or hext.
 * When outputting hext, set division by 8: clockoutdiv1 /2,  clockoutdiv2 /4
 */

void clkout_32k(void)
{
    gpio_init_type gpio_init_struct;

    /* enable periph clock */
    crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);

    /* set default parameter */
    gpio_default_para_init(&gpio_init_struct);
    /* config gpio */
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_out_type       = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode           = GPIO_MODE_MUX;
    gpio_init_struct.gpio_pins           = GPIO_PINS_9;
    gpio_init_struct.gpio_pull           = GPIO_PULL_NONE;
    gpio_init(GPIOC, &gpio_init_struct);
    /* config gpio mux function */
    gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE9, GPIO_MUX_0);

    /* config clkout2 output clock source */
    crm_clock_out2_set(CRM_CLKOUT2_LEXT);
    /* config clkout2 div */
    crm_clkout_div_set(CRM_CLKOUT_INDEX_2, CRM_CLKOUT_DIV1_1, CRM_CLKOUT_DIV2_1);
}

void clkout_1m(void)
{
    gpio_init_type gpio_init_struct;

    /* enable periph clock */
    crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);

    /* set default parameter */
    gpio_default_para_init(&gpio_init_struct);
    /* config gpio */
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_out_type       = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode           = GPIO_MODE_MUX;
    gpio_init_struct.gpio_pins           = GPIO_PINS_9;
    gpio_init_struct.gpio_pull           = GPIO_PULL_NONE;
    gpio_init(GPIOC, &gpio_init_struct);
    /* config gpio mux function */
    gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE9, GPIO_MUX_0);

    /* config clkout2 output clock source */
    crm_clock_out2_set(CRM_CLKOUT2_HEXT);
    /* config clkout2 div */
    crm_clkout_div_set(CRM_CLKOUT_INDEX_2, CRM_CLKOUT_DIV1_2, CRM_CLKOUT_DIV2_4);
}

MSH_CMD_EXPORT(clkout_32k, mirror 32.768kHz LSE clock on pin PC9);
MSH_CMD_EXPORT(clkout_1m, mirror 1Mhz HSE clock on pin PC9);
