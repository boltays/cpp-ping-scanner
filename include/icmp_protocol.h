#ifndef ICMP_PROTOCOL_H
#define ICMP_PROTOCOL_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ICMP_ECHO_REPLY 0
#define ICMP_ECHO_REQUEST 8

// ICMP data size : 56 byte
#define ICMP_DATA_SIZE 56

#define SUCCESS 1
#define FAILURE 0

// ICMP header : 8 byte
struct icmp_header {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t id;
    uint16_t sequence;
};

// ICMP echo packet (header + data) : 64 byte
struct icmp_packet {
    struct icmp_header header;
    uint8_t data[ICMP_DATA_SIZE];
};

uint16_t icmp_checksum(const void* data, size_t length);
void icmp_create_echo_request(struct icmp_packet* packet, uint16_t id, uint16_t sequence);
int icmp_validate_reply(const struct icmp_packet* packet, uint16_t expected_id, uint16_t expected_sequence);

#ifdef __cplusplus
}
#endif

#endif /*ICMP_PROTOCOL_H*/