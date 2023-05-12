#pragma once
#include <stdint.h>

class CLpfClient
{
public:
	CLpfClient();
	~CLpfClient();
public:
	int32_t initTcp(const char* pstrCIP, const uint16_t u16CPort);
	int32_t sendData(const char* pstrCSrcData, const int32_t i32CLen);
	int32_t recvData(char* pstrCBuff, const int32_t i32BuffSize);
private:
	void closeSocket();
private:
	int32_t m_i32Socket = -1;
};

