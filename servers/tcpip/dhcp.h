/** @file dhcp.h */
#pragma once
#include "ethernet.h"
#include <libs/common/list.h>

#define BOOTP_OP_REQUEST      1           // BOOTPリクエスト
#define BOOTP_HWTYPE_ETHERNET 1           // イーサネット
#define DHCP_MAGIC            0x63825363  // DHCPを示すマジックナンバー

/** @ingroup tcpip
 * @enum dhcp_option
 * @brief DHCPオプションの種類
 */
enum dhcp_option {
    DHCP_OPTION_NETMASK = 1,            /**< = 1: ネットマスク */
    DHCP_OPTION_ROUTER = 3,             /**< = 3: デフォルトゲートウェイ */
    DHCP_OPTION_DNS = 6,                /**< = 6: DNSキャッシュサーバ */
    DHCP_OPTION_REQUESTED_ADDR = 50,    /**< = 50: 要求するIPアドレス */
    DHCP_OPTION_DHCP_TYPE = 53,         /**< = 53: DHCPパケットの種類 */
    DHCP_OPTION_PARAM_LIST = 55,        /**< = 55: DHCPサーバに要求する追加情報 */
    DHCP_OPTION_END = 255,              /**< =255: オプションの終了マーカー */
};

/** @ingroup tcpip
 * @enum dhcp_type
 * @brief DHCPパケットの種類
 */
enum dhcp_type {
    DHCP_TYPE_DISCOVER = 1,     /**< = 1: DHCP DISCOVER */
    DHCP_TYPE_OFFER = 2,        /**< = 2: DHCP OFFER */
    DHCP_TYPE_REQUEST = 3,      /**< = 3: DHCP REQUEST */
    DHCP_TYPE_ACK = 5,          /**< = 5: DHCP ACK */
};

/** @ingroup tcpip
 * @struct dhcp_header
 * @brief DHCPパケット
 */
struct dhcp_header {
    uint8_t op;                 /**< 要求か応答か */
    uint8_t hw_type;            /**< ハードウェアアドレスの種類 */
    uint8_t hw_len;             /**< ハードウェアアドレスの長さ */
    uint8_t hops;               /**< ルータを経由した回数 */
    uint32_t xid;               /**< トランザクションID */
    uint16_t secs;              /**< クライアントがアドレスの取得処理を開始してからの秒数 */
    uint16_t flags;             /**< フラグ */
    uint32_t client_ipaddr;     /**< クライアントのIPアドレス */
    uint32_t your_ipaddr;       /**< クライアントに割り当てるIPアドレス */
    uint32_t server_ipaddr;     /**< TFTPサーバのIPアドレス */
    uint32_t gateway_ipaddr;    /**< DHCPパケットを転送したノード (参考: DHCPリレー) */
    macaddr_t client_hwaddr;    /**< クライアントのMACアドレス */
    uint8_t unused[202];        /**< 未使用領域 */
    uint32_t magic;             /**< DHCPを示すマジックナンバー */
    uint8_t options[];          /**< DHCPオプション: いわゆるTLV (Type-Length-Value) 形式 */
} __packed;

/** @ingroup tcpip
 * @struct dhcp_type_option
 * @brief DHCPオプション: DHCPパケットの種類 (DHCP_OPTION_DHCP_TYPE)
 */
struct dhcp_type_option {
    uint8_t dhcp_type;          /**< DHCPパケットの種類 */
} __packed;

/** @ingroup tcpip
 * @struct dhcp_netmask_option
 * @brief DHCPオプション: ネットマスク (DHCP_OPTION_NETMASK)
 */
struct dhcp_netmask_option {
    uint32_t netmask;           /**< ネットマスク */
} __packed;

/** @ingroup tcpip
 * @struct dhcp_router_option
 * @brief DHCPオプション: デフォルトゲートウェイ (DHCP_OPTION_ROUTER)
 */
struct dhcp_router_option {
    uint32_t router;            /**< デフォルトゲートウェイ */
} __packed;

/** @ingroup tcpip
 * @struct dhcp_dns_option
 * @brief DHCPオプション: DNSキャッシュサーバ (DHCP_OPTION_DNS)
 */
struct dhcp_dns_option {
    uint32_t dns;               /**< DNS */
} __packed;

#define DHCP_PARMAS_MAX 3
/** @ingroup tcpip
 * @struct dhcp_params_option
 * @brief DHCPオプション: DHCPサーバに要求する追加情報 (DHCP_OPTION_PARAM_LIST)
 */
struct dhcp_params_option {
    uint8_t params[DHCP_PARMAS_MAX];    /**< 追加情報 */
} __packed;

/** @ingroup tcpip
 * @struct dhcp_reqaddr_option
 * @brief DHCPオプション: 要求するIPアドレス (DHCP_OPTION_REQUESTED_ADDR)
 */
//
struct dhcp_reqaddr_option {
    uint32_t ipaddr;            /**< IPアドレス */
} __packed;

void dhcp_transmit(enum dhcp_type type, ipv4addr_t requested_addr);
void dhcp_receive(void);
void dhcp_init(void);
