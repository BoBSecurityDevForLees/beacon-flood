#include "CRadioHeader.h"

// flooding Initializer
C80211RadioHeader::C80211RadioHeader()
{
    // All member Zero
    this->it_version = 0x00;
    this->it_pad = 0x00;
    this->it_len = 0x08;
    this->it_present = 0x00;
}

// parsing Initializer
C80211RadioHeader::C80211RadioHeader(const u_char* packet)
{
    // version
    memcpy(&this->it_version, &packet[0], 1);
    // pad
    memcpy(&this->it_pad, &packet[1], 1);
    // len
    memcpy(&this->it_len, &packet[2], 2);
    // present
    memcpy(&this->it_present, &packet[4], 4);
    // signal
    memcpy(&this->it_signalPW, &packet[18], 1);
}

C80211RadioHeader::~C80211RadioHeader()
{

}

uint C80211RadioHeader::get80211Length()
{
    return this->it_len;
}

int C80211RadioHeader::getsignalPower()
{
    return this->it_signalPW;
}

bool C80211RadioHeader::getFloodPacket(char* packet)
{
    char innerPacket[this->it_len];
    innerPacket[0] = this->it_version;
    innerPacket[1] = this->it_pad;
    memcpy(&innerPacket[2], &this->it_len, 2);
    memcpy(&innerPacket[4], &this->it_present, 4);

    memcpy(&packet[0], innerPacket, this->it_len);
    return true;
}

int C80211RadioHeader::getFloodPacketSize()
{
    return this->it_len;
}