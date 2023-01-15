#include "CBeaconFlood.h"

CBeaconFlood::CBeaconFlood(int packetSize, char* packet)
{
    this->packetSize = packetSize;
    this->packet = (u_char*)malloc(packetSize);
    memcpy(this->packet, packet, packetSize);

    // for(int i = 0; i < packetSize; i++)
    //     printf("%02x ", this->packet[i]);
    // std::cout << std::endl;
}

CBeaconFlood::~CBeaconFlood()
{
    // free(this->packet);
}

u_char* CBeaconFlood::getFloodPacket()
{
    return this->packet;
}

int CBeaconFlood::getFloodPacketSize()
{
    return this->packetSize;
}