/** @file list.c */
#include <libs/common/list.h>
#include <libs/common/print.h>

// prev と next の間に新しいエントリ new を挿入する。
//
//    prev <-> next  =>  prev <-> new <-> next
//
static void list_insert(list_elem_t *prev, list_elem_t *next,
                        list_elem_t *new) {
    new->prev = prev;
    new->next = next;
    next->prev = new;
    prev->next = new;
}

// エントリを無効な状態にする。バグ検出用。
static void list_elem_nullify(list_elem_t *elem) {
    elem->prev = NULL;
    elem->next = NULL;
}

/** @ingroup common
 * @brief リストを初期化する. 初期状態ではリストは空。
 * @param list 初期化するリストへのポインタ
 */
void list_init(list_t *list) {
    list->prev = list;
    list->next = list;
}

/** @ingroup common
 * @brief リストのエントリを初期化する.
 * @param elem 初期化するリストエントリへのポインタ
 */
void list_elem_init(list_elem_t *elem) {
    list_elem_nullify(elem);
}

/** @ingroup common
 * @brief リストが空かどうかを返す.
 * @param list リストへのポインタ
 * @return リストが空であればtrue, そうでなければfalse
 */
bool list_is_empty(list_t *list) {
    return list->next == list;
}

/** @ingroup common
 * @brief リストのエントリがどこかのリストに所属しているか (つまり使用中か) を返す。O(1)。
 * @param elem リストエントリへのポインタ
 * @return エントリがリストに属していればtrue, そうでなければfalse
 */
bool list_is_linked(list_elem_t *elem) {
    return elem->next != NULL;
}

/** @ingroup common
 * @brief リストの総エントリ数を返す. O(n)。
 * @param list リストへのポインタ
 * @return リストの総エントリ数
 */
size_t list_len(list_t *list) {
    size_t len = 0;
    struct list *node = list->next;
    while (node != list) {
        len++;
        node = node->next;
    }

    return len;
}

/** @ingroup common
 * @brief リストに指定されたエントリが含まれているかを返す. O(n)。
 * @param list リストへのポインタ
 * @param elem リストエントリへのポインタ
 * @return リストにエントリが含まれていればtrue, そうでなければfalse
 */
bool list_contains(list_t *list, list_elem_t *elem) {
    list_elem_t *node = list->next;
    while (node != list) {
        if (node == elem) {
            return true;
        }

        node = node->next;
    }

    return false;
}

/** @ingroup common
 * @brief リストからエントリを削除する. O(1)。
 * @param elem リストエントリへのポインタ
 */
void list_remove(list_elem_t *elem) {
    if (!list_is_linked(elem)) {
        return;
    }

    // 前後のエントリをつなぎ直すことで、エントリをリストから削除する。
    //
    //   prev <-> elem <-> next => prev <-> next
    //
    elem->prev->next = elem->next;
    elem->next->prev = elem->prev;

    // エントリを無効な状態にする。こうすることで二回以上の削除を防ぐ。
    list_elem_nullify(elem);
}

/** @ingroup common
 * @brief エントリをリストの末尾に追加する. O(1)。
 * @param list リストへのポインタ
 * @param new_tail リストエントリへのポインタ
 */
void list_push_back(list_t *list, list_elem_t *new_tail) {
    DEBUG_ASSERT(!list_contains(list, new_tail));
    DEBUG_ASSERT(!list_is_linked(new_tail));
    list_insert(list->prev, list, new_tail);
}

/** @ingroup common
 * @brief リストの先頭エントリを取り出す. O(1)。
 * @param list リストへのポインタ
 * @return リストの先頭エントリへのポインタ、空の場合はNULL
 */
list_elem_t *list_pop_front(list_t *list) {
    struct list *head = list->next;
    if (head == list) {
        return NULL;
    }

    // エントリをリストから削除する。
    struct list *next = head->next;
    list->next = next;
    next->prev = list;

    // エントリを無効な状態にする。こうすることで list_remove() を呼び出しても削除しない
    // ようにする。
    list_elem_nullify(head);
    return head;
}
