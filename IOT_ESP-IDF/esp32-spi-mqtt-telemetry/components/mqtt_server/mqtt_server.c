#include "mqtt_server.h"
#include <string.h>

// Helper to write UTF-8 encoded strings
static int write_utf8_string(uint8_t *buf, const char *str) {
    if (!str) return 0;
    size_t len = strlen(str);
    buf[0] = (len >> 8) & 0xFF;
    buf[1] = len & 0xFF;
    memcpy(&buf[2], str, len);
    return len + 2;
}

int mqtt_build_connect_packet(uint8_t *buf, size_t buf_len, const mqtt_connect_opt_t *opts) {
    if (!buf || !opts || !opts->client_id) return -1;
    
    uint8_t variable_header[] = {
        0x00, 0x04, 'M', 'Q', 'T', 'T', // Protocol Name
        0x04,                           // Protocol Level (MQTT 3.1.1)
        0x02,                           // Connect Flags (Clean session)
        (opts->keep_alive >> 8) & 0xFF, // Keep Alive MSB
        opts->keep_alive & 0xFF         // Keep Alive LSB
    };
    
    // Update connect flags if username/password are provided
    if (opts->username) variable_header[7] |= 0x80;
    if (opts->password) variable_header[7] |= 0x40;

    size_t var_hdr_len = sizeof(variable_header);
    
    size_t payload_len = 2 + strlen(opts->client_id);
    if (opts->username) payload_len += 2 + strlen(opts->username);
    if (opts->password) payload_len += 2 + strlen(opts->password);
    
    size_t remaining_length = var_hdr_len + payload_len;
    
    // Check buffer size (assuming remaining length < 128 for simplicity in baremetal)
    if (1 + 1 + remaining_length > buf_len) return -1;
    if (remaining_length > 127) return -2; // This simple builder only supports remaining length up to 127 bytes
    
    int offset = 0;
    buf[offset++] = 0x10; // CONNECT Packet type
    buf[offset++] = (uint8_t)remaining_length;
    
    memcpy(&buf[offset], variable_header, var_hdr_len);
    offset += var_hdr_len;
    
    offset += write_utf8_string(&buf[offset], opts->client_id);
    
    if (opts->username) {
        offset += write_utf8_string(&buf[offset], opts->username);
    }
    if (opts->password) {
        offset += write_utf8_string(&buf[offset], opts->password);
    }
    
    return offset;
}

int mqtt_build_publish_packet(uint8_t *buf, size_t buf_len, const char *topic, const uint8_t *payload, size_t payload_len, uint8_t qos, bool retain) {
    if (!buf || !topic || !payload) return -1;
    
    size_t topic_len = strlen(topic);
    size_t remaining_length = 2 + topic_len + payload_len;
    
    if (qos > 0) remaining_length += 2; // Packet identifier
    
    if (remaining_length > 127) return -2; // Simplify remaining length encoding for baremetal
    
    if (1 + 1 + remaining_length > buf_len) return -1;
    
    int offset = 0;
    uint8_t header = 0x30; // PUBLISH type
    if (retain) header |= 0x01;
    header |= (qos & 0x03) << 1;
    
    buf[offset++] = header;
    buf[offset++] = (uint8_t)remaining_length;
    
    offset += write_utf8_string(&buf[offset], topic);
    
    if (qos > 0) {
        // Packet ID = 1 for simplicity
        buf[offset++] = 0x00;
        buf[offset++] = 0x01;
    }
    
    memcpy(&buf[offset], payload, payload_len);
    offset += payload_len;
    
    return offset;
}

int mqtt_build_disconnect_packet(uint8_t *buf, size_t buf_len) {
    if (buf_len < 2) return -1;
    int idx = 0;
    // Fixed Header
    buf[idx++] = 0xE0;
    buf[idx++] = 0x00;
    
    return idx;
}

int mqtt_build_subscribe_packet(uint8_t *buf, size_t buf_len, const char *topic, uint16_t packet_id) {
    if (!buf || !topic) return -1;
    
    size_t topic_len = strlen(topic);
    // Payload: Packet ID (2) + Topic Length (2) + Topic + Requested QoS (1)
    size_t remaining_length = 2 + 2 + topic_len + 1;
    
    // Simple bounds check
    if (buf_len < 5 + remaining_length) return -1; 
    
    int idx = 0;
    
    // Fixed Header: 0x82 (SUBSCRIBE, QoS 1)
    buf[idx++] = 0x82;
    
    // Remaining Length
    if (remaining_length > 127) return -2;
    buf[idx++] = (uint8_t)remaining_length;
    
    // Packet ID
    buf[idx++] = (packet_id >> 8) & 0xFF;
    buf[idx++] = packet_id & 0xFF;
    
    // Topic Length + Topic string
    idx += write_utf8_string(&buf[idx], topic);
    
    // Requested QoS (QoS 0)
    buf[idx++] = 0x00;
    
    return idx;
}
