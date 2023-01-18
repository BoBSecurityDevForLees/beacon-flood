#pragma once

#include <iostream>
#include <cstring>
#include "CRadioHeader.h"
#include "CBeaconFrame.h"
#include "CWirelessManagement.h"

class CBeaconFlood{
private:
    C80211RadioHeader* radioHeader;
    C80211BeaconFrame* beaconFrame;
    CWirelessManagement* wirelessmanagement;

    int packetSize;
    char* packet;

public:
    CBeaconFlood(const char* strSSID);
    CBeaconFlood(const CBeaconFlood& beaconFlood);
    ~CBeaconFlood();

    u_char* getFloodPacket();
    int getFloodPacketSize();
};