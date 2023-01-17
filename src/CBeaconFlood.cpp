#include "CBeaconFlood.h"

CBeaconFlood::CBeaconFlood(int packetSize, char* packet)
{
    this->packetSize = packetSize;
    this->packet = (u_char*)malloc(packetSize);
    memcpy(this->packet, packet, packetSize);
}

CBeaconFlood::CBeaconFlood(const CBeaconFlood& beaconFlood)
{
    this->packetSize = beaconFlood.packetSize;
    this->packet = (u_char*)malloc(this->packetSize);
    memcpy(this->packet, beaconFlood.packet, packetSize);
}

CBeaconFlood::~CBeaconFlood()
{
    free(this->packet);
}

u_char* CBeaconFlood::getFloodPacket()
{
    return this->packet;
}

int CBeaconFlood::getFloodPacketSize()
{
    return this->packetSize;
}