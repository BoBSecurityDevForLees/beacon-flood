#include "CWirelessManagement.h"

CWirelessManagement::CWirelessManagement(const u_char* packet)
{
    // FixedByte Data
    memcpy(this->fixedData, &packet, 12);

    int SSIDsize = (int)packet[13];
    int SSIDLoc = 14;
    if(SSIDsize == 0)
    {
        this->strSSID = (char*)malloc(7);
        memcpy(this->strSSID, "MISSING", 7);
        this->strSSID[7] = '\0';
    }
    else
    {
        this->strSSID = (char*)malloc(SSIDsize+1);
        memcpy(this->strSSID, &packet[SSIDLoc], SSIDsize);
        this->strSSID[SSIDsize] = '\0';
    }
    // std::cout <<  SSIDLoc << " " << SSIDsize << std::endl;
    // 00일경우 이름 찾기
    
    // Supported Rates
    int supportedSize = (int)packet[SSIDLoc + SSIDsize + 1];
    int supportedLoc = SSIDLoc + SSIDsize + 2;
    this->strSupportRates = (char*)malloc(supportedSize);
    memcpy(this->strSupportRates, &packet[supportedLoc], supportedSize);
    // std::cout << supportedSize << " " << supportedLoc << std::endl;

    // 채널
    int channelSize = (int)packet[supportedLoc + supportedSize + 1];
    int channelLoc = supportedLoc + supportedSize + 2;
    this->channel = (int)packet[channelLoc];
}

CWirelessManagement::~CWirelessManagement()
{
    free(this->strSSID);
    free(this->strSupportRates);
}

char* CWirelessManagement::getSSID()
{
    return this->strSSID;
}

int CWirelessManagement::getChannel()
{
    return this->channel;
}