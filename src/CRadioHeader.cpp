#include "CRadioHeader.h"

// flooding Initializer
C80211RadioHeader::C80211RadioHeader()
{
    // All member Zero
    this->it_version = 0;
    this->it_pad = 0;
    this->it_len = 8;
    this->it_present = 0;
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

char* C80211RadioHeader::getFloodPacket()
{
    char* packet = (char*)malloc(this->it_len);
    sprintf(packet,"%02x%02x%04x%08x", this->it_version, this->it_pad, this->it_len, this->it_present);
    return packet;
}