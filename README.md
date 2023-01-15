# beacon-flood
이경문 멘토님의 심화교육 과정 2번째 과제인 IEEE 802.11 beacon packet을 발송하는 beacon flooding과제 입니다.

## 필요 라이브러리
1. Pcap: 네트워크 패킷을 잡기 위한 Lib
각종 네트워크 패킷을 잡을 수 있다.

## 프로젝트 빌드 방법
1. 필요 라이브러리를 설치한다.
    - libpcap
2. git clone https://github.com/arad4228/beacon-flood.git
3. cd beacon-flood
4. mkdir cmake-temp && mkdir build
5. cd cmake-temp
6. cmake ../src/ && cmake --build .
7. sudo ../build/beacon-flood < Network InterFaceName > < SSID File Location >
