#include "CLpfServer.h"
#include <iostream>
#include <ws2tcpip.h> // for struct sockaddr_in
using namespace std;

CLpfServer::CLpfServer()
{
	m_pSockaddrIn = new struct sockaddr_in;

}

CLpfServer::~CLpfServer()
{
	delete m_pSockaddrIn;
	m_pSockaddrIn = NULL;
}

int32_t CLpfServer::initTcp(const char * pstrCIP)
{
	int32_t i32Rtn = -1;
	m_i32ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (0 <= m_i32ServerSocket)
	{
		if ((NULL != m_pSockaddrIn) && (NULL != pstrCIP))
		{
			memset(m_pSockaddrIn, 0, sizeof(*m_pSockaddrIn));

			sockaddr_in localAddr{};
			localAddr.sin_family = AF_INET;
			localAddr.sin_addr.s_addr = htonl(INADDR_ANY);  // 监听任意接口
			localAddr.sin_port = htons(8888);  // 端口号为 8888
			i32Rtn = bind(m_i32ServerSocket, reinterpret_cast<const sockaddr*>(&localAddr), sizeof(localAddr));
		}
	}
	return i32Rtn;
}


int32_t CLpfServer::listenTcp(const int32_t i32CCount)
{
	if (listen(m_i32ServerSocket, i32CCount) != 0)
	{
		printf("Failed to listen\n");
		closesocket(m_i32ServerSocket);
		WSACleanup();
		return -1;
	}

	printf("Waiting for client connections...\n");


	while (1)
	{
		// 接受客户端的连接，并创建与该客户端通信的 socket
		sockaddr_in clientAddr{};
		socklen_t addrlen = sizeof(clientAddr);
		SOCKET clientSocket = accept(m_i32ServerSocket, reinterpret_cast<sockaddr *>(&clientAddr), &addrlen);
		if (clientSocket == INVALID_SOCKET)
		{
			printf("Failed to accept client connection request.\n");
			continue;
		}

		// 打印客户端信息
		char addrBuffer[INET_ADDRSTRLEN]{};
		inet_ntop(AF_INET, &(clientAddr.sin_addr), addrBuffer, INET_ADDRSTRLEN);
		cout << "New client connected: " << addrBuffer << ":" << ntohs(clientAddr.sin_port) << endl;

		// 创建一个新线程处理该客户端的数据交互
		HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadProc, (LPVOID)(&clientSocket), 0, NULL);

	}

	// 关闭监听 socket
	closesocket(m_i32ServerSocket);
	WSACleanup();

	return 0;
}

int32_t CLpfServer::sendData(const int32_t i32CSocket, const char * pstrCSrcData, const int32_t i32CLen)
{
	int32_t i32RtnSendDataLen = -1;
	if (0 <= i32CSocket)
	{
		i32RtnSendDataLen = send(i32CSocket, pstrCSrcData, i32CLen, 0);
		printf("i32RtnSendDataLen=%d\n", i32RtnSendDataLen);
	}
	return i32RtnSendDataLen;
}

int32_t CLpfServer::recvData(const int32_t i32CSocket, char * pstrCBuff, const int32_t i32BuffSize)
{
	return int32_t();
}

void CLpfServer::closeSocket()	
{
#ifdef _WIN32
	closesocket(m_i32ServerSocket);
	m_i32ServerSocket = -1;
	WSACleanup();
#else
#endif
}

void *CLpfServer::threadProc(void *arg)
{
	int32_t i32CSocket = *((int*)arg);

	while (0<=i32CSocket)
	{
		// 接收数据
		char recvBuffer[512]{};
		int recvResult = recv(i32CSocket, recvBuffer, sizeof(recvBuffer), 0);
		if (recvResult == 0)
		{
			printf("Failed to receive data from client.\n");
			closesocket(i32CSocket);
			return NULL;
		}
		printf("recv:%d, ", recvResult);
		for (int32_t i = 0; i < recvResult; i++)
		{
			printf("%c", recvBuffer[i]);
		}
		printf("\n");

		// 发送数据
		int sendResult = send(i32CSocket, recvBuffer, recvResult, 0);
		if (sendResult == SOCKET_ERROR)
		{

			closesocket(i32CSocket);

			return NULL;
		}
		printf("send:%d\n", sendResult);
	}
	

	return NULL;

}
