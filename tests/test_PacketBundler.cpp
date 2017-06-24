#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "PacketDriver.h"
#include "PacketBundler.h"
#include <boost/shared_ptr.hpp>
#include<deque>

using namespace std;

int main()
{
  PacketDriver driver;
  driver.InitPacketDriver(DATA_PORT);
  PacketBundler bundler;

  std::string* data = new std::string();
  unsigned int* dataLength = new unsigned int();
  std::deque<std::string> bundles;
  std::string latest_bundle;
  unsigned int latest_bundle_length;
  while (true) {
    driver.GetPacket(data, dataLength);
    bundler.BundlePacket(data, dataLength);
    bundles = bundler.GetBundles();
    if (bundler.GetLatestBundle(&latest_bundle, &latest_bundle_length)) {
      std::cout << "Bundle length: " << latest_bundle_length << std::endl;
      std::cout << "Bundle number of packets: " << (latest_bundle_length/1206) << std::endl;
    }
  }

  return 0;
}
