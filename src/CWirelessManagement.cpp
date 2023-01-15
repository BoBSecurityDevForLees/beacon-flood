#include "CWirelessManagement.h"

// Flooding을 위한 생성자
CWirelessManagement::CWirelessManagement(int strSSIDLen)
{
    this->strSSIDLen = strSSIDLen;
    // 보낼 채널정보만 설정
    this->setChannel();
}

// Parsing을 위한 생성자
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
    // 만약 strSSID를 위해 동적할당 했다면
    if(malloc_usable_size(this->strSSID) != 0)
        free(this->strSSID);

    // 만약 strSupportedRates를 위해 동적할당 했다면
    if(malloc_usable_size(this->strSupportRates) != 0)
        free(this->strSupportRates);
}

bool CWirelessManagement::setChannel()
{
    // srand(time(NULL));
    // // maybe check CountryCode
    // this->channel = rand()%12;
    this->channel = 1;
    return true;
}

char* CWirelessManagement::getSSID()
{
    return this->strSSID;
}

int CWirelessManagement::getChannel()
{
    return this->channel;
}

int CWirelessManagement::getFloodPacketLen()
{
    return 12 + this->strSSIDLen + 2 + 10 + 3 + 6 + 26;
}

bool CWirelessManagement::getFloodPacket(const char* strSSID, char* packet)
{
    int fixedDataLen = 12;
    // set fixedData to fixed data
    u_char byteFixedData[12] = { 0x26, 0xc0, 0x51, 0x83, 0xa9, 0x0c, 0x00, 0x00, 0x64, 0x00, 0x11, 0x0c };
    
    // TagID = 0(SSID)
    int packetSSIDLen = this->strSSIDLen + 2;

    // TagID = 1(Supported Rates)
    // set Supported Rates to fixed data
    // 1, 2, 5.5, 6, 9, 12, 18
    int packetSupportedRatesLen = 10;
    u_char byteSupportedRates[8] = { 0x82, 0x84,  0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c};

    // TagId = 3(Channel)
    int packetChannelLen = 3;
    // 생성자를 통해 미리 채널정보를 입력해둠.

    // TrafficIndication
    int packetTrafficIndicationMapLen = 6;
    u_char bytetrafficIndicationMap[packetTrafficIndicationMapLen] = { 0x05, 0x04, 0x00, 0x03, 0x00, 0x00 };

    int packetVenderSpecificlen = 26;
    u_char byteVenterSpecific[packetVenderSpecificlen] = { 0xdd, 0x18, 0x00, 0x50, 0xf2, 0x01, 0x01, 0x00,
     0x00, 0x50, 0xf2, 0x04, 0x01, 0x00, 0x00, 0x50, 0xf2, 0x04, 0x01, 0x00, 0x00, 0x50, 0xf2, 0x02, 0x00, 0x00 }; 

    int packetLen = fixedDataLen + packetSSIDLen + packetSupportedRatesLen + packetChannelLen + packetTrafficIndicationMapLen + packetVenderSpecificlen;
    char innerpacket[packetLen] = { 0x00, };

    // fixedData 값 복사
    memcpy(innerpacket, byteFixedData, fixedDataLen);
    
    // SSID 값 복사
    innerpacket[fixedDataLen] = 0;                 // tag id
    innerpacket[fixedDataLen+1] = this->strSSIDLen;      // tag len
    int tagSSIDLenLoc = fixedDataLen + 1;
    memcpy(&innerpacket[tagSSIDLenLoc+1], strSSID, this->strSSIDLen);
    int tagSSIDBack = tagSSIDLenLoc + 1 + this->strSSIDLen;

    // Supported Rates 값 복사
    innerpacket[tagSSIDBack] = 1;                  // tag id
    innerpacket[tagSSIDBack+1] = 8;                // tag len
    int tagSupportedRatesLenLoc = tagSSIDBack + 1;
    memcpy(&innerpacket[tagSupportedRatesLenLoc+1], byteSupportedRates, 8);
    int tagSupportedRateBack = tagSupportedRatesLenLoc + 1 + 8;
    
    // DS parameter
    innerpacket[tagSupportedRateBack] = 3;
    innerpacket[tagSupportedRateBack + 1] = 1;
    innerpacket[tagSupportedRateBack + 2] = this->channel;
    int tagDSparameterBack = tagSupportedRateBack+3;

    // Traffic indication Map
    memcpy(&innerpacket[tagDSparameterBack], bytetrafficIndicationMap, packetTrafficIndicationMapLen);
    int tagTafficIndicationMapBack = tagDSparameterBack + packetTrafficIndicationMapLen;

    memcpy(&innerpacket[tagTafficIndicationMapBack], byteVenterSpecific, packetVenderSpecificlen);

    memcpy(&packet[0], innerpacket, packetLen);

    return true;
}