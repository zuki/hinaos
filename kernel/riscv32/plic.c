/** @file plic.c */
// Platform-Level Interrupt Controller (PLIC) デバイスドライバ
// https://github.com/riscv/riscv-plic-spec
#include "plic.h"
#include "asm.h"
#include "mp.h"
#include "uart.h"
#include <libs/common/print.h>

/** @ingroup kernel_riscv32
 * @brief 受信済み割り込み番号を取得する
 */
unsigned riscv32_plic_pending(void) {
    return mmio_read32_paddr(PLIC_CLAIM(CPUVAR->id));
}

/** @ingroup kernel_riscv32
 * @brief 割り込みを処理したことをPLICに通知する
 * @param irq 割り込み番号
 */
void riscv32_plic_ack(unsigned irq) {
    ASSERT(irq < IRQ_MAX);

    mmio_write32_paddr((PLIC_CLAIM(CPUVAR->id)), irq);
}

/** @ingroup kernel_riscv32
 * @brief 割り込みを有効にする
 * @param irq 割り込み番号
 */
error_t arch_irq_enable(unsigned irq) {
    ASSERT(irq < IRQ_MAX);

    // 割り込み優先度を1に設定
    mmio_write32_paddr((PLIC_PRIORITY(irq)), 1);
    // 割り込みを有効化する
    mmio_write32_paddr((PLIC_ENABLE(irq)),
                       mmio_read32_paddr(PLIC_ENABLE(irq)) | (1 << (irq % 32)));
    return OK;
}

/** @ingroup kernel_riscv32
 * @brief 割り込みを無効にする
 * @param irq 割り込み番号
 */
error_t arch_irq_disable(unsigned irq) {
    ASSERT(irq < IRQ_MAX);

    // 割り込み優先度を0に設定
    mmio_write32_paddr((PLIC_PRIORITY(irq)), 0);
    // 割り込みを無効化する
    mmio_write32_paddr((PLIC_ENABLE(irq)), mmio_read32_paddr(PLIC_ENABLE(irq))
                                               & ~(1 << (irq % 32)));
    return OK;
}

/** @ingroup kernel_riscv32
 * @brief 各CPUでのPLICの初期化
 */
void riscv32_plic_init_percpu(void) {
    // 割り込み閾値を0に設定し、全ての割り込みを有効にする
    mmio_write32_paddr((PLIC_THRESHOLD(CPUVAR->id)), 0);
}
