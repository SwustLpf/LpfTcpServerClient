#include <iostream>
#include <WinSock2.h>
#include "CLpfServer.h"
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
			system("pause");
			return -1;
		}
	}
#endif

	CLpfServer objCLpfServer;
	if (0 != objCLpfServer.initTcp("127.0.0.1"))
	{
		cout << "initTcp failed\n";
		exit(-1);
		system("pause");

	}



	objCLpfServer.listenTcp(20);

	system("pause");
}