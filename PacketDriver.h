// Velodyne HDL Packet Driver
// Nick Rypkema (rypkema@mit.edu), MIT 2017
// shared library to read a Velodyne HDL packet streaming over UDP

#ifndef PACKET_DRIVER_H_INCLUDED
#define PACKET_DRIVER_H_INCLUDED

#include <boost/asio.hpp>

static unsigned int DATA_PORT = 2368;

class PacketDriver
{
public:
  PacketDriver(unsigned int port);
  virtual ~PacketDriver();
  bool GetPacket(std::string* data, unsigned int* data_length);

protected:
  void GetPacketCallback(const boost::system::error_code& error, std::size_t num_bytes, std::string* data, unsigned int* data_length);

private:
  unsigned int _port;
  char _rx_buffer[1500];
  boost::asio::io_service _io_service;
  boost::shared_ptr<boost::asio::ip::udp::socket> _socket;
};

#endif // PACKET_DRIVER_H_INCLUDED
