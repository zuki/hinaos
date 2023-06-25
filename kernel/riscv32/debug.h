/** @file debug.h */
#pragma once
#include <libs/common/types.h>

/** @ingroup kernerl_riscv32
 * @def STACK_CANARY_VALUE
 * @brief スタックカナリー (stack canary) の値.
 * この値が書き換えられていたらスタックオーバーフローが
 * 発生していると判断する。 */
#define STACK_CANARY_VALUE 0xdeadca71

void stack_check(void);
void stack_set_canary(uint32_t sp_bottom);
void stack_reset_current_canary(void);
