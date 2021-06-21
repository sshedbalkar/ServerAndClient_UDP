#include "../include/UDPReceiver.h"
#include "../log/Logging.hpp"

#include <array>
#include <boost/asio.hpp>
#include <iostream>
#include <boost/bind.hpp>

namespace merci::nw {
// Default constructor
UDPReceiver::UDPReceiver()
    : io_service(nullptr),
      socket(nullptr),
      remote(nullptr),
      recv_buffer(nullptr) {
    io_service = new boost::asio::io_service();
    socket = new boost::asio::ip::udp::socket(*io_service);
    remote = new boost::asio::ip::udp::endpoint();
    recv_buffer = new std::array<char, 1024>();
}
// Destructor
UDPReceiver::~UDPReceiver() {
    delete io_service;
    delete socket;
    delete remote;
    delete[] recv_buffer;
}
// Copy constructor
UDPReceiver::UDPReceiver(const UDPReceiver &other) {
    if (this != &other) {
        io_service = new boost::asio::io_service();
        socket = new boost::asio::ip::udp::socket(*io_service);
        remote = new boost::asio::ip::udp::endpoint(
            boost::asio::ip::address(other.remote->address()),
            other.remote->port());
        recv_buffer = new std::array<char, 1024>();
    }
}
// Copy assignment constructor
UDPReceiver &UDPReceiver::operator=(const UDPReceiver &other) {
    if (this != &other) {
        io_service = new boost::asio::io_service();
        socket = new boost::asio::ip::udp::socket(*io_service);
        remote = new boost::asio::ip::udp::endpoint(
            boost::asio::ip::address(other.remote->address()),
            other.remote->port());
        recv_buffer = new std::array<char, 1024>(*other.recv_buffer);
    }
    return *this;
}
// Move constructor
UDPReceiver::UDPReceiver(UDPReceiver &&other) noexcept
    : io_service(std::move(other.io_service)),
      socket(std::move(other.socket)),
      remote(std::move(other.remote)),
      recv_buffer(std::move(other.recv_buffer)) {}
// Move assignment constructor
UDPReceiver &UDPReceiver::operator=(UDPReceiver &&other) noexcept {
    io_service = new boost::asio::io_service();
    socket = new boost::asio::ip::udp::socket(*io_service);
    remote = new boost::asio::ip::udp::endpoint(
        boost::asio::ip::address(other.remote->address()),
        other.remote->port());
    recv_buffer = new std::array<char, 1024>(*other.recv_buffer);
    return *this;
}
void UDPReceiver::handle_receive(const boost::system::error_code &error,
                                 size_t bytes_transferred) {
    if (error) {
        std::ostringstream buffer;
        buffer << "Receive failed: " << error.message();
        merci::logging::ERROR(buffer.str());
        return;
    }
    std::ostringstream buffer;
    buffer << "Received: '"
           << std::string(recv_buffer->begin(),
                          recv_buffer->begin() + bytes_transferred)
           << "' (" << error.message();
    merci::logging::INFO(buffer.str());
    wait();
}
void UDPReceiver::wait() {
    socket->async_receive_from(
        boost::asio::buffer(*recv_buffer), *remote,
        boost::bind(&UDPReceiver::handle_receive, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}
void UDPReceiver::receiver(const std::string &ip_addr, unsigned short port) {
    socket->open(boost::asio::ip::udp::v4());
    socket->bind(boost::asio::ip::udp::endpoint(
        boost::asio::ip::address::from_string(ip_addr.c_str()), port));

    wait();
    io_service->run();
}
}    // namespace merci::nw