#include "socket.hpp"

#include <iostream>
#include <stdexcept>
#include "raw_socket.h"

namespace pingscan {

Socket::Socket() : _fd(-1) {
    _fd = create_raw_socket();
    if(_fd < 0) {
        throw std::runtime_error("Failed to create a raw socket!");
    }
    std::cout << "Socket created with fd: " << _fd << "\n";
}

Socket::Socket(Socket&& other) noexcept : _fd(other._fd) {
    other._fd = -1;
}

Socket& Socket::operator=(Socket&& other) noexcept {
    if(this != &other) {
        if(_fd >= 0) {
            close_socket(_fd);
        }
        _fd = other._fd;
        other._fd = -1;
    }
    return *this;
}

Socket::~Socket() {
    if(_fd >= 0) {
        close_socket(_fd);
    }
}

int Socket::get_fd() const noexcept {
    return _fd;
}




















}