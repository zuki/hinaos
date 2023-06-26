/** @file dmabuf.h */
#pragma once
#include <libs/common/types.h>

/** @ingroup user
 * @struct dmabuf
 * @brief DMAバッファ管理構造体.
 * [paddr, paddr + entry_size * num_entries) の範囲が
 * この管理構造体が所持する物理メモリ領域になる。 */
struct dmabuf {
    paddr_t paddr;       /**< DMAバッファ領域の物理アドレス */
    uaddr_t uaddr;       /**< DMAバッファ領域の仮想アドレス */
    size_t entry_size;   /**< 1つのバッファのサイズ */
    size_t num_entries;  /**< バッファの数 */
    bool *used;          /**< バッファの使用状況 */
};


/** @ingroup user
 * @typedef dmabuf_t
 * @brief DMAバッファ管理構造体へのポインタ. */
typedef struct dmabuf *dmabuf_t;

dmabuf_t dmabuf_create(size_t entry_size, size_t num_entries);
void *dmabuf_alloc(dmabuf_t dmabuf, paddr_t *paddr);
void *dmabuf_p2v(dmabuf_t dmabuf, paddr_t paddr);
void dmabuf_free(dmabuf_t dmabuf, paddr_t paddr);
