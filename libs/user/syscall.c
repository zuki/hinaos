/** @file syscall.c */
#include <libs/common/hinavm_types.h>
#include <libs/common/print.h>
#include <libs/user/ipc.h>
#include <libs/user/syscall.h>

/** @ingroup user
 * @brief ipcシステムコール: メッセージの送受信
 * @param dst 送信先タスク
 * @param src 送信元タスク
 * @param m メッセージ
 * @param flags フラグ
 * @return 成功したらOK, それ以外はエラーコード
 */
error_t sys_ipc(task_t dst, task_t src, struct message *m, unsigned flags) {
    return arch_syscall(dst, src, (uintptr_t) m, flags, 0, SYS_IPC);
}

/** @ingroup user
 * @brief notifyシステムコール: 通知の送信
 * @param dst 送信先タスク
 * @param notifications 通知
 * @return 成功したらOK, それ以外はエラーコード
 */
error_t sys_notify(task_t dst, notifications_t notifications) {
    return arch_syscall(dst, notifications, 0, 0, 0, SYS_NOTIFY);
}

/** @ingroup user
 * @brief task_createシステムコール: タスクの生成
 * @param name タスク名
 * @param ip 実行アドレス
 * @param pager ページャタスク
 * @return タスクID
 */
task_t sys_task_create(const char *name, vaddr_t ip, task_t pager) {
    return arch_syscall((uintptr_t) name, ip, pager, 0, 0, SYS_TASK_CREATE);
}

/** @ingroup user
 * @brief hinavmシステムコール: HinaVMプログラムの実行
 * @param name タスク名
 * @param insts 命令配列
 * @param num_insts 命令数
 * @param pager ページャタスク
 * @return タスクID
 */
task_t sys_hinavm(const char *name, hinavm_inst_t *insts, size_t num_insts,
                  task_t pager) {
    return arch_syscall((uintptr_t) name, (uintptr_t) insts, num_insts, pager,
                        0, SYS_HINAVM);
}

/** @ingroup user
 * @brief task_destroyシステムコール: タスクの削除
 * @param task 削除するタスク
 * @return 成功したらOK, それ以外はエラーコード
 */
error_t sys_task_destroy(task_t task) {
    return arch_syscall(task, 0, 0, 0, 0, SYS_TASK_DESTROY);
}

/** @ingroup user
 * @brief task_exitシステムコール: 実行中タスクの終了
 */
__noreturn void sys_task_exit(void) {
    arch_syscall(0, 0, 0, 0, 0, SYS_TASK_EXIT);
    UNREACHABLE();
}

/** @ingroup user
 * @brief task_selfシステムコール: 実行中タスクのIDの取得
 * @return タスクID
 */
task_t sys_task_self(void) {
    return arch_syscall(0, 0, 0, 0, 0, SYS_TASK_SELF);
}

/** @ingroup user
 * @brief pm_allocシステムコール: 物理メモリの割り当て
 * @param tid タスクID
 * @param size メモリサイズ
 * @param flags フラグ
 * @return 物理ページ番号
 */
//
pfn_t sys_pm_alloc(task_t tid, size_t size, unsigned flags) {
    return arch_syscall(tid, size, flags, 0, 0, SYS_PM_ALLOC);
}

/** @ingroup user
 * @brief vm_mapシステムコール: ページのマップ
 * @param task タスクID
 * @param uaddr 仮想アドレス
 * @param paddr 物理アドレス
 * @param attrs 属性
 * @return 成功したらOK, それ以外はエラーコード
 */
error_t sys_vm_map(task_t task, uaddr_t uaddr, paddr_t paddr, unsigned attrs) {
    return arch_syscall(task, uaddr, paddr, attrs, 0, SYS_VM_MAP);
}

/** @ingroup user
 * @brief vm_unmapシステムコール: ページのアンマップ
 * @param task タスクID
 * @param uaddr 仮想アドレス
 * @return 成功したらOK, それ以外はエラーコード
 */
error_t sys_vm_unmap(task_t task, uaddr_t uaddr) {
    return arch_syscall(task, uaddr, 0, 0, 0, SYS_VM_UNMAP);
}

/** @ingroup user
 * @brief irq_listenシステムコール: 割り込み通知の購読
 * @param irq 割り込み番号
 * @return 成功したらOK, それ以外はエラーコード
 */
error_t sys_irq_listen(unsigned irq) {
    return arch_syscall(irq, 0, 0, 0, 0, SYS_IRQ_LISTEN);
}

/** @ingroup user
 * @brief irq_unlistenシステムコール: 割り込み通知の購読解除
 * @param irq 割り込み番号
 * @return 成功したらOK, それ以外はエラーコード
 */
error_t sys_irq_unlisten(unsigned irq) {
    return arch_syscall(irq, 0, 0, 0, 0, SYS_IRQ_UNLISTEN);
}

/** @ingroup user
 * @brief serial_writeシステムコール: 文字列の出力
 * @param buf 出力バッファ
 * @param len バッファ長
 * @return 実際に出力された文字列長
 */
int sys_serial_write(const char *buf, size_t len) {
    return arch_syscall((uintptr_t) buf, len, 0, 0, 0, SYS_SERIAL_WRITE);
}

/** @ingroup user
 * @brief serial_readシステムコール: 文字入力の読み込み
 * @param buf 入力バッファ
 * @param max_len 最大バッファ長
 * @return 実際に入力された文字列長
 */
int sys_serial_read(const char *buf, int max_len) {
    return arch_syscall((uintptr_t) buf, max_len, 0, 0, 0, SYS_SERIAL_READ);
}

/** @ingroup user
 * @brief timeシステムコール: タイムアウトの設定
 * @param milliseconds タイム・アウトするミリ秒
 * @return 成功したらOK, それ以外はエラーコード
 */
error_t sys_time(int milliseconds) {
    return arch_syscall(milliseconds, 0, 0, 0, 0, SYS_TIME);
}

/** @ingroup user
 * @brief uptimeシステムコール: システムの起動時間の取得 (ミリ秒)
 * @return システム起動時間（ミリ秒）
 */
int sys_uptime(void) {
    return arch_syscall(0, 0, 0, 0, 0, SYS_UPTIME);
}

/** @ingroup user
 * @brief shutdownシステムコール: システムのシャットダウン
 */
__noreturn void sys_shutdown(void) {
    arch_syscall(0, 0, 0, 0, 0, SYS_SHUTDOWN);
    UNREACHABLE();
}

/** @ingroup user
 * @brief epochシステムコール: epoch時の首藤
 * @return システム起動時間（ミリ秒）
 */
error_t sys_epoch(int32_t *time) {
    return arch_syscall((uintptr_t) time, 0, 0, 0, 0, SYS_EPOCH);
}
