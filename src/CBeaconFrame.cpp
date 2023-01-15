#include "CBeaconFrame.h"

// flooding Initializer
C80211BeaconFrame::C80211BeaconFrame()
{
    // Random Library 초기화
    srand(time(NULL));

    u_char byteffff[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
    // DestinationAddress Setting 
    memcpy(&this->strDestinationAddress, byteffff, 6);


    u_char byteRandom[6] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 };

    // Source Address Setting
    memcpy(&this->strSourceAddress, byteRandom, 6);

    // BSSID Setting
    // Source Address to BSSID Copy 6Byte
    memcpy(&this->strBSSID, &this->strSourceAddress, 6);
}

// parsing Initializer
C80211BeaconFrame::C80211BeaconFrame(const u_char* packet)
{
    u_char temp[6];
    // DestinationAddress 복사
    memcpy(&temp, &packet[4], 6);
    sprintf(this->strDestinationAddress,"%02x:%02x:%02x:%02x:%02x:%02x", temp[0], temp[1], temp[2], temp[3], temp[4], 
    temp[5]);

    // SourceAddress 복사
    memcpy(&temp, &packet[10], 6);
    sprintf(this->strSourceAddress,"%02x:%02x:%02x:%02x:%02x:%02x", temp[0], temp[1], temp[2], temp[3], temp[4], 
    temp[5]);

    // BSSID 복사
    memcpy(&temp, &packet[16], 6);
    sprintf(this->strBSSID,"%02x:%02x:%02x:%02x:%02x:%02x", temp[0], temp[1], temp[2], temp[3], temp[4], 
    temp[5]);
}

C80211BeaconFrame::~C80211BeaconFrame()
{

}

char* C80211BeaconFrame::getDestinationAddress()
{
    return this->strDestinationAddress;
}

char* C80211BeaconFrame::getSourceAddress()
{
    return this->strSourceAddress;
}

char* C80211BeaconFrame::getBSSID()
{
    return this->strBSSID;
}

bool C80211BeaconFrame::getFloodBeaconFrame(char* packet)
{
    char packetBeacon[24] = {0x00, };
    // Beacon Signal
    packetBeacon[0] = 0x80;
    packetBeacon[1] = 0x00;
    
    // Duration Time
    memcpy(&packetBeacon[2], this->strDuration, 2);
    
    // Destination, SourceAddress, BSSID
    memcpy(&packetBeacon[4], this->strDestinationAddress, 6);
    memcpy(&packetBeacon[10], this->strSourceAddress, 6);
    memcpy(&packetBeacon[16], this->strBSSID, 6);
    
    // Fragment, sequence number
    packetBeacon[22] = 0xC0;
    packetBeacon[23] = 0x38;

    // 내부 값에서 전달받은 포인터로 값 복사
    memcpy(&packet[0], packetBeacon, 24);

    return true;
}

int C80211BeaconFrame::getFloodBeaconFrameSize()
{
    return 24;
}