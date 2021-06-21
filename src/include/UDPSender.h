#pragma once

#include <string>
#include <boost/asio.hpp>

namespace merci::nw {
class UDPSender {
   public:
    UDPSender();
    ~UDPSender();
    UDPSender(const UDPSender &other);
    UDPSender &operator=(const UDPSender &other);
    UDPSender(UDPSender &&other) noexcept;
    UDPSender &operator=(UDPSender &&other) noexcept;
    //
    void bind(std::string ip_addr, unsigned short port);
    int send(std::string data_out);

   private:
    boost::asio::io_service* io_service;
    boost::asio::ip::udp::socket* socket;
    boost::asio::ip::udp::endpoint* remote;
    boost::system::error_code error;
};
}    // namespace merci::nw