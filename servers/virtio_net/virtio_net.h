/** @file virtio_net.h */
#pragma once
#include <libs/common/list.h>
#include <libs/common/message.h>
#include <libs/common/types.h>

#define NUM_TX_BUFFERS             128
#define NUM_RX_BUFFERS             128
#define VIRTIO_NET_MAX_PACKET_SIZE 1514

#define VIRTIO_NET_F_MAC       (1 << 5)
#define VIRTIO_NET_F_MRG_RXBUF (1 << 15)
#define VIRTIO_NET_F_STATUS    (1 << 16)
#define VIRTIO_NET_QUEUE_RX    0
#define VIRTIO_NET_QUEUE_TX    1

/** @ingropu virtio_net
 * @struct virtio_net_config
 * @brief デバイス固有のコンフィグ領域 (MMIO)
 */
struct virtio_net_config {
    uint8_t macaddr[6];             /**< MACアドレス */
    uint16_t status;                /**< ステータス */
    uint16_t max_virtqueue_pairs;   /**< 最大virtqueueペア数 */
    uint16_t mtu;                   /**< MTU */
} __packed;

#define VIRTIO_NET_HDR_GSO_NONE 0
/** @ingropu virtio_net
 * @struct virtio_net_header
 * @brief 処理要求のヘッダ
 */
struct virtio_net_header {
    uint8_t flags;                  /**< フラグ */
    uint8_t gso_type;               /**< GSO種別 */
    uint16_t hdr_len;               /**< ヘッダー長 */
    uint16_t gso_size;              /**< GSOサイズ */
    uint16_t checksum_start;        /**< チェックサム開始位置 */
    uint16_t checksum_offset;       /**< チェックサムお布施と */
} __packed;

/** @ingropu virtio_net
 * @struct virtio_net_req
 * @brief virtio-netデバイスへの処理要求
 */
struct virtio_net_req {
    struct virtio_net_header header;                /**< リクエストヘッダー */
    uint8_t payload[VIRTIO_NET_MAX_PACKET_SIZE];    /**< リクエストデータ */
} __packed;
