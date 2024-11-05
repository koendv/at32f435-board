/*
 * note:
 * board.h contains
 * #define EE_TYPE AT24C256
 * please modify EE_TYPE in board.h to the model you are using.
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "at24cxx.h"

#define DBG_TAG "EEPROM"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#define I2C_BUS "i2c1"

/* puts 0x55 in last byte of eeprom */
extern rt_err_t at24cxx_check(at24cxx_device_t dev);

const char *at24cxx_name[] = {
    "AT24C01",
    "AT24C02",
    "AT24C04",
    "AT24C08",
    "AT24C16",
    "AT24C32",
    "AT24C64",
    "AT24C128",
    "AT24C256",
    "AT24C512",
};

static at24cxx_device_t eeprom1_dev = RT_NULL;

static int eeprom_check()
{
    const char *name;
    if (AT24CTYPE != sizeof(at24cxx_name) / sizeof(at24cxx_name[0]))
        LOG_E("Please update eeprom list");

    if (EE_TYPE < AT24CTYPE)
        name = at24cxx_name[EE_TYPE];
    else
    {
        LOG_E("Check EE_TYPE");
        name = "?";
    }

    eeprom1_dev = at24cxx_init(I2C_BUS, 0x50);
    if (eeprom1_dev == RT_NULL)
    {
        LOG_E("%s eeprom init fail", name);
        return -RT_ERROR;
    }

    if (at24cxx_check(eeprom1_dev) == RT_EOK)
    {
        LOG_I("%s eeprom ok", name);
        return RT_EOK;
    }
    else
    {
        LOG_W("%s eeprom fail", name);
        return -RT_ERROR;
    }
}

INIT_COMPONENT_EXPORT(eeprom_check);
