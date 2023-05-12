#pragma once
#include <stdint.h>
class CLpfServer
{
public:
	CLpfServer();
	~CLpfServer();
public:
	int32_t initTcp(const char* pstrCIP);
	int32_t listenTcp(const int32_t i32CCount);

	int32_t sendData(const int32_t i32CSocket, const char* pstrCSrcData, const int32_t i32CLen);
	int32_t recvData(const int32_t i32CSocket, char* pstrCBuff, const int32_t i32BuffSize);
private:
	void closeSocket();

	static void *threadProc(void *arg); // ��Ϊ��Ա�������� this ָ��, ������Ϊ�ص�����, ��˱�����static



private:
	struct sockaddr_in *m_pSockaddrIn = NULL;
	int32_t m_i32ServerSocket = -1;
	int32_t m_i32ClientSocket = -1;
};

