#include <rtthread.h>
#include <dev_spi_flash.h>
#include <dev_spi_flash_sfud.h>
#include <dfs_fs.h>
#include <dfs_romfs.h>
#include <drv_spi.h>
#include <fal.h>
#include <fal_cfg.h>
#include "sdcard.h"
#include "pinout.h"

#undef DBG_TAG
#define DBG_TAG "MOUNT"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

/* Note:
 * if W25Q128 gives error
 * "The sector size of device is greater than the sector size of FAT."
 * then add to .config:
 * CONFIG_RT_DFS_ELM_MAX_SECTOR_SIZE=4096
 */

static const struct romfs_dirent _romfs_root[] =
    {
        {ROMFS_DIRENT_DIR,  "flash", RT_NULL, 0},
        {ROMFS_DIRENT_DIR, "sdcard", RT_NULL, 0},
};

const struct romfs_dirent romfs_root =
    {
        ROMFS_DIRENT_DIR,
        "/",
        (rt_uint8_t *)_romfs_root,
        sizeof(_romfs_root) / sizeof(_romfs_root[0]),
};

int mount_romfs()
{
    if (dfs_mount(RT_NULL, "/", "rom", 0, &(romfs_root)) != 0)
    {
        LOG_E("rom mount on '/' failed");
        return -RT_ERROR;
    }
    LOG_I("rom mount on '/' ok");
    return RT_EOK;
}

int mount_spiflash()
{
    struct rt_device *flash_dev = RT_NULL;

    rt_hw_spi_device_attach("spi2", "spi20", GPIOB, GPIO_PINS_12); // XXX fixme
    if (rt_sfud_flash_probe("norflash0", "spi20") == RT_NULL)
    {
        LOG_E("sfud flash probe fail");
        return -RT_ERROR;
    }
    fal_init();
    flash_dev = fal_blk_device_create(PARTITION1_NAME);
    if (flash_dev == RT_NULL)
    {
        LOG_E("fal block device fail");
        return -RT_ERROR;
    }

    if (dfs_mount(flash_dev->parent.name, "/flash", "elm", 0, 0) != 0)
    {
        LOG_I("Please format spi flash with mkfs -t elm");
#if 0
        LOG_I("formatting spi flash");
        if (dfs_mkfs("elm", flash_dev->parent.name) != 0)
        {
            LOG_I("spi flash format fail");
            return -RT_ERROR;
        }
        if (dfs_mount(flash_dev->parent.name, "/flash", "elm", 0, 0) != 0)
        {
            LOG_I("spi flash mount fail");
            return -RT_ERROR;
        }
#endif
    }
    LOG_I("spi flash mount on /flash ok");
    return RT_EOK;
}

/* mount all filesystems */
int mount_fs(void)
{
    if (mount_romfs() != RT_EOK) return -RT_ERROR;
    mount_spiflash();
#if defined(RT_USING_SDIO) && defined(BSP_USING_SDIO)
    sdcard_init();
#endif
}

INIT_APP_EXPORT(mount_fs);
