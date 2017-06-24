// Velodyne HDL Packet Bundler
// Nick Rypkema (rypkema@mit.edu), MIT 2017
// shared library to bundle velodyne packets into enough for a single frame

#ifndef PACKET_BUNDLER_H_INCLUDED
#define PACKET_BUNDLER_H_INCLUDED

#include <string>
#include <deque>
#include "PacketDecoder.h"

class PacketBundler
{
public:
  PacketBundler();
  virtual ~PacketBundler();
  void SetMaxNumberOfBundles(unsigned int max_num_of_bundles);
  void BundlePacket(std::string* data, unsigned int* data_length);
  std::deque<std::string> GetBundles();
  void ClearBundles();
  bool GetLatestBundle(std::string* bundle, unsigned int* bundle_length);

protected:
  void UnloadData();
  void BundleHDLPacket(unsigned char *data, unsigned int data_length);
  void SplitBundle();

private:
  unsigned int _last_azimuth;
  unsigned int _max_num_of_bundles;
  std::string* _bundle;
  std::deque<std::string> _bundles;
};

#endif // PACKET_BUNDLER_H_INCLUDED
