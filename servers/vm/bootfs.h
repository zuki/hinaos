/** @file bootfs.h */
#pragma once
#include <libs/common/types.h>

/** @ingroup vm
 * @struct bootfs_header
 * @brief BootFSファイルシステムヘッダ */
struct bootfs_header {
    uint16_t version;           /**< バージョン */
    uint16_t header_size;       /**< ヘッダーサイズ */
    uint16_t num_files;         /**< ファイル数 */
    uint16_t padding;           /**< パディング */
} __packed;

/** @ingroup vm
 * @struct bootfs_file
 * @brief BootFSファイルエントリ */
struct bootfs_file {
    char name[56];              /**< ファイル名 */
    uint32_t offset;            /**< オフセット */
    uint32_t len;               /**< ファイル長 */
} __packed;

struct bootfs_file *bootfs_open(const char *path);
struct bootfs_file *bootfs_open_iter(unsigned index);
void bootfs_read(struct bootfs_file *file, offset_t off, void *buf, size_t len);
void bootfs_init(void);
