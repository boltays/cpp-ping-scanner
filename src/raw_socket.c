#include "../include/raw_socket.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/select.h>

int create_raw_socket(void) {
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if(sockfd < 0) {
        return -1;
    }

    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        close(sockfd);
        return -1;
    }
    return sockfd;
}

int send_icmp_packet(int sockfd, const struct icmp_packet* packet, const char* dest_ip) {
    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));

    dest_addr.sin_family = AF_INET;

    if(inet_pton(AF_INET, dest_ip, &dest_addr.sin_addr) <= 0) {
        return -1; // invalid ip address
    }

    // send packet
    ssize_t sent = sendto(sockfd, packet, sizeof(struct icmp_packet), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
    if(sent < 0) {
        return -1;
    }
    return (int)sent;
}

int recv_icmp_packet(int sockfd, struct icmp_packet* packet, char* src_ip, int timeout_ms) {
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(sockfd, &read_fds);

    struct timeval timeout;
    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;
    int ready = select(sockfd + 1, &read_fds, NULL, NULL, &timeout);
    if(ready < 0) {
        return -1;
    } else if(ready == 0) {
        return 0;
    }

    // Receive data
    struct sockaddr_in src_addr;
    socklen_t addr_len = sizeof(src_addr);
    uint8_t buffer[1024];
    ssize_t received = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&src_addr, &addr_len);
    if(received < 0) {
        return -1;
    }

    // Extract IP header size
    int ip_header_len = (buffer[0] & 0x0F) * 4;

    // Check if enough data
    if(received < ip_header_len + (int)sizeof(struct icmp_packet)) {
        return -1;
    }

    // Copy ICMP packet skipping the IP header and convert source IP to str
    memcpy(packet, buffer + ip_header_len, sizeof(struct icmp_packet));
    if(inet_ntop(AF_INET, &src_addr.sin_addr, src_ip, IP_ADDR_MAX_LEN) == NULL) {
        return -1;
    }
    return (int)(received - ip_header_len);
}

void close_socket(int sockfd) {
    if(sockfd >= 0) {
        close(sockfd);
    }
}













