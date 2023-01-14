#pragma once

#include <iostream>
#include <cstring>
#include <string>
#include <vector>

typedef std::basic_string<wchar_t> wstring;

enum tagType
{
    TagSSID = 0,
    TagSupportedRate = 1,
    TagFHParameterSet = 2,
    TagDSParameterSet = 3,
    TagCFParameterSet = 4,
    TagTIM = 5,
    TagIBSS = 6,
    TagCountry = 7,
    TagHoppingPatternParameters = 8,
    TagHoppingPatternTable = 9,
    TagRequest = 10,
    TagChallengetext = 16,
    TagPowerConstraint = 32,
    TagPowerCapability = 33,
    TagTPCRequest = 34,
    TagTPCReport = 35,
    TagSupportedChannels = 36,
    TagChannelSwitchAnnouncement = 37,
    TagMeasurementRequest = 38,
    TagMeasurementReport = 39,
    TagQuiet = 40,
    TagIBSSDFS = 41,
    TagERPinformation = 42,
    TagRobustSecurityNetwork = 48,
    TagExtendedSupportedRates = 50,
    TagWifiProtectedAccess = 221
};

// Tag 공통 데이터
typedef struct ST_TagCommon{
    int8_t tagType;
    int8_t tagLegth;
}ST_TagCommon;

// Tag Data Class
class CTagSuper
{
private:
    ST_TagCommon common;    // Tag 공통정보
    u_char* data;           // Tag 가변 데이터

public:
    CTagSuper();
    ~CTagSuper();
};

class CWirelessManagement
{
private:
    char fixedData[12];
    char* strSSID;
    char* strSupportRates;
    int channel;


public:
    CWirelessManagement(const u_char* packet);
    ~CWirelessManagement();

    char* getSSID();
    int getChannel();

};