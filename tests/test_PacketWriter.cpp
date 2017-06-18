#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "PacketDriver.h"
#include "PacketFileWriter.h"
#include <boost/shared_ptr.hpp>

using namespace std;

int main()
{
  PacketDriver driver;
  driver.InitPacketDriver(DATA_PORT);
  vtkPacketFileWriter writer;
  if (!writer.Open("./test.pcap")) {
    std::cerr << "Could not open pcap file to write" << std::endl;
  }

  std::string* data = new std::string();
  unsigned int* dataLength = new unsigned int();
  while (true) {
    driver.GetPacket(data, dataLength);
    std::cout << "Length of packet: " << (*data).length() << std::endl;
    writer.WritePacket(reinterpret_cast<const unsigned char*>(data->c_str()), data->length());
  }

  return 0;
}
