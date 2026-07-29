#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <chrono>
#include "icmp_protocol.h"
#include "raw_socket.h"
#include "socket.hpp"

int main() {

    try {
        pingscan::Socket sock;
        std::cout << "Socket fd: " << sock.get_fd() << "\n";

        // Create ICMP packet
        struct icmp_packet packet;
        uint16_t pid = static_cast<uint16_t>(getpid());
        uint16_t seq = 0;
    
        icmp_create_echo_request(&packet, pid, seq);
    
        const char* target_ip = "8.8.8.8";
        std::cout << "Pinging " << target_ip << "...\n";
    
        // Record send time
        auto start = std::chrono::steady_clock::now();

        // Send packet
        int sent = send_icmp_packet(sock.get_fd(), &packet, target_ip);
        if (sent < 0) {
            std::cerr << "ERROR: Send failed\n";
            return 1;
        }
        std::cout << "Sent " << sent << " bytes\n";
    
        // Receive reply
        char src_ip[IP_ADDR_MAX_LEN];
        struct icmp_packet reply;
    
        std::cout << "Waiting for reply (3 second timeout)...\n";
        int result = recv_icmp_packet(sock.get_fd(), &reply, src_ip, 3000);
    
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

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    std::cout << "Test done, socket should be closed\n\n";

    
    return 0;
}