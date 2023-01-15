#pragma once

#include <iostream>
#include <cstring>
#include <random>

class C80211BeaconFrame
{
private:
    uint16_t framecontrol;
    uint16_t strDuration;
    u_char strDestinationAddress[6];
    u_char strSourceAddress[6];
    u_char strBSSID[6];

    bool generateRandomByte(u_char* strRandomByte);

public:
    C80211BeaconFrame();
    ~C80211BeaconFrame();

    bool getFloodBeaconFrame(char* packet);
    int getFloodBeaconFrameSize();
};