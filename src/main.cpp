#include <iostream>
#include <pcap.h>

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

    

    return 0;
}