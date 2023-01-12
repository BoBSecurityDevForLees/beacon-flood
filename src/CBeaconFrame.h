#pragma once

#include <iostream>
#include <cstring>

class C80211BeaconFrame
{
private:
    u_char strDuration[2];
    char strDestinationAddress[18];
    char strSourceAddress[18];
    char strBSSID[18];

public:
    C80211BeaconFrame(int count);               // Flooding을 위한 생성자
    C80211BeaconFrame(const u_char* packet);    // Parsing을 위한 생성자
    ~C80211BeaconFrame();
    char* getDestinationAddress();
    char* getSourceAddress();
    char* getBSSID();
    char* getFloodBeaconFrame();
};