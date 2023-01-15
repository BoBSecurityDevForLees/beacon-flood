#include "CBeaconFrame.h"

// flooding Initializer
C80211BeaconFrame::C80211BeaconFrame()
{
    this->framecontrol = 0x0080;
    this->strDuration = 0x0000;
    u_char byteBroadCast[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
    memcpy(&this->strDestinationAddress, byteBroadCast, 6);

    // Source Address Setting
    generateRandomByte(&this->strSourceAddress[0]);

    // BSSID Setting
    // Source Address to BSSID Copy 6Byte
    memcpy(&this->strBSSID, &this->strSourceAddress, 6);
}

C80211BeaconFrame::~C80211BeaconFrame()
{

}

bool C80211BeaconFrame::generateRandomByte(u_char* strRandomByte)
{
    // Random Library 초기화
    srand(time(NULL));

    sprintf((char*)strRandomByte, "%02x%02x%02x%02x%02x%02x", rand()%255, rand()%255,
    rand()%255, rand()%255, rand()%255, rand()%255);
    return true;
}

bool C80211BeaconFrame::getFloodBeaconFrame(char* packet)
{
    // Beacon framecontrol
    memcpy(&packet[0], &this->framecontrol, 2);
    // Duration Time
    memcpy(&packet[2], &this->strDuration, 2);
    
    // Destination, SourceAddress, BSSID
    memcpy(&packet[4], this->strDestinationAddress, 6);
    memcpy(&packet[10], this->strSourceAddress, 6);
    memcpy(&packet[16], this->strBSSID, 6);
    
    // Fragment, sequence number
    packet[22] = 0xC0;
    packet[23] = 0x38;
    return true;
}

int C80211BeaconFrame::getFloodBeaconFrameSize()
{
    return 24;
}