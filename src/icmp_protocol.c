#include "../include/icmp_protocol.h"
#include <string.h>
#include <sys/time.h>

/* Calculate Internet Checksum (RFC 1071) */
uint16_t icmp_checksum(const void* data, size_t length) {
    const uint16_t* buffer = (const uint16_t*)data;
    uint32_t sum = 0;

    // Check length > 1 byte
    while(length > 1) {
        sum += *buffer++;
        length -= 2;
    }

    // Check length == 1 byte
    if(length == 1) {
        sum += *(const uint8_t*)buffer;
    }

    // Check overflow
    while(sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    return (uint16_t)~sum;
}

void icmp_create_echo_request(struct icmp_packet* packet, uint16_t id, uint16_t sequence) {
    // Clear the packet
    memset(packet, 0, sizeof(struct icmp_packet));

    // Fill header
    packet->header.type = ICMP_ECHO_REQUEST;
    packet->header.code = 0;
    packet->header.id = id;
    packet->header.sequence = sequence;
    packet->header.checksum = 0; // calculate below

    // Set data as time value
    struct timeval tv;
    gettimeofday(&tv, NULL);
    memcpy(packet->data, &tv, sizeof(tv));

    // Fill the rest with pattern
    for(size_t i = sizeof(tv); i < ICMP_DATA_SIZE; i++) {
        packet->data[i] = (uint8_t)(i + 0x20);
    }

    packet->header.checksum = icmp_checksum(packet, sizeof(struct icmp_packet));
}

int icmp_validate_reply(const struct icmp_packet* packet, uint16_t expected_id, uint16_t expected_sequence) {
    if(ICMP_ECHO_REPLY != packet->header.type) {
        return FAILURE;
    }

    if(expected_id != packet->header.id) {
        return FAILURE;
    }

    if(expected_sequence != packet->header.sequence) {
        return FAILURE;
    }

    // Check checksum value
    uint16_t received_checksum = packet->header.checksum;
    struct icmp_packet temp = *packet;
    temp.header.checksum = 0;
    uint16_t calculated_checksum = icmp_checksum(&temp, sizeof(struct icmp_packet));

    if(received_checksum != calculated_checksum) {
        return FAILURE;
    }

    return SUCCESS;
}




