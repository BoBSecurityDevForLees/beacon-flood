#include "CWirelessManagement.h"

// Flooding을 위한 생성자
CWirelessManagement::CWirelessManagement()
{
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
    srand(time(NULL));
    // maybe check CountryCode
    this->channel = rand()%48;
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

char* CWirelessManagement::getFloodPacket(const char* strSSID)
{
    int fixedDataLen = 12;
    // set fixedData to fixed data
    u_char strFixedData[12] = { 0x26, 0xc0, 0x51, 0x83, 0xa9, 0x0c, 0x00, 0x00, 0x64, 0x00, 0x11, 0x0c };
    
    // TagID = 0(SSID)
    // SSID값 삽입.
    int strSSIDlen = strlen(strSSID);
    int packetSSIDLen = strSSIDlen + 2;

    // TagID = 1(Supported Rates)
    // set Supported Rates to fixed data
    // 1, 2, 5.5, 6, 9, 12, 18
    int packetSupportedRatesLen = 10;
    u_char strSupportedRates[8] = { 0x82, 0x84,  0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24};
    // memcpy(& , strSupportedRates, 8);

    // TagId = 3(Channel)
    int packetChannelLen = 3;
    // 생성자를 통해 미리 채널정보를 입력해둠.
    
    char* packetWirelessManagement;
    packetWirelessManagement = (char*)malloc(fixedDataLen + packetSSIDLen + packetSupportedRatesLen + packetChannelLen);
    
    // fixedData 값 복사
    memcpy(packetWirelessManagement, strFixedData, fixedDataLen);
    
    // SSID 값 복사
    packetWirelessManagement[fixedDataLen] = 0;                 // tag id
    packetWirelessManagement[fixedDataLen+1] = strSSIDlen;      // tag len
    int tagSSIDLenLoc = fixedDataLen + 1;
    memcpy(&packetWirelessManagement[tagSSIDLenLoc+1], strSSID, strSSIDlen);
    int tagSSIDBack = tagSSIDLenLoc + 1 + strSSIDlen;

    // Supported Rates 값 복사
    packetWirelessManagement[tagSSIDBack] = 1;                  // tag id
    packetWirelessManagement[tagSSIDBack+1] = 8;                // tag len
    int tagSupportedRatesLenLoc = tagSSIDBack + 1;
    memcpy(&packetWirelessManagement[tagSupportedRatesLenLoc+1], strSupportRates, 8);
    int tagSupportedRateBack = tagSupportedRatesLenLoc + 1 + 8;
    
    // DS parameter
    packetWirelessManagement[tagSupportedRateBack] = 3;
    packetWirelessManagement[tagSupportedRateBack + 1] = 1;
    packetWirelessManagement[tagSupportedRateBack + 2] = this->channel;

    return packetWirelessManagement;
}