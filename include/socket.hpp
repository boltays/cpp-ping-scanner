#pragma once

namespace pingscan {


class Socket {
public:
    Socket();
    ~Socket();

    Socket(const Socket&) = delete;
    Socket(Socket&&) noexcept;
    Socket& operator=(const Socket&) = delete;
    Socket& operator=(Socket&&) noexcept;

    int get_fd() const noexcept;

private:
    int _fd{};

};






}