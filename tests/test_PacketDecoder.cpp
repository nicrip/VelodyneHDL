#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "PacketDriver.h"
#include "PacketDecoder.h"
#include <boost/shared_ptr.hpp>
#include<deque>

using namespace std;

int main()
{
  PacketDriver driver;
  driver.InitPacketDriver(DATA_PORT);
  PacketDecoder decoder;
  decoder.SetCorrectionsFile("../32db.xml");

  std::string* data = new std::string();
  unsigned int* dataLength = new unsigned int();
  std::deque<PacketDecoder::HDLFrame> frames;
  PacketDecoder::HDLFrame latest_frame;
  while (true) {
    driver.GetPacket(data, dataLength);
    decoder.DecodePacket(data, dataLength);
    frames = decoder.GetFrames();
    if (decoder.GetLatestFrame(&latest_frame)) {
      std::cout << "Number of points: " << latest_frame.x.size() << std::endl;
    }
  }

  return 0;
}
