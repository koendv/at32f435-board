#include "board_api.h"
#include "general.h"
#include "target.h"
#include "target_internal.h"
#include "gdb_main.h"
#include "command.h"

#define DBG_TAG "TINYUF2"
//#define DBG_LVL DBG_ERR
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

extern target_controller_s gdb_controller;

// Initialize flash for DFU
void board_flash_init(void)
{
#if 0
    // XXX wait with automatically connecting until black magic platform_init()
    // XXX for the moment, attach using gdb
    if (cur_target == NULL)
    {
        TRY(EXCEPTION_ALL)
        {
            /* 
             * This is the same as typing
             * (gdb) target extended-remote /dev/ttyACM0
             * (gdb) monitor auto_scan
             * (gdb) attach 1
             */
            command_process(cur_target, "auto_scan");
            cur_target = target_attach_n(1, &gdb_controller);
        }
        CATCH()
        {
        default:
            target_list_free();
            break;
        }
    }
#endif
}

// Get size of flash
uint32_t board_flash_size(void)
{
    uint32_t retval = 128 * 1024; /* default value */

    if (cur_target && cur_target->flash)
    {
        retval = cur_target->flash->length;
        LOG_I("flash_size %d", retval);
    }
    return retval;
}

// Read from flash
void board_flash_read(uint32_t addr, void *buffer, uint32_t len)
{
    memset(buffer, 0, len);
    if (cur_target)
    {
        TRY(EXCEPTION_ALL)
        {
            LOG_I("target_mem32_read addr %d len %d", addr, len);
            target_mem32_read(cur_target, buffer, addr, len);
        }
        CATCH()
        {
        default:
            LOG_E("exception in target_mem32_read addr %d len %d", addr, len);
            break;
        }
    }
    return;
}

// Write to flash, len is uf2's payload size (often 256 bytes)
bool board_flash_write(uint32_t addr, void const *data, uint32_t len)
{
    if (cur_target && cur_target->flash)
    {
        TRY(EXCEPTION_ALL)
        {
            if (addr % cur_target->flash->blocksize == 0)
            {
                LOG_I("target_flash_erase addr %d blocksize %d", addr, cur_target->flash->blocksize);
                target_flash_erase(cur_target, addr, cur_target->flash->blocksize);
            }
            LOG_I("target_flash_write addr %d len %d", addr, len);
            target_flash_write(cur_target, addr, data, len);
        }
        CATCH()
        {
        default:
            LOG_E("exception in target_flash_write addr %d len %d", addr, len);
            break;
        }
        return true;
    }
    else
        return false;
}

// Flush/Sync flash contents
void board_flash_flush(void)
{
    if (cur_target)
    {
        TRY(EXCEPTION_ALL)
        {
            target_flash_complete(cur_target);
            target_reset(cur_target);
            LOG_I("target_flash_complete");
        }
        CATCH()
        {
        default:
            break;
        }
    }
    return;
}
