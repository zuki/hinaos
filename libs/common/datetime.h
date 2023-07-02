#pragma once

#include <libs/common/types.h>

struct timeval {
    uint64_t tv_sec;
    uint64_t tv_nsec;
};

struct tm {
    int tm_sec;         /**< 秒 (0-60) */
    int tm_min;         /**< 分 (0-59) */
    int tm_hour;        /**< 時間 (0-23) */
    int tm_mday;        /**< 日 （0-31) */
    int tm_mon;         /**< 月 (0-11) */
    int tm_year;        /**< 年 - 1900 */
    int tm_wday;        /**< 曜日 (0-6, 日曜 = 0) */
};
