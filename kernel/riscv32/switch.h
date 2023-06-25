/** @file switch.h */
#pragma once
#include <libs/common/types.h>

/** @ingroup kernel_riscv32
 * @fn riscv32_task_switch
 * @brief 実行コンテキストを切り替える.
 * @param prev_sp 現在実行中のタスクのスタックポインタ
 * @param next_sp 次のタスクのスタックポインタ
 */
void riscv32_task_switch(uint32_t *prev_sp, uint32_t *next_sp);
/** @ingroup kernel_riscv32
 * @fn riscv32_kernel_entry_trampoline
 * @brief カーネルタスクのエントリポイント.
 */
void riscv32_kernel_entry_trampoline(void);
/** @ingroup kernel_riscv32
 * @fn riscv32_user_entry_trampoline
 * @brief ユーザータスクのエントリポイント.
 */
void riscv32_user_entry_trampoline(void);
