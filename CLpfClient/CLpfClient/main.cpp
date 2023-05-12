#include <iostream>
#include <WinSock2.h>
#include "CLpfClient.h"
using namespace std;

#pragma comment(lib,"ws2_32.lib")

int main() 
{
#ifdef _WIN32
	{
		// ≥ı ºªØWinsock
		WSADATA wsaData;
		int32_t ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (ret != 0)
		{
			printf("WSAStartup Failed: %d\n", ret);
			return -1;
		}
	}
#endif

	//const char* pstrIP = "192.168.1.101";
	const char* pstrIP = "127.0.0.1";
	const uint16_t u16Port = 12346;
	cout << "start connect " << pstrIP << "port " << u16Port << ":" << endl;
	CLpfClient objCLpfClient;
	if (0 != objCLpfClient.initTcp(pstrIP, u16Port))
	{
		cout << "initTcp failed\n";
	}
	objCLpfClient.sendData("aaa", 3);

	char strBuff[1024];
	int32_t i32RecvLen = objCLpfClient.recvData(strBuff, sizeof(strBuff));
	for (int i = 0; i < i32RecvLen; i++)
	{
		printf("%c", strBuff[i]);
	}
	printf("\n");

	system("pause");
}