
/*
 * mount / unmount sdcard.
 * - sdcard has GPT or MBR partition table.
 * - first partition is formatted in FAT32.
 * - menuconfig:
 *   CONFIG_RT_MMCSD_MAX_PARTITION=16
 *   CONFIG_RT_USING_DFS_MNTTABLE=y
 *   CONFIG_RT_USING_SDIO=y
 *   CONFIG_BSP_USING_SDIO=y
 *   CONFIG_BSP_USING_SDIO1=y
 *
 * SD_DETECT_PIN is low when an sd card is present.
 * Setting SD_ON_PIN high switches sd card power on.
 *
 * when SD_DETECT_PIN changes, interrupt handler sdcard_change_handler() is called.
 * the interrupt handler starts a 100ms timer to debounce the contact.
 * when the timer expires the function sdcard_change_thread() is called.
 * sdcard_change_thread() switches sd card power on or off, mounts or unmounts the file system.
 */

#include <stdbool.h>
#include <rtthread.h>
#include <dfs_fs.h>
#include <drv_gpio.h>
#include <drv_sdio.h>
#include "pinout.h"
#include "sdcard.h"

#undef DBG_TAG
#define DBG_TAG "SDCARD"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#define SD_DEVICE         "sd0"
#define SD_DIR            "/sdcard"
#define SD_DEBOUNCE_TICKS (RT_TICK_PER_SECOND / 10)
#define SD_TIMEOUT        2000

static rt_thread_t sdcard_mount_threadid = RT_NULL;
static rt_timer_t  sdcard_change_tim     = RT_NULL;

#ifdef RT_USING_DFS_MNTTABLE
const struct dfs_mount_tbl mount_table[] =
    {
        {SD_DEVICE, SD_DIR, "elm", 0, 0},
        {0},
};
#endif

#if defined(RT_USING_SDIO) && defined(BSP_USING_SDIO)

/* switch sdcard power on / off */
static void sdcard_power_on(bool power)
{
    rt_pin_write(SD_ON_PIN, power ? PIN_HIGH : PIN_LOW);
}

/* true if sdcard is present */
static bool sdcard_is_inserted()
{
    return rt_pin_read(SD_DETECT_PIN) == PIN_LOW;
}

/* true if sd0 is mounted on /sdcard */
static bool sdcard_is_mounted()
{
    /* XXX in dfs_v2: use dfs_mnt_dev_lookup() instead */
    rt_device_t   dev_id = RT_NULL;
    struct statfs fs_stat;

    dev_id = rt_device_find(SD_DEVICE);

    return (dev_id != RT_NULL) && (dfs_statfs(SD_DIR, &fs_stat) == 0);
}

/* print for debugging */
static void sd_print()
{
    LOG_I("sd card %sinserted, %smounted",
          sdcard_is_inserted() ? "" : "not ",
          sdcard_is_mounted() ? "" : "not ");
}

/* interrupt handler */
static void sdcard_change_handler(void *ptr)
{
    if (sdcard_change_tim != RT_NULL)
        rt_timer_start(sdcard_change_tim);
}

/* thread */
static void sdcard_change_thread(void *parameter)
{
    (void)parameter;
    /* mount or unmount card */
    if (sdcard_is_inserted())
    {
        LOG_I("sd card inserted");
        sdcard_mount();
    }
    else
    {
        LOG_I("sd card removed");
        sdcard_unmount();
    }
    rt_timer_stop(sdcard_change_tim);
}

void sdcard_mount()
{
    // switch sd card power on
    sdcard_power_on(TRUE);
    // wait for power up
    rt_thread_mdelay(300);
    // mount sd card
    at32_mmcsd_change();
    if (mmcsd_wait_cd_changed(SD_TIMEOUT) == -RT_ETIMEOUT)
        LOG_E("sd card init fail");
#ifndef RT_USING_DFS_MNTTABLE
    if (dfs_mount(SD_DEVICE, SD_DIR, "elm", 0, 0) != 0)
        LOG_E("sd card mount fail");
#endif
    sd_print();
    return;
}

void sdcard_unmount()
{
    /* XXX in dfs_v2: use dfs_mnt_destroy() */
#ifndef RT_USING_DFS_MNTTABLE
    // unmount sd card
    if (sdcard_is_mounted() && (dfs_unmount(SD_DIR) != 0))
        LOG_E("sd card unmount fail");
#endif
    // sdio shutdown
    at32_mmcsd_change();
    if (mmcsd_wait_cd_changed(SD_TIMEOUT) == -RT_ETIMEOUT)
        LOG_E("sd card change fail");
    // sdcard power off
    sdcard_power_on(FALSE);
    sd_print();
    return;
}

/* mount sdcard in background */
static void sdcard_mount_thread()
{
    if (!sdcard_is_inserted())
        return;
    sdcard_mount();
}

void sdcard_init()
{
    sdcard_change_tim = rt_timer_create("sdcard",
                                        sdcard_change_thread,
                                        RT_NULL,
                                        SD_DEBOUNCE_TICKS,
                                        RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);

    rt_pin_write(SD_ON_PIN, PIN_LOW);
    rt_pin_mode(SD_ON_PIN, PIN_MODE_OUTPUT);

    rt_pin_mode(SD_DETECT_PIN, PIN_MODE_INPUT);
    rt_pin_attach_irq(SD_DETECT_PIN, PIN_IRQ_MODE_RISING_FALLING, sdcard_change_handler, RT_NULL);
    rt_pin_irq_enable(SD_DETECT_PIN, PIN_IRQ_ENABLE);

    /* mount sdcard in background */
    sdcard_mount_threadid = rt_thread_create("sd mount", sdcard_mount_thread, RT_NULL, 1024, 5, 10);
    if (sdcard_mount_threadid != RT_NULL)
        rt_thread_startup(sdcard_mount_threadid);
}

//INIT_DEVICE_EXPORT(sdcard_init);

MSH_CMD_EXPORT_ALIAS(sdcard_mount, sdmount, mount sd card);
MSH_CMD_EXPORT_ALIAS(sdcard_unmount, sdunmount, unmount sd card);

#endif
