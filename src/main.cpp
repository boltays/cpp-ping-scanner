#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>

int main() {

    std::cout << "=== RAW SOCKET TEST===" << "\n";

    // Create raw socket for ICMP
    auto sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    if(sock < 0) {
        std::cerr << "Error creating socket: " << strerror(errno) << "\n";
        std::cerr << "Note: Raw sockets require root privileges" << "\n";
        std::cerr << "Run with: sudo ./ping_scanner" << "\n";
        return 1;        
    }

    std::cout << "Raw socket created successfuly (fd: " << sock << ")" << "\n";

    close(sock);
    std::cout << "Socket closed." << "\n";

    return 0;

}