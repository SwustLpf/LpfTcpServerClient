#include "CLpfClient.h"
#include <ws2tcpip.h> // for struct sockaddr_in
#include <iostream>
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
			serverAddr.sin_addr.s_addr = inet_addr(pstrCIP);  // 修改为服务器IP地址
			serverAddr.sin_port = htons(u16CPort);  // 修改为服务器端口号
			i32Rtn = connect(m_i32Socket, reinterpret_cast<const sockaddr*>(&serverAddr), sizeof(serverAddr));
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
