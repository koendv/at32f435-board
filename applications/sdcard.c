
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

static rt_thread_t sd_detect_threadid = RT_NULL;
static rt_sem_t    sd_detect_sem      = RT_NULL;

static void sd_detect(void);

#ifdef RT_USING_DFS_MNTTABLE
const struct dfs_mount_tbl mount_table[] =
    {
        {SD_DEVICE, SD_DIR, "elm", 0, 0},
        {0},
};
#endif

#if defined(RT_USING_SDIO) && defined(BSP_USING_SDIO)

/* print for debugging */
void sd_print()
{
    bool          sdcard_detected;
    bool          sdcard_mounted;
    struct statfs fs_stat;

    sdcard_detected = rt_pin_read(SD_DETECT_PIN) == PIN_LOW;
    sdcard_mounted  = dfs_statfs(SD_DIR, &fs_stat) == 0;

    LOG_I("sd card %sdetected, %smounted",
          sdcard_detected ? "" : "not ",
          sdcard_mounted ? "" : "not ");
}

/* interrupt handler */
static void sd_detect_handler(void *ptr)
{
    if (sd_detect_sem)
        rt_sem_release(sd_detect_sem);
}

/* thread */
static void sd_detect_thread()
{
    /* wait for interrupt pin to go down */
    while (1)
    {
        rt_sem_take(sd_detect_sem, RT_WAITING_FOREVER);
        LOG_I("SD card changed");
        sd_detect();
    }
}

static void sd_detect()
{
    bool          sdcard_detected;
    bool          sdcard_mounted;
    struct statfs fs_stat;

    sdcard_detected = rt_pin_read(SD_DETECT_PIN) == PIN_LOW;

    if (sdcard_detected)
        sd_mount();
    else
        sd_unmount();
}

void sd_mount()
{
    LOG_I("sd card mount");
    // switch sd card power on
    rt_pin_write(SD_ON_PIN, PIN_HIGH);
    // wait for power up
    rt_thread_mdelay(500);
    // mount sd card
    at32_mmcsd_change();
    if (mmcsd_wait_cd_changed(2 * RT_TICK_PER_SECOND) == -RT_ETIMEOUT)
        LOG_E("sd card init fail");
#ifndef RT_USING_DFS_MNTTABLE
    if (dfs_mount(SD_DEVICE, SD_DIR, "elm", 0, 0) != 0)
        LOG_E("sd card mount fail");
#endif
    sd_print();
    return;
}

void sd_unmount()
{
    LOG_I("sd card unmount");
#ifndef RT_USING_DFS_MNTTABLE
    // unmount sd card
    // in dfs_v2 use dfs_mnt_destroy() instead
    if (dfs_unmount(SD_DIR) != 0)
        LOG_E("sd card unmount fail");
#endif
    // sdcard power off
    rt_pin_write(SD_ON_PIN, PIN_LOW);
    // sdio shutdown
    at32_mmcsd_change();
    if (mmcsd_wait_cd_changed(2 * RT_TICK_PER_SECOND) == -RT_ETIMEOUT)
        LOG_E("sdcard change fail");
    sd_print();
}

/* mount sd card in background */
static void sd_mount_thread()
{
    if (rt_pin_read(SD_DETECT_PIN) == PIN_HIGH)
    {
        LOG_I("no sd card");
        return;
    }
    rt_pin_write(SD_ON_PIN, PIN_HIGH);
    rt_thread_mdelay(1000);
    sd_mount();
}

void sd_init()
{
    rt_thread_t sd_mount_threadid = RT_NULL;

    sd_detect_sem = rt_sem_create("sd detect", 0, RT_IPC_FLAG_FIFO);
    rt_sem_control(sd_detect_sem, RT_IPC_CMD_SET_VLIMIT, (void *)1);

    sd_detect_threadid = rt_thread_create("sd detect", sd_detect_thread, RT_NULL, 1024, 5, 10);
    if (sd_detect_threadid != RT_NULL)
        rt_thread_startup(sd_detect_threadid);

    rt_pin_write(SD_ON_PIN, PIN_LOW);
    rt_pin_mode(SD_ON_PIN, PIN_MODE_OUTPUT);

    rt_pin_mode(SD_DETECT_PIN, PIN_MODE_INPUT);
    rt_pin_attach_irq(SD_DETECT_PIN, PIN_IRQ_MODE_RISING_FALLING, sd_detect_handler, RT_NULL);
    rt_pin_irq_enable(SD_DETECT_PIN, PIN_IRQ_ENABLE);

    /* mount sdcard in background */
    sd_mount_threadid = rt_thread_create("sd mount", sd_mount_thread, RT_NULL, 1024, 5, 10);
    if (sd_mount_threadid != RT_NULL)
        rt_thread_startup(sd_mount_threadid);
}

MSH_CMD_EXPORT_ALIAS(sd_mount, sdmount, mount sd card);
MSH_CMD_EXPORT_ALIAS(sd_unmount, sdunmount, unmount sd card);

#endif
