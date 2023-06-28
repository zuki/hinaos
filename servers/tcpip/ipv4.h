/** @file ipv4.h */
#pragma once
#include "mbuf.h"

// 送信パケットのTTLフィールドのデフォルト値
#define DEFAULT_TTL 32

/** @ingroup tcpip
 * @typedef ipv4addr_t
 * @brief IPv4アドレスを表す型 */
typedef uint32_t ipv4addr_t;

// ブロードキャストアドレス (255.255.255.255)
#define IPV4_ADDR_BROADCAST 0xffffffff
// ポインタに対するNULLのような「未指定」を表すアドレス (0.0.0.0)
#define IPV4_ADDR_UNSPECIFIED 0

/** @ingroup tcpip
 * @enum ip_proto_type
 * @brief IPv4パケットの上位プロトコル */
enum ip_proto_type {
    IPV4_PROTO_TCP = 0x06,      /**< = 0x06: TCP */
    IPV4_PROTO_UDP = 0x11,      /**< = 0x11: UDP */
};

/** @ingroup tcpip
 * @typedef port_t
 * @brief ポート番号を表す型 */
typedef uint16_t port_t;

/** @ingroup tcpip
 * @typedef endpoint_t
 * @brief IPv4アドレスとポート番号のペア */
typedef struct endpoint {
    ipv4addr_t addr;
    port_t port;
} endpoint_t;

/** @ingroup tcpip
 * @struct ipv4_header
 * @brief IPv4ヘッダ */
struct ipv4_header {
    uint8_t ver_ihl;          /**< バージョンとヘッダ長 */
    uint8_t dscp_ecn;         /**< DSCPとECN */
    uint16_t len;             /**< IPv4ペイロード長 */
    uint16_t id;              /**< 識別子 */
    uint16_t flags_frag_off;  /**< フラグとフラグメントオフセット */
    uint8_t ttl;              /**< TTL */
    uint8_t proto;            /**< 上位プロトコル */
    uint16_t checksum;        /**< チェックサム */
    uint32_t src_addr;        /**< 送信元IPv4アドレス */
    uint32_t dst_addr;        /**< 宛先IPv4アドレス */
} __packed;

void ipv4_transmit(ipv4addr_t dst, uint8_t proto, mbuf_t payload);
void ipv4_receive(mbuf_t pkt);
