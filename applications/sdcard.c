
/*
 * mount / unmount sdcard.
 * - sdcard has GPT or MBR partition table.
 * - first partition is formatted in FAT32.
 * - first partition is 16gb or smaller (mmcsd max partition)
 * - menuconfig:
 *   CONFIG_RT_MMCSD_MAX_PARTITION=16
 *   CONFIG_RT_USING_DFS_MNTTABLE=y
 *   CONFIG_RT_USING_SDIO=y
 *   CONFIG_BSP_USING_SDIO=y
 *   CONFIG_BSP_USING_SDIO1=y
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

#define SD_DEVICE "sd0"
#define SD_DIR    "/sdcard"

static rt_thread_t sdcard_change_threadid = RT_NULL;
static rt_thread_t sdcard_mount_threadid  = RT_NULL;
static rt_sem_t    sdcard_change_sem      = RT_NULL;

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

/* true if sdcard is mounted */
static bool sdcard_is_mounted()
{
    /* XXX in dfs_v2: use rt_device_find() and dfs_mnt_dev_lookup() instead */
    struct statfs fs_stat;
    return dfs_statfs(SD_DIR, &fs_stat) == 0;
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
    if (sdcard_change_sem != RT_NULL)
        rt_sem_release(sdcard_change_sem);
}

/* thread */
static void sdcard_change_thread()
{
    /* wait for SD_DETECT pin to change */
    while (1)
    {
        rt_sem_take(sdcard_change_sem, RT_WAITING_FOREVER);
        LOG_I("sd card changed");
        if (sdcard_is_inserted() && !sdcard_is_mounted())
            sdcard_mount();
        else if (!sdcard_is_inserted() && sdcard_is_mounted())
            sdcard_unmount();
    }
}

void sdcard_mount()
{
    LOG_I("sd card mount");
    // switch sd card power on
    sdcard_power_on(TRUE);
    // wait for power up
    rt_thread_mdelay(300);
    // mount sd card
    at32_mmcsd_change();
    if (mmcsd_wait_cd_changed(2000) == -RT_ETIMEOUT)
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
    LOG_I("sd card unmount");
    /* XXX in dfs_v2: use dfs_mnt_destroy() instead */
#ifndef RT_USING_DFS_MNTTABLE
    // unmount sd card
    if (sdcard_is_mounted() && (dfs_unmount(SD_DIR) != 0))
        LOG_E("sd card unmount fail");
#endif
    // sdio shutdown
    at32_mmcsd_change();
    if (mmcsd_wait_cd_changed(2000) == -RT_ETIMEOUT)
        LOG_E("sd card change fail");
    // sdcard power off
    sdcard_power_on(FALSE);
    // wait for charge to drain
    rt_thread_mdelay(300);
    sd_print();
}

/* mount sdcard in background */
static void sdcard_mount_thread()
{
    if (!sdcard_is_inserted())
        return;
    LOG_I("sd card found");
    sdcard_mount();
}

void sdcard_init()
{
    sdcard_change_sem = rt_sem_create("sd change", 0, RT_IPC_FLAG_FIFO);
    rt_sem_control(sdcard_change_sem, RT_IPC_CMD_SET_VLIMIT, (void *)1);

    sdcard_change_threadid = rt_thread_create("sd change", sdcard_change_thread, RT_NULL, 1024, 5, 10);
    if (sdcard_change_threadid != RT_NULL)
        rt_thread_startup(sdcard_change_threadid);

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
