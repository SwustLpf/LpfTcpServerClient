#include "CLpfClient.h"
#include <ws2tcpip.h> // for struct sockaddr_in
#include <iostream>
using namespace std;

#define TIME_TEST
#ifdef TIME_TEST
#include <ctime>
#endif
CLpfClient::CLpfClient()
{
}

CLpfClient::~CLpfClient()
{

	closeSocket();
}

int32_t CLpfClient::initTcp(const char * pstrCIP, const uint16_t u16CPort)
{
	int32_t i32Rtn = -1;
	m_i32Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (0 <= m_i32Socket)
	{
		if (NULL != pstrCIP)
		{
			// 连接服务器
			sockaddr_in serverAddr{};
			serverAddr.sin_family = AF_INET;
#if 0
			serverAddr.sin_addr.s_addr = inet_addr(pstrCIP);  // 修改为服务器IP地址
#else
			inet_pton(AF_INET, pstrCIP, &serverAddr.sin_addr.S_un.S_addr); // 因为 inet_addr 函数已经被丢弃了
#endif
			serverAddr.sin_port = htons(u16CPort);  // 修改为服务器端口号
#ifdef TIME_TEST
			std::clock_t start = std::clock(); // 记录开始时的时钟滴答数
#endif
			i32Rtn = connect(m_i32Socket, reinterpret_cast<const sockaddr*>(&serverAddr), sizeof(serverAddr));
#ifdef TIME_TEST
			std::clock_t end = std::clock(); // 记录结束时的时钟滴答数
			double duration = static_cast<double>(end - start); // 计算时间差
			std::cout << "duration: " << duration << " ms" << std::endl;
#endif
		}
	}
	else
	{
#ifdef _WIN32
		WSACleanup();
#endif
		printf("m_i32Socket = %d\n", m_i32Socket);
		i32Rtn = -10;
	}
	

	return i32Rtn;
}

int32_t CLpfClient::sendData(const char * pstrCSrcData, const int32_t i32CLen)
{
	int32_t i32RtnSendDataLen = -1;
	if (0 <= m_i32Socket)
	{
		i32RtnSendDataLen = send(m_i32Socket, pstrCSrcData, i32CLen, 0);
		printf("i32RtnSendDataLen=%d\n", i32RtnSendDataLen);
	}
	return i32RtnSendDataLen;
}

int32_t CLpfClient::recvData(char * pstrCBuff, const int32_t i32BuffSize)
{
	int32_t i32RecvLen= recv(m_i32Socket, pstrCBuff, i32BuffSize, 0);
	return i32RecvLen;
}

void CLpfClient::closeSocket()
{
#ifdef _WIN32
	closesocket(m_i32Socket);
	m_i32Socket = -1;
	WSACleanup();
#else
#endif
}
