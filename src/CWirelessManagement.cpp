#include "CWirelessManagement.h"

// Flooding을 위한 생성자
CWirelessManagement::CWirelessManagement(int strSSIDLen)
{
    this->strSSIDLen = strSSIDLen;
    // 보낼 채널정보만 설정
    this->channel = 8;

    u_char byteFixedData[12] = { 0x26, 0xc0, 0x51, 0x83, 0xa9, 0x0c, 0x00, 0x00, 0x64, 
    0x00, 0x11, 0x0c };
    memcpy(&this->byteFixedData, byteFixedData, sizeof(byteFixedData));

    u_char byteSupportedRates[8] = { 0x82, 0x84,  0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c};
    memcpy(&this->byteSupportedRates, byteSupportedRates, sizeof(byteSupportedRates));

    u_char byteTrafficIndicationMap[4] = { 0x00, 0x03, 0x00, 0x00 };
    memcpy(&this->byteTrafficIndicationMap, byteTrafficIndicationMap, sizeof(byteTrafficIndicationMap));

    u_char byteVenderSpecific[24] = { 0x00, 0x50, 0xf2, 0x01, 0x01, 0x00, 0x00, 0x50, 0xf2,
    0x04, 0x01, 0x00, 0x00, 0x50, 0xf2, 0x04, 0x01, 0x00, 0x00, 0x50, 0xf2, 0x02, 0x00, 0x00 }; 
    memcpy(&this->byteVenderSpecific, byteVenderSpecific, sizeof(byteVenderSpecific));
}


CWirelessManagement::~CWirelessManagement()
{

}

int CWirelessManagement::getFloodPacketSize()
{
    return 12 + this->strSSIDLen + 2 + 10 + 3 + 6 + 26;
}

bool CWirelessManagement::getFloodPacket(const char* strSSID, char* packet)
{
    // Fixed Data Copy
    int fixedDataLen = 12;
    memcpy(&packet[0], &this->byteFixedData, fixedDataLen);

    // TagID = 0(SSID)
    int packetSSIDLen = this->strSSIDLen + 2;

    packet[fixedDataLen] = 0;                       // tag id
    packet[fixedDataLen+1] = this->strSSIDLen;      // tag len
    memcpy(&packet[fixedDataLen + 2], strSSID, this->strSSIDLen);
    int tagSSIDBack = fixedDataLen + 2 + this->strSSIDLen;

    // TagID = 1(Supported Rates)
    // set Supported Rates to fixed data
    int packetSupportedRatesLen = 10;
    packet[tagSSIDBack] = 1;
    packet[tagSSIDBack+1] = 8;
    memcpy(&packet[tagSSIDBack+2], this->byteSupportedRates, sizeof(this->byteSupportedRates));
    int tagSupportedRateBack = tagSSIDBack + 2 + 8;

    // TagId = 3(DS parameter)
    int packetChannelLen = 3;
    packet[tagSupportedRateBack] = 3;
    packet[tagSupportedRateBack + 1] = 1;
    packet[tagSupportedRateBack + 2] = this->channel;
    int tagDSparameterBack = tagSupportedRateBack + 3;

    // TagId = 5(TrafficIndication)
    int packetTrafficIndicationMapLen = 6;
    packet[tagDSparameterBack] = 5;
    packet[tagDSparameterBack+1] = 4;
    memcpy(&packet[tagDSparameterBack+2], this->byteTrafficIndicationMap, sizeof(this->byteTrafficIndicationMap));
    int tagTafficIndicationMapBack = tagDSparameterBack + packetTrafficIndicationMapLen;

    // TagId - 221(VenderSpecific)
    int packetVenderSpecificlen = 26;
    packet[tagTafficIndicationMapBack] = 0xdd;
    packet[tagTafficIndicationMapBack+1] = 0x18;
    memcpy(&packet[tagTafficIndicationMapBack+2], this->byteVenderSpecific, sizeof(this->byteVenderSpecific));
    return true;
}