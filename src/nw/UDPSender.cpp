#include "../include/UDPSender.h"
#include "../log/Logging.hpp"

#include <boost/asio.hpp>
#include <iostream>
#include <utility>

namespace merci::nw {
// Default constructor
UDPSender::UDPSender()
    : io_service(nullptr), socket(nullptr), remote(nullptr), error() {}
// Destructor
UDPSender::~UDPSender() {
    delete io_service;
    delete socket;
    delete remote;
}
// Copy constructor
UDPSender::UDPSender(const UDPSender &other) {
    if (this != &other) {
        io_service = new boost::asio::io_service();
        socket = new boost::asio::ip::udp::socket(*io_service);
        remote = new boost::asio::ip::udp::endpoint(
            boost::asio::ip::address(other.remote->address()),
            other.remote->port());
    }
}
// Copy assignment constructor
UDPSender &UDPSender::operator=(const UDPSender &other) {
    if (this != &other) {
        io_service = new boost::asio::io_service();
        socket = new boost::asio::ip::udp::socket(*io_service);
        remote = new boost::asio::ip::udp::endpoint(
            boost::asio::ip::address(other.remote->address()),
            other.remote->port());
    }
    return *this;
}
// Move constructor
UDPSender::UDPSender(UDPSender &&other) noexcept
    : io_service(std::move(other.io_service)),
      socket(std::move(other.socket)),
      remote(std::move(other.remote)),
      error(std::move(other.error)) {}
// Move assignment constructor
UDPSender &UDPSender::operator=(UDPSender &&other) noexcept {
    io_service = new boost::asio::io_service();
    socket = new boost::asio::ip::udp::socket(*io_service);
    remote = new boost::asio::ip::udp::endpoint(
        boost::asio::ip::address(other.remote->address()),
        other.remote->port());
    return *this;
}
void UDPSender::bind(std::string ip_addr, unsigned short port) {
    io_service = new boost::asio::io_service();
    socket = new boost::asio::ip::udp::socket(*io_service);
    remote = new boost::asio::ip::udp::endpoint(
        boost::asio::ip::address::from_string(ip_addr), port);
}
int UDPSender::send(std::string data_out) {
    merci::logging::TRACE("Sending...");
    socket->open(boost::asio::ip::udp::v4());
    auto sent =
        socket->send_to(boost::asio::buffer(data_out), *remote, 0, error);
    socket->close();
    return sent;
}
}    // namespace merci::nw