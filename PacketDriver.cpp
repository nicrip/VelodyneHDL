// Velodyne HDL Packet Driver
// Nick Rypkema (rypkema@mit.edu), MIT 2017
// shared library to read a Velodyne HDL packet streaming over UDP

#include "PacketDriver.h"
#include <boost/bind.hpp>

using boost::asio::ip::udp;

PacketDriver::PacketDriver()
{

}

PacketDriver::PacketDriver(unsigned int port) : _port(port)
{
  boost::asio::ip::udp::endpoint destination_endpoint(boost::asio::ip::address_v4::any(), _port);

  try {
    _socket = boost::shared_ptr<boost::asio::ip::udp::socket>(new boost::asio::ip::udp::socket(_io_service));
    _socket->open(destination_endpoint.protocol());
    _socket->bind(destination_endpoint);
  } catch(std::exception & e) {
    std::cout << "PacketDriver: Error binding to socket - " << e.what() << ". Trying once more..." << std::endl;
    try {
      destination_endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4::any(), _port);
      _socket = boost::shared_ptr<boost::asio::ip::udp::socket>(new boost::asio::ip::udp::socket(_io_service));
      _socket->open(destination_endpoint.protocol());
      _socket->bind(destination_endpoint);
    } catch(std::exception & e) {
      std::cout << "PacketDriver: Error binding to socket - " << e.what() << ". Failed!" << std::endl;
      return;
    }
  }

  std::cout << "PacketDriver: Success binding to Velodyne socket!" << std::endl;
  return;
}

PacketDriver::~PacketDriver()
{
  _socket->close();
  _io_service.stop();
}

void PacketDriver::InitPacketDriver(unsigned int port)
{
  _port = port;

  boost::asio::ip::udp::endpoint destination_endpoint(boost::asio::ip::address_v4::any(), _port);

  try {
    _socket = boost::shared_ptr<boost::asio::ip::udp::socket>(new boost::asio::ip::udp::socket(_io_service));
    _socket->open(destination_endpoint.protocol());
    _socket->bind(destination_endpoint);
  } catch(std::exception & e) {
    std::cout << "PacketDriver: Error binding to socket - " << e.what() << ". Trying once more..." << std::endl;
    try {
      destination_endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4::any(), _port);
      _socket = boost::shared_ptr<boost::asio::ip::udp::socket>(new boost::asio::ip::udp::socket(_io_service));
      _socket->open(destination_endpoint.protocol());
      _socket->bind(destination_endpoint);
    } catch(std::exception & e) {
      std::cout << "PacketDriver: Error binding to socket - " << e.what() << ". Failed!" << std::endl;
      return;
    }
  }

  std::cout << "PacketDriver: Success binding to Velodyne socket!" << std::endl;
  return;
}

bool PacketDriver::GetPacket(std::string* data, unsigned int* data_length)
{
  try {
    _socket->async_receive(boost::asio::buffer(_rx_buffer, 1500),
    boost::bind(&PacketDriver::GetPacketCallback, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, data, data_length));
    _io_service.reset();
    _io_service.run();
    return (true);
  } catch(std::exception & e) {
    std::cout << "PacketDriver: Error receiving packet - " << e.what() << "." << std::endl;
    return(false);
  }
}

void PacketDriver::GetPacketCallback(const boost::system::error_code& error, std::size_t num_bytes, std::string* data, unsigned int* data_length)
{
  (*data).assign(_rx_buffer, num_bytes);
  *data_length = (unsigned int) num_bytes;
  return;
}

