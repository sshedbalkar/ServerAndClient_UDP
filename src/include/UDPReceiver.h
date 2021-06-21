#include <boost/asio.hpp>

namespace merci::nw {
class UDPReceiver {
   public:
    UDPReceiver();
    ~UDPReceiver();
    UDPReceiver(const UDPReceiver &other);
    UDPReceiver &operator=(const UDPReceiver &other);
    UDPReceiver(UDPReceiver &&other) noexcept;
    UDPReceiver &operator=(UDPReceiver &&other) noexcept;
    //
    void handle_receive(const boost::system::error_code &error,
                        size_t bytes_transferred);
    void wait();
    void receiver(const std::string &ip_addr, unsigned short port);

   private:
    boost::asio::io_service *io_service;
    boost::asio::ip::udp::socket *socket;
    std::array<char, 1024> *recv_buffer;
    boost::asio::ip::udp::endpoint *remote;
};
}    // namespace merci::nw