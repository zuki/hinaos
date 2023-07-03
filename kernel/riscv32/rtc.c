/** @file rtc.c */
#include "rtc.h"
#include "asm.h"
#include <kernel/printk.h>

int32_t arch_rtc_epoch(void) {
    uint32_t low = mmio_read32_paddr(RTC_ADDR + RTC_TIME_LOW);
    int32_t high = (int32_t)mmio_read32_paddr(RTC_ADDR + RTC_TIME_HIGH);
    int64_t time = ((int64_t)high << 32) | low;
    return (int32_t)time;
}
