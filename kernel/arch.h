/** @file arch.h */
#pragma once
#include <libs/common/types.h>

struct cpuvar;
#include <arch_types.h>

/** @ingroup kernel
 * @def CPUVAR_MAGIC
 * @brief cpuvarマジックナンバー */
#define CPUVAR_MAGIC 0xbeefbeef
/** @ingroup kernel
 * @def CPUVAR
 * @brief cpuvarを取得する */
#define CPUVAR       (arch_cpuvar_get())

struct task;

/** @ingroup kernel
 * @struct cpuvar
 * @brief cpuvar構造体 */
struct cpuvar {
    struct arch_cpuvar arch;    /**< アーキテクチャ固有のcpuvar */
    int id;                     /**< cpu番号 */
    bool online;                /**< 起動完了済み */
    unsigned ipi_pending;       /**< 保留中のIPI */
    struct task *idle_task;     /**< アイドルタスクへのポインタ */
    struct task *current_task;  /**< カレントタスクへのポインタ */
    unsigned magic;             /**< マジックナンバー */
};

#define IPI_TLB_FLUSH  (1 << 0)
#define IPI_RESCHEDULE (1 << 1)

/** @ingroup kernel
 * @struct memory_map_entry
 * @brief メモリマップエントリ構造体 */
struct memory_map_entry {
    paddr_t paddr;              /**< 物理アドレス */
    size_t size;                /**< サイズ */
};

/** @ingroup kernel
 * @def NUM_MEMORY_MAP_ENTRIES_MAX
 * @brief メモリマップエントリの最大数 */
#define NUM_MEMORY_MAP_ENTRIES_MAX 8

/** @ingroup kernel
 * @struct memory_map
 * @brief メモリマップ構造体 */
struct memory_map {
    struct memory_map_entry frees[NUM_MEMORY_MAP_ENTRIES_MAX];      /**< RAM領域のマップエントリ */
    struct memory_map_entry devices[NUM_MEMORY_MAP_ENTRIES_MAX];    /**< MMIO領域のマップエントリ */
    int num_frees;      /**< 未使用エントリ数 */
    int num_devices;    /**< MMIOエントリ数 */
};

/** @ingroup kernel
 * @struct bootinfo
 * @brief 起動情報構造体 */
struct bootinfo {
    paddr_t boot_elf;               /**< ELF形式の起動モジュールの先頭アドレス */
    struct memory_map memory_map;   /**< メモリマップ構造体 */
};

ARCH_TYPES_STATIC_ASSERTS

void arch_serial_write(char ch);
int arch_serial_read(void);
error_t arch_vm_init(struct arch_vm *vm);
void arch_vm_destroy(struct arch_vm *vm);
error_t arch_vm_map(struct arch_vm *vm, vaddr_t vaddr, paddr_t paddr,
                    unsigned attrs);
error_t arch_vm_unmap(struct arch_vm *vm, vaddr_t vaddr);
vaddr_t arch_paddr_to_vaddr(paddr_t paddr);
bool arch_is_mappable_uaddr(uaddr_t uaddr);
error_t arch_task_init(struct task *task, uaddr_t ip, vaddr_t kernel_entry,
                       void *arg);
void arch_task_destroy(struct task *task);
void arch_task_switch(struct task *prev, struct task *next);
void arch_init(void);
void arch_init_percpu(void);
void arch_idle(void);
void arch_send_ipi(unsigned ipi);
/** @ingroup kernel_riscv32
 * @brief ユーザ空間からカーネル空間にコピーする
 * @param dst コピー先（カーネル空間）のアドレス
 * @param src コピー元（ユーザ空間）のアドレス
 * @param len コピー長
 */
void arch_memcpy_from_user(void *dst, __user const void *src, size_t len);
/** @ingroup kernel_riscv32
 * @brief カーネル空間からにユーザ空間コピーする
 * @param dst コピー先（ユーザ空間）のアドレス
 * @param src コピー元（カーネル空間）のアドレス
 * @param len コピー長
 */
void arch_memcpy_to_user(__user void *dst, const void *src, size_t len);
error_t arch_irq_enable(unsigned irq);
error_t arch_irq_disable(unsigned irq);
__noreturn void arch_shutdown(void);

int32_t arch_rtc_epoch(void);
