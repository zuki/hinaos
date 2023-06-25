/** @file plic.h */
#pragma once

/** @ingroup kernel_riscv32
 * @def NUM_IRQS_MAX
 * @brief PLICの最大割り込み数 */
#define NUM_IRQS_MAX 1024
/** @ingroup kernel_riscv32
 * @def PLIC_ADDR
 * @brief PLICの物理アドレス */
#define PLIC_ADDR    ((paddr_t) 0x0c000000)
/** @ingroup kernel_riscv32
 * @def PLIC_SIZE
 * @brief PLICのMMIO領域のサイズ */
#define PLIC_SIZE    0x400000

// Interrupt Source Priority
// https://github.com/riscv/riscv-plic-spec/blob/master/riscv-plic.adoc#3-interrupt-priorities
#define PLIC_PRIORITY(irq) (PLIC_ADDR + 4 * (irq))

// Interrupt Enable Bits
// https://github.com/riscv/riscv-plic-spec/blob/master/riscv-plic.adoc#5-interrupt-enables
#define PLIC_ENABLE(irq) (PLIC_ADDR + 0x2080 + ((irq) / 32 * sizeof(uint32_t)))

// Priority Threshold
// https://github.com/riscv/riscv-plic-spec/blob/master/riscv-plic.adoc#6-priority-thresholds
#define PLIC_THRESHOLD(hart) (PLIC_ADDR + 0x201000 + 0x2000 * (hart))

// Interrupt Claim Register
// https://github.com/riscv/riscv-plic-spec/blob/master/riscv-plic.adoc#7-interrupt-claim-process
#define PLIC_CLAIM(hart) (PLIC_ADDR + 0x201004 + 0x2000 * (hart))

unsigned riscv32_plic_pending(void);
void riscv32_plic_ack(unsigned irq);
void riscv32_plic_init_percpu(void);
