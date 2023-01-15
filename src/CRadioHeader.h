#pragma once

#include <iostream>
#include <cstring>

class C80211RadioHeader
{
private:
    uint8_t it_version;
    uint8_t it_pad;
    uint16_t it_len;
    uint64_t it_present;
    uint8_t it_flag;
    uint8_t it_data_rate;
    uint16_t it_channelFrequency;
    uint16_t it_channelflag;
    int8_t it_signalPW;
    uint16_t it_rx_flag;
    uint8_t it_antenna;

public:
    C80211RadioHeader();                            
    ~C80211RadioHeader();

    bool getFloodPacket(char* packet);
    int getFloodPacketSize();
};