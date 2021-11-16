#pragma once

#include <string>
#include <WinSock2.h>
#include <stdio.h>
#include <Windows.h>
#include "Parameters.h"
#include "LockQueue.h"


class UDPserver {
public:
	// ��������Դ���ݵĶ˿ڣ�����Դ���ݳ��ȣ���������Դ�Ķ��У��������ݿ�ĵ�ַ���������ݿ�Ķ˿ڣ��������ݿ�Ķ���
	// UDPserver(int port, int buf_length, const char* address=NULL);
	UDPserver(int source_port, int buf_length, ReceivedQueue* rq);
	bool InitUDP();
	int receive_udp();
	bool CloseUDP();
private:
	int source_udp_port, buf_length;
	
	SOCKET server_socket;
	SOCKADDR_IN addrClient;   //�������տͻ��˵ĵ�ַ��Ϣ
	ReceivedQueue* receive_queue;
	int len = sizeof(SOCKADDR);
	unsigned char recvBuf[1012];
};


class UDPclient {
public:
	UDPclient(const char* server_address, int server_port, LockedQueue* lq);
	bool InitUDPclient();
	int send_udp();
	bool CloseUDP();
private:
	int server_udp_port;
	const char* server_address;
	SOCKET client_socket;
	SOCKADDR_IN addrClient;
	LockedQueue* send_queue;
};

