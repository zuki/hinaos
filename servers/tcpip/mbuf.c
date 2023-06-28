/** @file mbuf.c */
#include "mbuf.h"
#include <libs/common/print.h>
#include <libs/common/string.h>
#include <libs/user/malloc.h>

/** @ingroup tcpip
 * @brief mbufを割り当てる
 */
mbuf_t mbuf_alloc(void) {
    struct mbuf *m = malloc(sizeof(*m));
    m->next = NULL;
    m->offset = 0;
    m->offset_end = 0;
    return m;
}

/** @ingroup tcpip
 * @brief 新たにmbufを割り当てて、指定したデータをコピーする
 * @param data データ
 * @param len データ長
 */
mbuf_t mbuf_new(const void *data, size_t len) {
    struct mbuf *head = NULL;
    struct mbuf *tail = NULL;
    const uint8_t *p = data;
    // データ全体をコピーし終えるまで、mbufをひとつずつ割り当てていく
    do {
        size_t mbuf_len = MIN(len, MBUF_MAX_LEN);
        struct mbuf *new_tail = mbuf_alloc();
        if (!head) {
            head = new_tail;
        }

        new_tail->next = NULL;
        new_tail->offset_end = mbuf_len;
        memcpy(new_tail->data, p, mbuf_len);

        if (tail) {
            tail->next = new_tail;
        }

        len -= mbuf_len;
        p += mbuf_len;
        tail = new_tail;
    } while (len > 0);

    return head;
}

/** @ingroup tcpip
 * @brief mbufチェーン全体を解放する
 * @param mbuf mbufチェーン
 */
void mbuf_delete(mbuf_t mbuf) {
    if (!mbuf) {
        return;
    }

    do {
        struct mbuf *next = mbuf->next;
        free(mbuf);
        mbuf = next;
    } while (mbuf);
}

/** @ingroup tcpip
 * @brief mbufチェーンの末尾に、新たなmbufを追加する
 * @param mbuf mbufチェーン
 * @param new_tail 追加するmbuf
 */
void mbuf_append(mbuf_t mbuf, mbuf_t new_tail) {
    struct mbuf *m = mbuf;
    while (m->next) {
        m = m->next;
    }
    m->next = new_tail;
}

/** @ingroup tcpip
 * @brief mbufチェーンの末尾に、指定したデータを追加する
 * @param mbuf mbufチェーン
 * @param data 追加データ
 * @param len 追加データ長
 */
void mbuf_append_bytes(mbuf_t mbuf, const void *data, size_t len) {
    // 末尾のmbufを探す
    while (mbuf->next != NULL) {
        mbuf = mbuf->next;
    }

    // 末尾のmbufに入るだけデータをコピーする
    size_t avail_len = MBUF_MAX_LEN - mbuf->offset_end;
    size_t copy_len = MIN(len, avail_len);
    if (copy_len > 0) {
        memcpy(&mbuf->data[mbuf->offset_end], data, copy_len);
        mbuf->offset_end += copy_len;
    }

    // 末尾のmbufに入りきらなかったデータを、新たなmbufにコピーする
    len -= copy_len;
    if (len > 0) {
        mbuf_append(mbuf, mbuf_new((uint8_t *) data + copy_len, len));
    }
}

/** @ingroup tcpip
 * @brief mbufが空かどうかを返す. 複数の要素から成るチェーンであっても、全てのmbufが空であればtrueを返す
 * @param mbuf mbufチェーン
 * @return mbufが空であればtrue, そうでなければfalse
 */
bool mbuf_is_empty(mbuf_t mbuf) {
    return mbuf_len(mbuf) == 0;
}

/** @ingroup tcpip
 * @brief mbufの先頭のデータを返す
 * @param mbuf mbufチェーン
 * @return 先頭のmbufのデータへのポインタ
 */
const void *mbuf_data(mbuf_t mbuf) {
    return &mbuf->data[mbuf->offset];
}

/** @ingroup tcpip
 * @brief mbufの単一要素の長さを返す
 * @param mbuf mbufチェーン
 * @return mbufの単一要素の長さ
 */
size_t mbuf_len_one(mbuf_t mbuf) {
    return mbuf->offset_end - mbuf->offset;
}

/** @ingroup tcpip
 * @brief mbufチェーン全体の長さを返す
 * @param mbuf mbufチェーン
 * @return mbufチェーン全体の長さ
 */
size_t mbuf_len(mbuf_t mbuf) {
    size_t total_len = 0;
    while (mbuf) {
        total_len += mbuf_len_one(mbuf);
        mbuf = mbuf->next;
    }
    return total_len;
}

/** @ingroup tcpip
 * @brief mbufから指定した長さだけデータを読み込む. 実際に読み込んだ長さを返す。
 * @param mbuf mbufチェーン
 * @param buf 読み込み用のバッファ
 * @param buf_len バッファ長
 * @return 実際に読み込んだ長さ
 */
size_t mbuf_read(mbuf_t *mbuf, void *buf, size_t buf_len) {
    size_t read_len = 0;
    while (true) {
        size_t mbuf_len = mbuf_len_one(*mbuf);
        if (!mbuf_len && (*mbuf)->next) {
            // Delete the current mbuf and move into the next one.
            struct mbuf *prev = *mbuf;
            *mbuf = (*mbuf)->next;
            free(prev);
            continue;
        }

        // mbufからコピーできる長さを計算する
        size_t copy_len = MIN(buf_len - read_len, mbuf_len);
        if (!copy_len) {
            break;
        }

        // データをコピーしてオフセットを進める
        memcpy(&buf[read_len], mbuf_data(*mbuf), copy_len);
        (*mbuf)->offset += copy_len;
        read_len += copy_len;
    }

    return read_len;
}

// mbufをひとつコピーする
static void mbuf_copy_one(mbuf_t dst, mbuf_t src) {
    dst->next = NULL;
    dst->offset = src->offset;
    dst->offset_end = src->offset_end;
    memcpy(dst->data, src->data, mbuf_len_one(src));
}

/** @ingroup tcpip
 * @brief mbufチェーンから指定した長さだけデータを読み込み、新しいmbufとして返す。
 * @param mbuf mbufチェーン
 * @param len 読み込む長さ
 * @return 新しいmbuf
 */
mbuf_t mbuf_peek(mbuf_t mbuf, size_t len) {
    mbuf_t dst = mbuf_alloc();
    mbuf_t head = dst;
    mbuf_t src = mbuf;
    while (src) {
        size_t mbuf_len = mbuf_len_one(src);
        mbuf_copy_one(dst, src);
        if (len <= mbuf_len) {
            dst->offset_end -= mbuf_len - len;
            break;
        }

        dst->next = mbuf_alloc();
        dst = dst->next;
        src = src->next;
        len -= mbuf_len;
    }

    return head;
}

/** @ingroup tcpip
 * @brief mbufチェーンの先頭から指定したバイトだけ破棄する
 * @param mbuf mbufチェーン
 * @param len 破棄する長さ
 * @return 破棄した長さ
 */
size_t mbuf_discard(mbuf_t *mbuf, size_t len) {
    size_t remaining = len;
    while (remaining > 0) {
        size_t mbuf_len = mbuf_len_one(*mbuf);
        if (!mbuf_len && (*mbuf)->next) {
            // このmbufを削除して、次のmbufに移動する
            struct mbuf *prev = *mbuf;
            *mbuf = (*mbuf)->next;
            free(prev);
            continue;
        }

        // このmbuf内で破棄する長さを計算する
        size_t discard_len = MIN(remaining, mbuf_len);
        if (!discard_len) {
            break;
        }

        (*mbuf)->offset += discard_len;
        remaining -= discard_len;
    }

    return len - remaining;
}

/** @ingroup tcpip
 * @brief 指定したバイト数になるように、mbufチェーンの末尾からデータを削除する (切り詰める)
 * @param mbuf mbufチェーン
 * @param len 指定のバイト数
 */
void mbuf_truncate(mbuf_t mbuf, size_t len) {
    while (mbuf && len > 0) {
        size_t mbuf_len = mbuf_len_one(mbuf);
        if (len < mbuf_len) {
            // このmbufの末尾を切り詰めて、以降のmbufを削除する
            mbuf->offset_end -= mbuf_len - len;
            mbuf_delete(mbuf->next);
            mbuf->next = NULL;
            break;
        }

        len -= mbuf_len;
        mbuf = mbuf->next;
    }
}

/** @ingroup tcpip
 * @brief mbufチェーンを複製する
 * @param mbuf mbufチェーン
 * @return 複製したmbufチェーンへのポインタ
 */
mbuf_t mbuf_clone(mbuf_t mbuf) {
    mbuf_t head = mbuf_alloc();
    mbuf_t clone = head;
    while (true) {
        memcpy(clone->data, mbuf->data, sizeof(mbuf->data));
        clone->offset = mbuf->offset;
        clone->offset_end = mbuf->offset_end;
        mbuf = mbuf->next;
        if (!mbuf) {
            break;
        }

        clone = mbuf_alloc();
    }

    return head;
}
