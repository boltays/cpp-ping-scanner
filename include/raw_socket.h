#ifndef c
#define RAW_SOCKET_H

#include <stdint.h>
#include <stddef.h>
#include "icmp_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IP_ADDR_MAX_LEN 16

int create_raw_socket(void);
int send_icmp_packet(int sockfd, const struct icmp_packet* packet, const char* dest_ip);
int recv_icmp_packet(int sockfd, struct icmp_packet* packet, char* src_ip, int timeout_ms);
void close_socket(int sockfd);

#ifdef __cplusplus
}
#endif

#endif /* RAW_SOCKET_H */