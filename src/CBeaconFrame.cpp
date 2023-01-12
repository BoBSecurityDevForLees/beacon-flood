#include "CBeaconFrame.h"

// flooding Initializer
C80211BeaconFrame::C80211BeaconFrame(int count)
{
    
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