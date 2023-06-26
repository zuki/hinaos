/** @file dmabuf.c */
#include "dmabuf.h"
#include "driver.h"
#include <libs/common/print.h>
#include <libs/common/string.h>
#include <libs/user/malloc.h>

// 渡された物理アドレスがdmabufの管理下にあるかどうかをチェックする
static void check_paddr(dmabuf_t dmabuf, paddr_t paddr) {
    ASSERT(dmabuf->paddr <= paddr);
    ASSERT(paddr < dmabuf->paddr + dmabuf->entry_size * dmabuf->num_entries);
}

/** @ingroup user
 * @brief DMAバッファ管理構造体を生成する.
 * @param entry_size バッファの1つの要素のサイズ
 * @param num_entries バッファの要素数
 * @return 生成したDMAバッファ管理構造体へのポインタ, 失敗時にはNULL
 */
dmabuf_t dmabuf_create(size_t entry_size, size_t num_entries) {
    // 返すアドレスがアライメントされることを保証
    entry_size = ALIGN_UP(entry_size, 4);

    // バッファの管理情報を確保し、初期化する
    struct dmabuf *dmabuf = malloc(sizeof(struct dmabuf));
    dmabuf->entry_size = entry_size;
    dmabuf->num_entries = num_entries;
    dmabuf->used = malloc(sizeof(bool) * num_entries);
    memset(dmabuf->used, 0, sizeof(bool) * num_entries);

    // DMAバッファを確保する
    error_t err = driver_alloc_pages(
        ALIGN_UP(entry_size * num_entries, PAGE_SIZE),
        PAGE_READABLE | PAGE_WRITABLE, &dmabuf->uaddr, &dmabuf->paddr);
    if (err != OK) {
        WARN("failed to allocate a DMA region for dmabuf: %s", err2str(err));
        return NULL;
    }

    DEBUG_ASSERT(dmabuf->paddr != 0);
    return dmabuf;
}

/** @ingroup user
 * @brief DMAバッファをひとつ割り当てる.
 * @param dmabuf DMAバッファ管理構造体
 * @param paddr 割り当てられたDMAバッファの物理アドレスを格納する変数へのポインタ
 * @return 割り当てたDMAバッファ, 失敗時にはNULL
 */
void *dmabuf_alloc(dmabuf_t dmabuf, paddr_t *paddr) {
    for (size_t i = 0; i < dmabuf->num_entries; i++) {
        if (!dmabuf->used[i]) {
            dmabuf->used[i] = true;
            offset_t offset = i * dmabuf->entry_size;
            *paddr = dmabuf->paddr + offset;
            return (void *) (dmabuf->uaddr + offset);
        }
    }
    return NULL;
}

/** @ingroup user
 * @brief dmabuf_alloc関数で割り当てた物理アドレスから対応する仮想アドレスを得る.
 * @param dmabuf DMAバッファ管理構造体
 * @param paddr DMAバッファの物理アドレス
 * @return paddrに対応する仮想アドレス
 */
void *dmabuf_p2v(dmabuf_t dmabuf, paddr_t paddr) {
    check_paddr(dmabuf, paddr);
    return (void *) (dmabuf->uaddr + paddr - dmabuf->paddr);
}

/** @ingroup user
 * @brief dmabuf_alloc関数で割り当てたDMAバッファを解放する.
 * @param dmabuf DMAバッファ管理構造体
 * @param paddr DMAバッファの物理アドレス
 */
void dmabuf_free(dmabuf_t dmabuf, paddr_t paddr) {
    check_paddr(dmabuf, paddr);
    size_t index = (paddr - dmabuf->paddr) / dmabuf->entry_size;
    dmabuf->used[index] = false;
}
