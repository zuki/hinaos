/** @file mp.h */
#pragma once
#include <libs/common/types.h>

/** @ingroup kernel_riscv32
 * @def BKL_LOCKED
 * @brief カーネルロックをいずれかのCPUが使用中 */
#define BKL_LOCKED   0x12ab
/** @ingroup kernel_riscv32
 * @def BKL_UNLOCKED
 * @brief カーネルロックを誰も使用していない */
#define BKL_UNLOCKED 0xc0be
/** @ingroup kernel_riscv32
 * @def BKL_HALTED
 * @brief システムが停止した状態 (ロックを取らず停止する) */
#define BKL_HALTED   0xdead

int mp_self(void);
void mp_lock(void);
void mp_force_lock(void);
void mp_unlock(void);
struct cpuvar *riscv32_cpuvar_of(int hartid);
void mp_send_ipi(void);
__noreturn void halt(void);
void riscv32_mp_init_percpu(void);
