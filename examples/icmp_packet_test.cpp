#include <iostream>
#include <iomanip>
#include <unistd.h>
#include "icmp_protocol.h"

void print_hex(const uint8_t* data, size_t length) {
    for (size_t i = 0; i < length; i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                  << static_cast<int>(data[i]) << " ";
        if ((i + 1) % 16 == 0) std::cout << "\n";
    }
    std::cout << std::dec << "\n";
}

int main() {
    std::cout << "=== ICMP Packet Test ===\n\n";
    
    struct icmp_packet packet;
    uint16_t pid = static_cast<uint16_t>(getpid());
    uint16_t seq = 0;
    
    icmp_create_echo_request(&packet, pid, seq);
    
    // Print header info
    std::cout << "ICMP Header:\n";
    std::cout << "  Type:     " << static_cast<int>(packet.header.type) << " (Echo Request)\n";
    std::cout << "  Code:     " << static_cast<int>(packet.header.code) << "\n";
    std::cout << "  Checksum: 0x" << std::hex << packet.header.checksum << std::dec << "\n";
    std::cout << "  ID:       " << packet.header.id << " (PID)\n";
    std::cout << "  Sequence: " << packet.header.sequence << "\n\n";
    
    std::cout << "Full Packet (hex):\n";
    print_hex(reinterpret_cast<uint8_t*>(&packet), sizeof(packet));
    
    std::cout << "\nValidation Test:\n";
    
    // Wrong type test
    if (icmp_validate_reply(&packet, pid, seq)) {
        std::cout << "  Validation failed (expected - this is request, not reply)\n";
    } else {
        std::cout << "  Validation correctly rejected\n";
    }
    
    // Correct reply test
    packet.header.type = ICMP_ECHO_REPLY;
    packet.header.checksum = 0;
    packet.header.checksum = icmp_checksum(&packet, sizeof(packet));
    
    if (icmp_validate_reply(&packet, pid, seq)) {
        std::cout << "  Validation passed\n";
    } else {
        std::cout << "  Validation failed\n";
    }
    
    return 0;
}