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
  PacketDriver driver(DATA_PORT);
  PacketDecoder decoder;

  std::string* data = new std::string();
  unsigned int* dataLength = new unsigned int();
  while (true) {
    driver.GetPacket(data, dataLength);
    decoder.DecodePacket(data, dataLength);
    std::deque<PacketDecoder::HDLFrame> frames = decoder.GetFrames();
    if (frames.size()) {
      PacketDecoder::HDLFrame latest_frame = frames.back();
      std::cout << "Number of points: " << latest_frame.x.size() << std::endl;
      decoder.ClearFrames();
    }
  }

	return 0;
}
