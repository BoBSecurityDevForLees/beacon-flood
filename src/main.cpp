#include <iostream>
#include <fstream>
#include <pcap.h>
#include "CRadioHeader.h"
#include "CBeaconFrame.h"
#include "CWirelessManagement.h"

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
    {
        std::cout << strSSID << std::endl;
        vecSSIDList.push_back(strSSID);
    }

    // beacon Flooding을 유발할 패킷 리스트
    std::vector<u_char*> vecBeaconFloodingPacketList;

    for(int i = 0; i < vecSSIDList.size(); i++)
    {
        u_char* packet;
        // 패킷을 구성하는 클래스 생성
        C80211RadioHeader radioHeader = C80211RadioHeader();
        C80211BeaconFrame beaconFrame = C80211BeaconFrame();
        CWirelessManagement wirelessManagement = CWirelessManagement();

        // 각종 정보 로딩 완료
        char* strRadioHeader = radioHeader.getFloodPacket();
        int strRadioHeaderSize = malloc_usable_size(strRadioHeader);
        char* strBeaconFrame = beaconFrame.getFloodBeaconFrame();
        int strBeaconFrameSize = malloc_usable_size(strBeaconFrame);
        char* strWirelessManagement = wirelessManagement.getFloodPacket(vecSSIDList[i].c_str());
        int strWirelessManagementSize = malloc_usable_size(strWirelessManagement);

        int totalPacketByte =  + strRadioHeaderSize + strBeaconFrameSize + strWirelessManagementSize;

        packet = (u_char*)malloc(totalPacketByte);
        memcpy(packet, strRadioHeader, strRadioHeaderSize);
        free(strRadioHeader);
        memcpy(&packet[strRadioHeaderSize], strBeaconFrame, strBeaconFrameSize);
        free(strBeaconFrame);
        memcpy(&packet[strRadioHeaderSize + strBeaconFrameSize], strWirelessManagement, strWirelessManagementSize);
        free(strWirelessManagement);
        vecBeaconFloodingPacketList.push_back(packet);
    }

    while(true)
    {

    }

    return 0;
}