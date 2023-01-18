#include "CBeaconFlood.h"

CBeaconFlood::CBeaconFlood(const char* strSSID)
{
    // 클래스 멤버 변수 선언
    this->radioHeader = new C80211RadioHeader();
    this->beaconFrame = new C80211BeaconFrame();
    this->wirelessmanagement = new CWirelessManagement(strlen(strSSID));

    // packet에 대한 길이 계산을 위한 변수 선언
    int radioHeaderSize = this->radioHeader->getFloodPacketSize();
    int beaconFrameSize = this->beaconFrame->getFloodBeaconFrameSize();
    int wirelessmanagementSize = this->wirelessmanagement->getFloodPacketSize();

    this->packetSize = radioHeaderSize + beaconFrameSize  + wirelessmanagementSize;
    this->packet = (char*)malloc(this->packetSize);

    // packet 데이터 복사
    this->radioHeader->getFloodPacket(this->packet);
    this->beaconFrame->getFloodBeaconFrame(&this->packet[radioHeaderSize]);
    this->wirelessmanagement->getFloodPacket(strSSID, &this->packet[radioHeaderSize+beaconFrameSize]);
}

CBeaconFlood::CBeaconFlood(const CBeaconFlood& beaconFlood)
{
    this->packetSize = beaconFlood.packetSize;
    this->packet = (char*)malloc(this->packetSize);
    memcpy(this->packet, beaconFlood.packet, packetSize);
}

CBeaconFlood::~CBeaconFlood()
{
    free(this->packet);
}

u_char* CBeaconFlood::getFloodPacket()
{
    return (u_char*)this->packet;
}

int CBeaconFlood::getFloodPacketSize()
{
    return this->packetSize;
}