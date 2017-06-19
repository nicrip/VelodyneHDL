# VelodyneHDL

### A minimal driver for Velodyne HDL-16/32/64 lidars
Most Velodyne lidar drivers are needlessly complex, or specifically written for a particular middleware (e.g. ROS). This repo contains minimal, lightweight code to build shared libraries to interface to and decode packets from the Velodyne HDL family of lidars.

#### Contains
 - PacketDriver: builds to PacketDriver.so, a library to read (via boost::asio) Velodyne packets streamed to UDP port 2368.
 - PacketDecoder: builds to PacketDecoder.so, a library to decode (convert to x, y, z, intensity, etc.) Velodyne packets.
 - PacketFileSender: builds to PacketFileSender, an executable to stream packets from a pcap file to UDP port 2368 (code from VTK)
 - PacketFileReader: a header file to read packets from a pcap file (code from VTK)
 - PacketFileWriter: a header file to write packets to a pcap file (code from VTK)
 
#### Example Usage
Under the tests directory you can find example code on how to use the PacketDriver and PacketDecoder libraries, as well as the PacketFileWriter header.

#### Dependencies  

###### PCAP Library:
sudo apt-get install libpcap-dev  

###### Boost Libraries:
sudo apt-get install libboost-all-dev  

#### Build  

###### Building:
> mkdir build  
> cd build  
> cmake ..  
> make

###### Add To Path:
Add to your .bashrc:  
export PATH="PATH:/..../path/to/build  
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/..../path/to/build"  

#### Usage  

###### Streaming PCAP File:
PacketFileSender pcap_file.pcap  

###### Interfacing to Velodyne:
test_PacketDriver

###### Interfacing to Velodyne and Decoding Packets:
test_PacketDecoder

###### Interfacing to Velodyne and Writing Packets to pcap File:
test_PacketWriter
