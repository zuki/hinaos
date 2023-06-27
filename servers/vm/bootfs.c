/** @file bootfs.c */
#include "bootfs.h"
#include <libs/common/print.h>
#include <libs/common/string.h>

extern char __bootfs[];            // BootFSイメージ
/** @ingroup vm
 * @var files
 * @brief BootFSのファイルリスト */
static struct bootfs_file *files;
/** @ingroup vm
 * @var num_files
 * @brief BootFS内のファイル数 */
static unsigned num_files;

/** @ingroup vm
 * @brief BootFSからファイルを読み込む
 * @param file ファイル名
 * @param off 読み込み位置のオフセット
 * @param buf 読み込むバッファ
 * @param len 読み込む長さ
 */
void bootfs_read(struct bootfs_file *file, offset_t off, void *buf,
                 size_t len) {
    void *p = (void *) (((uaddr_t) __bootfs) + file->offset + off);
    memcpy(buf, p, len);
}

/** @ingroup vm
 * @brief BootFSのファイルを開く
 * @param path ファイルのパス
 * @return BootFS構造体へのポインタ, 存在しない場合はNULL
 */
struct bootfs_file *bootfs_open(const char *path) {
    // ファイル名が一致するエントリを探す。
    struct bootfs_file *file;
    for (int i = 0; (file = bootfs_open_iter(i)) != NULL; i++) {
        if (!strncmp(file->name, path, sizeof(file->name))) {
            return file;
        }
    }

    return NULL;
}

/** @ingroup vm
 * @brief index番目のファイルエントリを返す
 * @param index インデックス
 * @return BootFS構造体へのポインタ, indexが不正な場合はNULL
 */
struct bootfs_file *bootfs_open_iter(unsigned index) {
    if (index >= num_files) {
        return NULL;
    }

    return &files[index];
}

/** @ingroup vm
 * @brief BootFSを初期化する
 */
void bootfs_init(void) {
    struct bootfs_header *header = (struct bootfs_header *) __bootfs;
    num_files = header->num_files;
    files =
        (struct bootfs_file *) (((uaddr_t) &__bootfs) + header->header_size);

    TRACE("bootfs: found following %d files", num_files);
    for (unsigned i = 0; i < num_files; i++) {
        TRACE("bootfs: \"%s\" (%d KiB)", files[i].name, files[i].len / 1024);
    }
}
