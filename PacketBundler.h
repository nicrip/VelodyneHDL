// Velodyne HDL Packet Bundler
// Nick Rypkema (rypkema@mit.edu), MIT 2017
// shared library to bundle velodyne packets into enough for a single frame

#ifndef PACKET_BUNDLER_H_INCLUDED
#define PACKET_BUNDLER_H_INCLUDED

#include <string>
#include <deque>

namespace
{
const int HDL_NUM_ROT_ANGLES = 36001;
const int HDL_LASER_PER_FIRING = 32;
const int HDL_MAX_NUM_LASERS = 64;
const int HDL_FIRING_PER_PKT = 12;

#pragma pack(push, 1)
typedef struct HDLLaserReturn
{
  unsigned short distance;
  unsigned char intensity;
} HDLLaserReturn;
#pragma pack(pop)

struct HDLFiringData
{
  unsigned short blockIdentifier;
  unsigned short rotationalPosition;
  HDLLaserReturn laserReturns[HDL_LASER_PER_FIRING];
};

struct HDLDataPacket
{
  HDLFiringData firingData[HDL_FIRING_PER_PKT];
  unsigned int gpsTimestamp;
  unsigned char blank1;
  unsigned char blank2;
};
}

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
