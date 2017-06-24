// Velodyne HDL Packet Bundle Decoder
// Nick Rypkema (rypkema@mit.edu), MIT 2017
// shared library to decode a bundle of velodyne packets

#ifndef PACKET_BUNDLE_DECODER_H_INCLUDED
#define PACKET_BUNDLE_DECODER_H_INCLUDED

#include <string>
#include <vector>
#include <deque>
#include "PacketDecoder.h"

class PacketBundleDecoder
{
public:
  struct HDLFrame
  {
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;
    std::vector<unsigned char> intensity;
    std::vector<unsigned char> laser_id;
    std::vector<unsigned short> azimuth;
    std::vector<double> distance;
    std::vector<unsigned int> ms_from_top_of_hour;
  };

public:
  PacketBundleDecoder();
  virtual ~PacketBundleDecoder();
  void SetMaxNumberOfFrames(unsigned int max_num_of_frames);
  void DecodeBundle(std::string* bundle, unsigned int* bundle_length);
  void SetCorrectionsFile(const std::string& corrections_file);
  std::deque<HDLFrame> GetFrames();
  void ClearFrames();
  bool GetLatestFrame(HDLFrame* frame);

protected:
  void UnloadData();
  void InitTables();
  void LoadCorrectionsFile(const std::string& correctionsFile);
  void LoadHDL32Corrections();
  void SetCorrectionsCommon();
  void ProcessHDLPacket(unsigned char *data, unsigned int data_length);
  void PushFiringData(unsigned char laserId, unsigned short azimuth, unsigned int timestamp, HDLLaserReturn laserReturn, HDLLaserCorrection correction);

private:
  std::string _corrections_file;
  unsigned int _max_num_of_frames;
  HDLFrame* _frame;
  std::deque<HDLFrame> _frames;
};

#endif // PACKET_BUNDLE_DECODER_H_INCLUDED
