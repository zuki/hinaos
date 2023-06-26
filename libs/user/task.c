/** @file task.c */
#include <libs/user/syscall.h>
#include <libs/user/task.h>

/** @ingroup user
 * @brief 実行中タスクのタスクIDを取得する.
 * @return タスクID
 */
task_t task_self(void) {
    static task_t tid = 0;
    if (tid) {
        // 既にタスクIDを取得しているので、キャッシュした値を返す。
        return tid;
    }

    // タスクIDは一度取得したら変わらないので、キャッシュしておく。
    tid = sys_task_self();
    return tid;
}
