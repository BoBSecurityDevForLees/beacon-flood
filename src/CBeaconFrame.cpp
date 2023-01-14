#include "CBeaconFrame.h"

// flooding Initializer
C80211BeaconFrame::C80211BeaconFrame()
{
    // Random Library 초기화
    srand(time(NULL));

    // DestinationAddress Setting 
    sprintf(this->strDestinationAddress,"%02x%02x%02x%02x%02x%02x", 255, 255, 255, 255, 255, 255);

    // Source Address Setting
    sprintf(this->strSourceAddress, "%02x%02x%02x%02x%02x%02x", (rand()%255), (rand()%255), (rand()%255), (rand()%255),
    (rand()%255), (rand()%255));

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
    
    // Destination, SourceAddress, BSSID
    memcpy(&packetBeacon[2], this->strDestinationAddress, 6);
    memcpy(&packetBeacon[8], this->strSourceAddress, 6);
    memcpy(&packetBeacon[14], this->strBSSID, 6);
    
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