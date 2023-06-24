/**
 * @file memory.c
 */
#include "memory.h"
#include "arch.h"
#include "ipc.h"
#include "printk.h"
#include "task.h"
#include <libs/common/string.h>

/**
 * @ingroup kernel
 * @var zones
 * @brief 物理メモリの各連続領域 (ゾーン) のリスト
 */
static list_t zones = LIST_INIT(zones);

// 物理アドレスに対応するゾーンを探す。
static struct page *find_page_by_paddr(paddr_t paddr,
                                       enum memory_zone_type *zone_type) {
    DEBUG_ASSERT(IS_ALIGNED(paddr, PAGE_SIZE));

    LIST_FOR_EACH (zone, &zones, struct memory_zone, next) {
        if (zone->base <= paddr
            && paddr < zone->base + zone->num_pages * PAGE_SIZE) {
            size_t start = (paddr - zone->base) / PAGE_SIZE;

            if (zone_type) {
                *zone_type = zone->type;
            }
            return &zone->pages[start];
        }
    }

    return NULL;
}

// ゾーンを追加する。
static void add_zone(struct memory_zone *zone, enum memory_zone_type type,
                     paddr_t paddr, size_t num_pages) {
    zone->type = type;
    zone->base = paddr;
    zone->num_pages = num_pages;
    for (size_t i = 0; i < num_pages; i++) {
        zone->pages[i].ref_count = 0;
    }

    list_elem_init(&zone->next);
    list_push_back(&zones, &zone->next);
}

// start番目からnum_pages個の物理ページが空いているかどうかを返す。
static bool is_contiguously_free(struct memory_zone *zone, size_t start,
                                 size_t num_pages) {
    for (size_t i = 0; i < num_pages; i++) {
        if (zone->pages[start + i].ref_count != 0) {
            return false;
        }
    }
    return true;
}

/**
 * @ingroup kernel
 * @brief 指定されたバイトの連続した物理メモリ領域を物理ページ単位で割り当てる.
 * @param size 割り当てるバイト数
 * @param owner 割り当てた領域の所有者のクラス. NULLの場合はカーネル。
 * @param flags フラグ.
 *  - PM_ALLOC_ZEROED: 物理ページをゼロクリアする
 *  - PM_ALLOC_ALIGNED: sizeでアラインされた物理メモリアドレスを返す
 * @return 割り当てた物理メモリの先頭アドレス. 割り当てるメモリがない場合は0。
 */
paddr_t pm_alloc(size_t size, struct task *owner, unsigned flags) {
    size_t aligned_size = ALIGN_UP(size, PAGE_SIZE);  // 実際に割り当てるサイズ
    size_t num_pages = aligned_size / PAGE_SIZE;      // 割り当てる物理ページ数
    LIST_FOR_EACH (zone, &zones, struct memory_zone, next) {
        if (zone->type != MEMORY_ZONE_FREE) {
            // MMIO領域は使えない
            continue;
        }

        // 各start番目からnum_pages個の物理ページが空いているかどうかを調べる。
        for (size_t start = 0; start < zone->num_pages; start++) {
            paddr_t paddr = zone->base + start * PAGE_SIZE;
            if ((flags & PM_ALLOC_ALIGNED) != 0
                && !IS_ALIGNED(paddr, aligned_size)) {
                // アラインメントが合わないのでスキップ
                continue;
            }

            if (is_contiguously_free(zone, start, num_pages)) {
                // 空いているので各物理ページを割り当てる
                for (size_t i = 0; i < num_pages; i++) {
                    struct page *page = &zone->pages[start + i];
                    page->ref_count = 1;
                    page->owner = owner;
                    list_elem_init(&page->next);

                    if (owner) {
                        list_push_back(&owner->pages, &page->next);
                    }
                }

                // 必要があればゼロクリアする
                if (flags & PM_ALLOC_ZEROED) {
                    memset((void *) arch_paddr_to_vaddr(paddr), 0,
                           PAGE_SIZE * num_pages);
                }

                return paddr;
            }
        }
    }

    WARN("pm: run out of memory");
    return 0;
}

// 物理ページを1つ解放する。
static void free_page(struct page *page) {
    DEBUG_ASSERT(page->ref_count > 0);

    // 参照カウントを減らす。0になるまでは余所で参照されているので注意。
    page->ref_count--;

    if (page->ref_count == 0) {
        list_remove(&page->next);
    }
}

/** @ingroup kernel
 * @brief 物理ページの所有者を設定する. 所有者タスクが終了するときに、
 * 指定した物理ページの参照カウントが減算されるようになる。タスクに
 * 対して物理ページを割り当てたいが、まだそのタスクの初期化が終わって
 * いない場合に使う。
 * @param paddr 所有者を設定する物理ページのアドレス
 * @param owner 所有者（タスク）
 */
void pm_own_page(paddr_t paddr, struct task *owner) {
    struct page *page = find_page_by_paddr(paddr, NULL);

    ASSERT(page != NULL);
    ASSERT(page->owner == NULL);
    ASSERT(page->ref_count == 1);
    ASSERT(!list_is_linked(&page->next));

    page->owner = owner;
    list_push_back(&owner->pages, &page->next);
}

/** @ingroup kernel
 * @brief pm_alloc関数で割り当てた、連続した物理メモリ領域を解放する.
 * @param paddr 解放する物理メモリ領域のアドレス
 * @param size 解放する物理メモリ領域のサイズ
 */
void pm_free(paddr_t paddr, size_t size) {
    DEBUG_ASSERT(IS_ALIGNED(size, PAGE_SIZE));

    // 各ページを解放する
    for (size_t offset = 0; offset < size; offset += PAGE_SIZE) {
        // 物理アドレスからページ管理構造体を取得する
        struct page *page = find_page_by_paddr(paddr + offset, NULL);
        ASSERT(page != NULL);
        free_page(page);
    }
}

/** @ingroup kernel
 * @brief pm_free関数の引数にリストを指定するバージョン.
 * @param pages 解放する物理ページのリスト
 */
void pm_free_by_list(list_t *pages) {
    LIST_FOR_EACH (page, pages, struct page, next) {
        free_page(page);
    }
}

/** @ingroup kernel
 * @brief ページを指定した仮想アドレスにマップ (ページテーブルへの追加) する.
 */
error_t vm_map(struct task *task, uaddr_t uaddr, paddr_t paddr,
               unsigned attrs) {
    // 物理アドレスからページ管理構造体を取得する。
    enum memory_zone_type zone_type;
    struct page *page = find_page_by_paddr(paddr, &zone_type);
    if (!page) {
        WARN("%s: vm_map: no page for paddr %p", task->name, paddr);
        return ERR_INVALID_PADDR;
    }

    // ページをマップしてよいか、言い換えるとその物理ページへのアクセスを許可してよいかを判断する
    switch (zone_type) {
        // RAM領域
        case MEMORY_ZONE_FREE:
            if (page->ref_count == 0) {
                WARN("%s: vm_map: paddr %p is not allocated", task->name,
                     paddr);
                return ERR_INVALID_PADDR;
            }

            // 次のいずれかの条件を満たすときにページをマップできる:
            //
            // 1) taskがそのページを所有しているタスク
            // 2) taskがそのページを所有しているタスクのページャタスク
            if (page->owner != task && page->owner->pager != task) {
                WARN("%s: vm_map: paddr %p is not owned", task->name, paddr);
                return ERR_INVALID_PADDR;
            }
            break;
        // MMIO領域
        case MEMORY_ZONE_MMIO:
            if (page->ref_count > 0) {
                // 既にマップされている。複数のタスクが同じMMIO領域をマップすることはできない。
                // 複数のデバイスドライバサーバが同時に同じデバイスを操作することはないはず。
                WARN("%s: vm_map: device paddr %p is already mapped (owner=%s)",
                     task->name, paddr, page->owner ? page->owner->name : NULL);
                return ERR_INVALID_PADDR;
            }
            break;
    }

    error_t err = arch_vm_map(&task->vm, uaddr, paddr, attrs);
    if (err != OK) {
        return err;
    }

    // MMIO領域の場合はタスクを所有者として登録する。RAM領域の場合はpm_alloc関数で登録済み。
    if (zone_type == MEMORY_ZONE_MMIO && task) {
        list_push_back(&task->pages, &page->next);
    }

    page->ref_count++;
    return OK;
}

// ページをアンマップ (ページテーブルからの削除) する。
error_t vm_unmap(struct task *task, uaddr_t uaddr) {
    if (!arch_is_mappable_uaddr(uaddr)) {
        return ERR_INVALID_ARG;
    }

    error_t err = arch_vm_unmap(&task->vm, uaddr);
    if (err != OK) {
        return err;
    }

    return OK;
}

// ページフォルトハンドラ
void handle_page_fault(vaddr_t vaddr, vaddr_t ip, unsigned fault) {
    // カーネル内ではページフォルトが起きない
    // (ユーザーポインタのメモリコピー時には PAGE_FAULT_USER がセットされている)
    if ((fault & PAGE_FAULT_USER) == 0) {
        PANIC("page fault in kernel: vaddr=%p, ip=%p, reason=%x", vaddr, ip,
              fault);
    }

    // ページフォルトが起きたアドレスがマップ可能なアドレスかどうかチェックする
    // (NULLページやカーネル領域のアドレスはマップ不可)
    if (!arch_is_mappable_uaddr(vaddr)) {
        WARN("%s: page fault at unmappable vaddr: vaddr=%p, ip=%p",
             CURRENT_TASK->name, vaddr, ip);
        task_exit(EXP_INVALID_UADDR);
    }

    // アイドルタスクと最初のユーザータスクではページフォルトが起きない
    struct task *pager = CURRENT_TASK->pager;
    if (!pager) {
        PANIC("%s: unexpected page fault: vaddr=%p, ip=%p", CURRENT_TASK->name,
              vaddr, ip);
    }

    // ページャタスクにページフォルト処理要求メッセージを送信し返信を待つ
    struct message m;
    m.type = PAGE_FAULT_MSG;
    m.page_fault.task = CURRENT_TASK->tid;
    m.page_fault.uaddr = vaddr;
    m.page_fault.ip = ip;
    m.page_fault.fault = fault;
    error_t err = ipc(pager, pager->tid, (__user struct message *) &m,
                      IPC_CALL | IPC_KERNEL);

    // ページャタスクからの応答メッセージが正しいかどうかチェックする
    if (err != OK || m.type != PAGE_FAULT_REPLY_MSG) {
        task_exit(EXP_INVALID_PAGER_REPLY);
    }
}

/** @group kernel
 * @brief メモリ管理システムの初期化.
 * @param bootinfo bootinfoへのポインタ
 */
void memory_init(struct bootinfo *bootinfo) {
    struct memory_map *memory_map = &bootinfo->memory_map;
    for (int i = 0; i < memory_map->num_frees; i++) {
        struct memory_map_entry *e = &memory_map->frees[i];

        TRACE("free memory: %p - %p (%dMiB)", e->paddr, e->paddr + e->size,
              e->size / 1024 / 1024);

        struct memory_zone *zone =
            (struct memory_zone *) arch_paddr_to_vaddr(e->paddr);
        size_t num_pages =
            ALIGN_DOWN(e->size, PAGE_SIZE) / (PAGE_SIZE + sizeof(struct page));

        void *end_of_header = &zone->pages[num_pages + 1];
        size_t header_size = ((vaddr_t) end_of_header) - ((vaddr_t) zone);
        paddr_t paddr = e->paddr + ALIGN_UP(header_size, PAGE_SIZE);

        add_zone(zone, MEMORY_ZONE_FREE, paddr, num_pages);
    }

    for (int i = 0; i < memory_map->num_devices; i++) {
        struct memory_map_entry *e = &memory_map->devices[i];
        ASSERT(IS_ALIGNED(e->size, PAGE_SIZE));

        TRACE("MMIO memory: %p - %p (%dKiB)", e->paddr, e->paddr + e->size,
              e->size / 1024);

        size_t num_pages = e->size / PAGE_SIZE;
        paddr_t zone_paddr = pm_alloc(sizeof(struct page) * num_pages, NULL,
                                      PM_ALLOC_UNINITIALIZED);
        ASSERT(zone_paddr != 0);
        struct memory_zone *zone =
            (struct memory_zone *) arch_paddr_to_vaddr(zone_paddr);
        add_zone(zone, MEMORY_ZONE_MMIO, e->paddr, num_pages);
    }
}
