/** @file main.c */
#include <libs/user/rtc.h>
#include <libs/user/mmio.h>
#include <libs/user/ipc.h>
#include <libs/user/syscall.h>
#include <libs/user/driver.h>
#include <libs/common/types.h>
#include <libs/common/print.h>
#include <libs/common/datetime.h>

// Epoc秒数を日付に変換
//   goldfish/drivers/rtc/rtc-lib.cを一部変更
static void rtc_time_to_tm(int64_t time, struct tm *tm)
{
	unsigned int month, year;
	int days;

	days = time / 86400;
	time -= (unsigned int) days * 86400;

	/* day of the week, 1970-01-01 was a Thursday */
	tm->tm_wday = (days + 4) % 7;

	year = 1970 + days / 365;
	days -= (year - 1970) * 365
		+ LEAPS_THRU_END_OF(year - 1)
		- LEAPS_THRU_END_OF(1970 - 1);
	if (days < 0) {
		year -= 1;
		days += 365 + is_leap_year(year);
	}
	tm->tm_year = year - 1900;
	//tm->tm_yday = days + 1;

	for (month = 0; month < 11; month++) {
		int newdays;

		newdays = days - rtc_month_days(month, year);
		if (newdays < 0)
			break;
		days = newdays;
	}
	tm->tm_mon = month;
	tm->tm_mday = days + 1;

	tm->tm_hour = time / 3600;
	time -= tm->tm_hour * 3600;
	tm->tm_min = time / 60;
	tm->tm_sec = time - tm->tm_min * 60;

	//tm->tm_isdst = 0;
}

// Epocタイムの取得
static void rtc_epoch(int32_t *high, uint32_t *low) {
    ASSERT_OK(sys_epoch(high, low));
    //INFO("high: 0x%0x, low: 0x%0x", *high, *low);
}

// 現在時刻の取得
static void rtc_timeofday(struct tm *tm) {
    int32_t high;
    uint32_t low;
    int64_t time;
    rtc_epoch(&high, &low);
    time = ((int64_t)high << 32) | low;
    rtc_time_to_tm(time, tm);
}

/** @ingroup rtc
 * @brief rtcドライバのmain関数
 */
void main(void) {
    // RTCデバイスとして登録する
    ASSERT_OK(ipc_register("rtc"));
    TRACE("ready");
    while (true) {
        struct message m;
        ASSERT_OK(ipc_recv(IPC_ANY, &m));
        switch (m.type) {
            case RTC_TIMEOFDAY_MSG: {
                struct tm now;
                rtc_timeofday(&now);
                m.type = RTC_TIMEOFDAY_REPLY_MSG;
                m.rtc_timeofday_reply.year = now.tm_year + 1900;
                m.rtc_timeofday_reply.mon  = now.tm_mon + 1;
                m.rtc_timeofday_reply.day  = now.tm_mday;
                m.rtc_timeofday_reply.wday = now.tm_wday;
                m.rtc_timeofday_reply.hour = now.tm_hour;
                m.rtc_timeofday_reply.min  = now.tm_min;
                m.rtc_timeofday_reply.sec  = now.tm_sec;
                ipc_reply(m.src, &m);
                break;
            }
            case RTC_EPOCH_MSG: {
                int32_t high;
                uint32_t low;
                rtc_epoch(&high, &low);
                m.type = RTC_EPOCH_REPLY_MSG;
                m.rtc_epoch_reply.high = high;
                m.rtc_epoch_reply.low  = low;
                ipc_reply(m.src, &m);
                break;
            }
            default:
                WARN("unhandled message: %d", m.type);
                break;
        }
    }
}
