# patches

- at32f435_437_can.patch
    [PR](https://github.com/RT-Thread/rt-thread/pull/9611) duplicate names CAN_RX_FIFO0, CAN_RX_FIFO1 in at32f435_437_can.h and rt-thread/components/drivers/include/drivers/can.h

- lvgl-9.1.0-event-double-clicked.patch

    The CST816T touch panel is able to detect double clicks. Add LV_EVENT_DOUBLE_CLICKED to list of events.  This event may become part of lvgl, see [PR](https://githubissues.com/lvgl/lvgl/5351).
    
- lvgl-9.1.0-rtthread.patch

    [PR](https://github.com/lvgl/lvgl/pull/6667) fix st7789 display driver hang
    Also, put [mutex](https://docs.lvgl.io/master/porting/os.html) lv_lock()/lv_unlock() mutex around lvgl task. 

- spi_no_busy_wait.patch

    Releases the cpu to another task when waiting for an spi read or write.
    This allows writing pixels to the display and calculating pixels at the same time.
    This patch is tentative.

- dfs_pcache.patch

    Allows compiling dfs_v2 on systems without memory management unit. (no mmu.h)

