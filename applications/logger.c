/*
 * logger.
 * writes a logfile in swo format to sdcard.
 */

#include <rtthread.h>
#define DBG_TAG "LOG"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

static uint8_t    tx_buf[256];
static uint32_t   tx_len = 0;
static rt_timer_t tmr;

/* buffer already in swo format. write to logfile */
void swo_logger(uint8_t *buf, uint32_t bufsize)
{
    if (bufsize == 0) return;
    // XXX
    // needs semafore
}

static void inline check_tx_buf()
{
    if (tx_len + 5 > sizeof(tx_buf))
    {
        // write to sdcard
        swo_logger(tx_buf, tx_len);
        tx_len = 0;
    }
}

/* convert buffer to swo format and write to logfile */
void logger(uint32_t stream, uint8_t *buf, uint32_t bufsize)
{
    if ((stream > 31) || (bufsize == 0)) return;

    while (bufsize > 3)
    {
        tx_buf[tx_len++]  = stream << 3 | 0x3;
        tx_buf[tx_len++]  = *buf++;
        tx_buf[tx_len++]  = *buf++;
        tx_buf[tx_len++]  = *buf++;
        tx_buf[tx_len++]  = *buf++;
        bufsize          -= 4;
        check_tx_buf();
    }
    while (bufsize > 1)
    {
        tx_buf[tx_len++]  = stream << 3 | 0x2;
        tx_buf[tx_len++]  = *buf++;
        tx_buf[tx_len++]  = *buf++;
        bufsize          -= 2;
        check_tx_buf();
    }
    if (bufsize > 0)
    {
        tx_buf[tx_len++] = stream << 3 | 0x1;
        tx_buf[tx_len++] = *buf++;
    }
    swo_logger(tx_buf, tx_len);
    tx_len = 0;
}

static void log_timestamp()
{
    uint32_t tick  = rt_tick_get();
    uint8_t  ts[5] = {0};
    ts[0]          = 0xC0;
    ts[1]          = tick & 0xEF | 0x80;
    ts[2]          = (tick >> 7) & 0xEF | 0x80;
    ts[3]          = (tick >> 14) & 0xEF | 0x80;
    ts[4]          = (tick >> 21) & 0xEF;
    // write to logfile XXX
}

void timestamps(uint32_t msec)
{
    tmr = rt_timer_create("logger",
                          log_timestamp,
                          RT_NULL,
                          msec, RT_TIMER_FLAG_PERIODIC);
    if (tmr == RT_NULL)
    {
        LOG_E("timestamp fail");
        return;
    }
    rt_timer_start(tmr);
}
