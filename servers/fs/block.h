/** @file block.h */
#pragma once

#include <libs/common/list.h>
#include <libs/common/types.h>

// ブロックのサイズ (バイト)
#define BLOCK_SIZE 4096

// ブロック番号
typedef uint16_t block_t;

/** @ingroup fs
 * @struct block
 * @brief ブロックキャッシュ
 *
 * ストレージデバイスの内容を読み書きする際には、まずデバイスからBLOCK_SIZE分の
 * データを一気に読み出してブロックキャッシュとして追加し、ファイルシステム実装は
 * メモリ上にあるキャッシュデータを読み書きする。
 */
struct block {
    block_t index;             /**< ディスク上のブロック番号 */
    list_elem_t cache_next;    /**< ブロックキャッシュのリストの要素 */
    list_elem_t dirty_next;    /**< 変更済みブロックキャッシュのリストの要素 */
    uint8_t data[BLOCK_SIZE];  /**< ブロックの内容 */
};

error_t block_read(block_t index, struct block **block);
void block_mark_as_dirty(struct block *block);
void block_flush_all(void);
void block_init(void);
