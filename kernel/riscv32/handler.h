/** @file handler.h */
#pragma once

/** @ingroup kernel_riscv32
 * @fn riscv32_trap_handler
 * @brief 割り込みハンドラ.
 * 割り込みは自動的に無効化され、S-modeで実行される。
 */
void riscv32_trap_handler(void);
/** @ingroup kernel_riscv32
 * @fn riscv32_timer_handler
 * @brief タイマー割り込みハンドラ. M-modeで実行される。
 */
void riscv32_timer_handler(void);
