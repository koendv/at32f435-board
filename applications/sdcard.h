#ifndef SDCARD_H_
#define SDCARD_H_

void      sdcard_init(void);
void      sdcard_mount(void);
void      sdcard_unmount(void);
rt_bool_t sdcard_is_inserted(void);
rt_bool_t sdcard_is_mounted(void);
void      sdcard_power_on(rt_bool_t power);

#endif
