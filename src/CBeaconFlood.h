#pragma once

#include <iostream>
#include <cstring>

class CBeaconFlood{
private:
    int packetSize;
    u_char* packet;

public:
    CBeaconFlood(int packetSize, char* packet);
    CBeaconFlood(const CBeaconFlood& beaconFlood);
    ~CBeaconFlood();

    u_char* getFloodPacket();
    int getFloodPacketSize();
};