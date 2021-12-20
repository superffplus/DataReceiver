#pragma once

#include <string>
#include <WinSock2.h>
#include <stdio.h>
#include <Windows.h>
#include "Parameters.h"
#include "LockQueue.h"


class UDP {
public:
	// 接收数据源数据的端口，数据源数据长度，接收数据源的队列，发送数据库的地址，发送数据库的端口，发送数据库的队列
	// UDPserver(int port, int buf_length, const char* address=NULL);
	UDP(int source_port, int buf_length, ReceivedQueue* rq, const char* server_address, int server_port, LockedQueue* lq);
	bool InitUDP();
	int receive_udp();
	int send_udp();
	bool CloseUDP();
private:
	int source_udp_port, buf_length, server_udp_port;
	const char* server_address;
	SOCKET server_socket, client_socket;
	SOCKADDR_IN addrServer, addrClient;   //用来接收客户端的地址信息
	ReceivedQueue* receive_queue;
	LockedQueue* send_queue;
	int len = sizeof(SOCKADDR);
	unsigned char recvBuf[1012];
};
