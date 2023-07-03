/** @file rtc.h */
#pragma once

// 1秒あたりのナノ秒数
//#define NSEC_PER_SEC	1000000000L
// RTCのMMIOの物理アドレス
#define RTC_ADDR    ((paddr_t) 0x101000)
// RTCのMMIOのサイズ
//#define RTC_SIZE    (0x1000)
// RTCの割り込み番号
//#define RTC_IRQ     11

// 現在時刻の下位ビットを取得してRTC_TIME_HIGHを更新する
#define RTC_TIME_LOW            0x00
// RTC_TIME_LOWの読み取り時の上位ビットを取得する
#define RTC_TIME_HIGH           0x04
// アラームの下位ビットを設定して有効にする
#define RTC_ALARM_LOW           0x08
// 次のアラームの上位ビットを設定する
#define RTC_ALARM_HIGH          0x0c
#define RTC_IRQ_ENABLED         0x10
#define RTC_CLEAR_ALARM         0x14
#define RTC_ALARM_STATUS        0x18
#define RTC_CLEAR_INTERRUPT     0x1c
