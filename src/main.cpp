#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <chrono>
#include "icmp_protocol.h"
#include "raw_socket.h"

int main() {
    std::cout << "=== Real Ping Test ===\n\n";
    
    const char* target_ip = "8.8.8.8";
    
    // Create socket
    std::cout << "Creating raw socket...\n";
    int sock = create_raw_socket();
    if (sock < 0) {
        std::cerr << "ERROR: Failed to create socket\n";
        return 1;
    }
    std::cout << "Socket created fd: " << sock << "\n\n";
    
    // Create ICMP packet
    struct icmp_packet packet;
    uint16_t pid = static_cast<uint16_t>(getpid());
    uint16_t seq = 0;
    
    icmp_create_echo_request(&packet, pid, seq);
    
    std::cout << "Pinging " << target_ip << "...\n";
    
    // Record send time
    auto start = std::chrono::steady_clock::now();
    
    // Send packet
    int sent = send_icmp_packet(sock, &packet, target_ip);
    if (sent < 0) {
        std::cerr << "ERROR: Send failed\n";
        close_socket(sock);
        return 1;
    }
    std::cout << "Sent " << sent << " bytes\n";
    
    // Receive reply
    char src_ip[IP_ADDR_MAX_LEN];
    struct icmp_packet reply;
    
    std::cout << "Waiting for reply (3 second timeout)...\n";
    int result = recv_icmp_packet(sock, &reply, src_ip, 3000);
    
    // Record receive time
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    if (result > 0) {
        std::cout << "\n SUCCESS!\n";
        std::cout << "Reply from: " << src_ip << "\n";
        std::cout << "Bytes received: " << result << "\n";
        std::cout << "Time: " << duration.count() << " ms\n";
        std::cout << "Type: " << static_cast<int>(reply.header.type) << " (";
        if (reply.header.type == ICMP_ECHO_REPLY) {
            std::cout << "Echo Reply";
        } else {
            std::cout << "Unknown";
        }
        std::cout << ")\n";
        std::cout << "ID: " << reply.header.id << "\n";
        std::cout << "Sequence: " << reply.header.sequence << "\n";
    } else if (result == 0) {
        std::cout << "\nTIMEOUT\n";
        std::cout << "No reply within 3 seconds\n";
    } else {
        std::cerr << "\nERROR\n";
        std::cerr << "Receive failed\n";
    }
    
    // Close socket
    close_socket(sock);
    std::cout << "\nSocket closed.\n";
    
    return 0;
}