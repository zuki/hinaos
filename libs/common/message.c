/** @file message.c */
#include <libs/common/message.h>

// IPCスタブジェネレータが生成するコンパイル時チェック
IPCSTUB_STATIC_ASSERTIONS

// doxygenでmsgtype2str()のreturn値にIPCSTUB_STATIC_ASSERTIONSが
// 含まれるのを防ぐためで意味はない
#define FOR_DOXYGEN

/** @ingroup common
 * @brief メッセージの種類に対応する名前を返す. デバッグ用。
 * @param type メッセージ種別
 * @return メッセージ種別名
 */
const char *msgtype2str(int type) {
    int id = MSG_ID(type);
    if (id == 0 || id > IPCSTUB_MSGID_MAX) {
        return "(invalid)";
    }

    return IPCSTUB_MSGID2STR[id];
}
