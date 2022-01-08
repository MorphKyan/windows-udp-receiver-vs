//
// Created by morph on 2022/1/8.
//
#include <iostream>
#include <Winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)

int main()
{
	sockaddr_in RecvAddr;
	int RecvAddrSize = 0;
	WSADATA wsaData;
	RecvAddrSize = sizeof(RecvAddr);
	int nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (nResult != NO_ERROR) {
		std::cout << WSAGetLastError();
		return 0;
	}

	SOCKET sockClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8200);
	int ret = bind(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (ret < 0) {
		std::cout << ret;
	}

	for (int clc = 0; clc < 5; clc++)
	{
		char recvBuf[17] = { 0 };
		recv(sockClient, recvBuf, 17, 0);
		int SecurityType = static_cast<int>(recvBuf[0]);
		int TradSesMode = static_cast<int>(recvBuf[1]);

		unsigned int mask = 0x000000FF;
		unsigned int TradeDate = 0;
		for (int i = 0; i < 4; ++i) {
			TradeDate = TradeDate << 8;
			TradeDate = TradeDate | (mask & recvBuf[7 - i]);
		}

		unsigned int LastUpdateDate = 0;
		for (int i = 0; i < 4; ++i) {
			LastUpdateDate = LastUpdateDate << 8;
			LastUpdateDate = LastUpdateDate | (mask & recvBuf[11 - i]);
		}

		std::string xxType = { recvBuf[12],recvBuf[13], recvBuf[14], recvBuf[15], recvBuf[16] };

		printf("severRecv %d %d %d %d %s\n", SecurityType, TradSesMode, TradeDate, LastUpdateDate, xxType.c_str());
	}

	closesocket(sockClient);
	WSACleanup();

	getchar();
	return 0;
}