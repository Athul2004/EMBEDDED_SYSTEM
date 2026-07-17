#ifndef MQTT_SERVER_H
#define MQTT_SERVER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * Simple baremetal MQTT protocol packet constructor
 */

typedef struct {
    const char *client_id;
    const char *username;
    const char *password;
    uint16_t keep_alive;
} mqtt_connect_opt_t;

/**
 * @brief Build an MQTT CONNECT packet
 * @param buf Buffer to hold the packet
 * @param buf_len Size of the buffer
 * @param opts Connection options
 * @return Length of the constructed packet, or negative on error
 */
int mqtt_build_connect_packet(uint8_t *buf, size_t buf_len, const mqtt_connect_opt_t *opts);

/**
 * @brief Build an MQTT PUBLISH packet
 * @param buf Buffer to hold the packet
 * @param buf_len Size of the buffer
 * @param topic Topic to publish to
 * @param payload Payload data
 * @param payload_len Payload length
 * @param qos Quality of Service (0, 1, or 2)
 * @param retain Retain flag
 * @return Length of the constructed packet, or negative on error
 */
int mqtt_build_publish_packet(uint8_t *buf, size_t buf_len, const char *topic, const uint8_t *payload, size_t payload_len, uint8_t qos, bool retain);

/**
 * @brief Build an MQTT DISCONNECT packet
 * @param buf Buffer to hold the packet
 * @param buf_len Size of the buffer
 * @return Length of the constructed packet, or negative on error
 */
int mqtt_build_disconnect_packet(uint8_t *buf, size_t buf_len);

/**
 * @brief Build an MQTT SUBSCRIBE packet
 * @param buf Buffer to hold the packet
 * @param buf_len Size of the buffer
 * @param topic Topic to subscribe to
 * @param packet_id Packet ID for tracking
 * @return Length of the constructed packet, or negative on error
 */
int mqtt_build_subscribe_packet(uint8_t *buf, size_t buf_len, const char *topic, uint16_t packet_id);

#endif // MQTT_SERVER_H
