// Copyright 2013 Velodyne Acoustics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
/*=========================================================================

  Program:   Visualization Toolkit
  Module:    PacketFileSender.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME PacketFileSender -
// .SECTION Description
// This program reads a pcap file and sends the packets using UDP.

#include "PacketDriver.h"
#include "PacketDecoder.h"
#include "PacketFileReader.h"

#include <string>
#include <cstdlib>
#include <iostream>

#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>


int main(int argc, char* argv[])
{

  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <packet file>" << std::endl;
    return 1;
  }

  std::string filename = argv[1];

  vtkPacketFileReader packetReader;
  packetReader.Open(filename);
  if (!packetReader.IsOpen())
    {
    std::cout << "Failed to open packet file: " << filename << std::endl;
    return 1;
    }


  try
    {

    std::string destinationIp = "127.0.0.1";
    int dataPort = 2368;

    boost::asio::io_service ioService;
    boost::asio::ip::udp::endpoint destinationEndpoint(boost::asio::ip::address_v4::from_string(destinationIp), dataPort);
    boost::asio::ip::udp::socket socket(ioService);
    socket.open(destinationEndpoint.protocol());
    //socket.connect(destinationEndpoint);

    static unsigned long packetCounter = 0;
    double systemTime = 0;
    double prevSystemTime = 0;
    double packetTime = 0;
    double prevPacketTime = 0;
    int microsec_sleep = 450;

    while (packetReader.IsOpen())
      {

        const unsigned char* data = 0;
        unsigned int dataLength = 0;
        double timeSinceStart = 0;
        if (!packetReader.NextPacket(data, dataLength, timeSinceStart))
          {
          printf("end of packet file\n");
          break;
          }

        if (dataLength != 1206)
          {
          continue;
          }

        size_t bytesSent = socket.send_to(boost::asio::buffer(data, dataLength), destinationEndpoint);

        if ((++packetCounter % 500) == 0)
          {
          printf("total sent packets: %lu\n", packetCounter);
          }

        unsigned char* data2 = const_cast<unsigned char*>(data);
        HDLDataPacket* dataPacket = reinterpret_cast<HDLDataPacket *>(data2);
        packetTime = (dataPacket->gpsTimestamp*1e-6);

        timespec tp;
        clock_gettime(CLOCK_REALTIME,&tp);
        long long timestamp_s = (tp).tv_sec*1e9;
        long long timestamp = (tp).tv_nsec + timestamp_s;
        systemTime = ((double)timestamp)*1e-9;

        if (prevPacketTime != 0 && prevSystemTime != 0) {
          double packetDiff = packetTime-prevPacketTime;
          double systemDiff = systemTime-prevSystemTime;
          microsec_sleep = microsec_sleep - (systemDiff-packetDiff)*1e6;
          //std::cout << packetDiff << ", " << systemDiff << ", " << microsec_sleep << std::endl;
        }

        prevSystemTime = systemTime;
        prevPacketTime = packetTime;

        boost::this_thread::sleep(boost::posix_time::microseconds(microsec_sleep));
      }

    }
  catch( std::exception & e )
    {
    std::cout << "Caught Exception: " << e.what() << std::endl;
    return 1;
    }

  return 0;
}
