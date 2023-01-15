#include "CRadioHeader.h"

// flooding Initializer
C80211RadioHeader::C80211RadioHeader()
{
    // All member Zero
    this->it_version = 0x00;
    this->it_pad = 0x00;
    this->it_len = 24;

    u_char present[8] = { 0x2e, 0x40, 0x00, 0xa0, 0x20, 0x08, 0x00, 0x00};
    memcpy(&this->it_present, present, 8);

    this->it_flag = 0x00;
    this->it_data_rate = 0x02;
    this->it_channelFrequency = 0x098f;
    this->it_channelflag = 0x00a0;
    this->it_signalPW = 0xd1;
    this->it_rx_flag = 0x0000;
    this->it_antenna = 0;
}

C80211RadioHeader::~C80211RadioHeader()
{

}

bool C80211RadioHeader::getFloodPacket(char* packet)
{
    packet[0] = this->it_version;
    packet[1] = this->it_pad;
    memcpy(&packet[2], &this->it_len, 2);
    memcpy(&packet[4], &this->it_present, 8);
    
    packet[12] = this->it_flag;
    packet[13] = this->it_data_rate;
    
    memcpy(&packet[14], &this->it_channelFrequency, 2);

    memcpy(&packet[16], &this->it_channelflag, 2);

    packet[18] = this->it_signalPW;
    packet[19] = this->it_antenna;              
    memcpy(&packet[20], &this->it_rx_flag, 2);

    packet[22] = this->it_signalPW;             // Antenna Signale
    packet[23] = this->it_antenna;

    for(int i =0; i < 24; i++)
        printf("%02x ", (u_char)packet[i]);
    std::cout << std::endl;
    return true;
}

int C80211RadioHeader::getFloodPacketSize()
{
    return this->it_len;
}