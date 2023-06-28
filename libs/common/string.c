/** @file string.c */
#include <libs/common/print.h>
#include <libs/common/string.h>

/** @ingroup common
 * @brief メモリの内容を比較する.
 * @param p1 メモリ1
 * @param p2 メモリ2
 * @param len 長さ
 * @return p1の最初のlenバイトがp2の最初のlenバイトよりも
 * 小さいか、同じか、あるいは大きいかによってそれぞれ、
 * 負の整数、0、正の整数を返す。
 */
int memcmp(const void *p1, const void *p2, size_t len) {
    uint8_t *s1 = (uint8_t *) p1;
    uint8_t *s2 = (uint8_t *) p2;
    while (*s1 == *s2 && len > 0) {
        s1++;
        s2++;
        len--;
    }

    return (len > 0) ? *s1 - *s2 : 0;
}

/** @ingroup common
 * @brief メモリ領域の各バイトを指定した値で埋める.
 * @param dst メモリ領域の先頭アドレス
 * @param ch セットする文字
 * @param len 長さ
 * @return メモリ領域の先頭アドレス
 */
void *memset(void *dst, int ch, size_t len) {
    uint8_t *d = dst;
    while (len-- > 0) {
        *d = ch;
        d++;
    }
    return dst;
}

/** @ingroup common
 * @brief メモリ領域をコピーする.
 * @param dst コピー先アドレス
 * @param src コピー元アドレス
 * @param len 長さ
 * @return メモリ領域の先頭アドレス
 */
void *memcpy(void *dst, const void *src, size_t len) {
    DEBUG_ASSERT(len < 256 * 1024 * 1024 /* 256MiB */
                 && "too long memcpy (perhaps integer overflow?)");

    uint8_t *d = dst;
    const uint8_t *s = src;
    while (len-- > 0) {
        *d = *s;
        d++;
        s++;
    }
    return dst;
}

/** @ingroup common
 * @brief メモリ領域をコピーする. 重なりがあっても正しく動作する。
 * @param dst コピー先アドレス
 * @param src コピー元アドレス
 * @param len 長さ
 * @return メモリ領域の先頭アドレス
 */
void *memmove(void *dst, const void *src, size_t len) {
    DEBUG_ASSERT(len < 256 * 1024 * 1024 /* 256MiB */
                 && "too long memmove (perhaps integer overflow?)");

    if ((uintptr_t) dst <= (uintptr_t) src) {
        memcpy(dst, src, len);
    } else {
        uint8_t *d = dst + len;
        const uint8_t *s = src + len;
        while (len-- > 0) {
            *d = *s;
            --d;
            --s;
        }
    }
    return dst;
}

/** @ingroup common
 * @brief 文字列の長さを返す.
 * @param s 文字列
 * @return 文字列長
 */
size_t strlen(const char *s) {
    size_t len = 0;
    while (*s != '\0') {
        len++;
        s++;
    }
    return len;
}

/** @ingroup common
 * @brief 文字列を比較する. 同じなら0を返す。
 * @param s1 文字列1
 * @param s2 文字列2
 * @return s1がs2に較べて 1)小さい、2)等しい、3)大きい場合に
 * ゼロよりも 1)小さい、2)等しい、3)大きい整数を返す。
 */
int strcmp(const char *s1, const char *s2) {
    while (true) {
        if (*s1 != *s2) {
            return *s1 - *s2;
        }

        if (*s1 == '\0') {
            return 0;
        }

        s1++;
        s2++;
    }

    return 0;
}

/** @ingroup common
 * @brief 指定した文字数まで文字列を比較する. 同じなら0を返す。
 * @param s1 文字列1
 * @param s2 文字列2
 * @param len 比較する文字列長
 * @return s1とs2の最初のlenバイトだけを比較することを除けば
 * strcmp()と同様である。
 */
int strncmp(const char *s1, const char *s2, size_t len) {
    while (len > 0) {
        if (*s1 != *s2) {
            return *s1 - *s2;
        }

        if (*s1 == '\0') {
            // Both `*s1` and `*s2` equal to '\0'.
            break;
        }

        s1++;
        s2++;
        len--;
    }

    return 0;
}

/** @ingroup common
 * @brief 文字列をコピーする. 宛先のバッファサイズを超える場合は、
 * バッファに収まるだけをコピーする。
 * @param dst コピー先バッファ
 * @param dst_len バッファ長
 * @param src コピー元文字列
 * @return コピー先バッファ
 */
char *strcpy_safe(char *dst, size_t dst_len, const char *src) {
    ASSERT(dst_len > 0);

    size_t i = 0;
    while (i < dst_len - 1 && src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }

    dst[i] = '\0';
    return dst;
}

/** @ingroup common
 * @brief 指定した文字を文字列から探し、その位置を返す.
 * @param str 文字列
 * @param c 文字
 * @return 文字列中の文字の位置、存在しない場合はNULL
 */
char *strchr(const char *str, int c) {
    char *s = (char *) str;
    while (*s != '\0') {
        if (*s == c) {
            return s;
        }

        s++;
    }

    return NULL;
}

/** @ingroup common
 * @brief 指定した文字列を文字列から探し、その位置を返す.
 * @param haystack 探される文字列
 * @param needle 探す文字列
 * @return 文字列中の文字列の位置、存在しない場合はNULL
 */
char *strstr(const char *haystack, const char *needle) {
    char *s = (char *) haystack;
    size_t needle_len = strlen(needle);
    while (*s != '\0') {
        if (!strncmp(s, needle, needle_len)) {
            return s;
        }

        s++;
    }

    return NULL;
}

/** @ingroup common
 * @brief 文字列を数値に変換する. 10進数のみ対応。
 * @param s 文字列
 * @return 文字列に対応する数値. 0-9以外の文字は無視する。
 */
int atoi(const char *s) {
    int x = 0;
    while ('0' <= *s && *s <= '9') {
        x = (x * 10) + (*s - '0');
        s++;
    }

    return x;
}
