/** @file driver.c */
// デバイスドライバAPI。基本的にはVMサーバに対するメッセージパッシングのラッパー。
#include <libs/user/driver.h>
#include <libs/user/ipc.h>

/** @ingroup user
 * @brief 指定された物理メモリ領域を空いている仮想アドレス領域にマップする.
 * MMIO領域にアクセスしたい時に使える。
 * @param paddr 物理アドレス領域
 * @param size サイズ
 * @param map_flags メモリ領域の権限.
 *      PAGE_(READABLE|WRITABLE|EXECUTABLE) を指定する。
 * @param uaddr マップされた仮想アドレスを格納する変数へのポインタ
 * @return 成功したらOK, エラーの場合はエラーコード.
 */
error_t driver_map_pages(paddr_t paddr, size_t size, int map_flags,
                         uaddr_t *uaddr) {
    struct message m;
    m.type = VM_MAP_PHYSICAL_MSG;
    m.vm_map_physical.paddr = paddr;
    m.vm_map_physical.size = size;
    m.vm_map_physical.map_flags = map_flags;
    error_t err = ipc_call(VM_SERVER, &m);
    if (err != OK) {
        return err;
    }

    *uaddr = m.vm_map_physical_reply.uaddr;
    return OK;
}

/** @ingroup user
 * @brief 物理メモリ領域を確保する.
 * @param size サイズ
 * @param map_flags メモリ領域の権限.
 *      PAGE_(READABLE|WRITABLE|EXECUTABLE) を指定する。
 * @param uaddr 確保したメモリ領域の仮想アドレスを格納する変数へのポインタ
 * @param paddr 確保したメモリ領域の物理アドレスを格納する変数へのポインタ
 * @return 成功したらOK, エラーの場合はエラーコード.
 */
error_t driver_alloc_pages(size_t size, int map_flags, uaddr_t *uaddr,
                           paddr_t *paddr) {
    struct message m;
    m.type = VM_ALLOC_PHYSICAL_MSG;
    m.vm_alloc_physical.size = size;
    m.vm_alloc_physical.alloc_flags = 0;
    m.vm_alloc_physical.map_flags = map_flags;
    error_t err = ipc_call(VM_SERVER, &m);
    if (err != OK) {
        return err;
    }

    *uaddr = m.vm_alloc_physical_reply.uaddr;
    *paddr = m.vm_alloc_physical_reply.paddr;
    return OK;
}
