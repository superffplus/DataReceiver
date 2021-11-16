#include "udp.h"

UDPserver::UDPserver(int source_port, int buf_length, ReceivedQueue* rq) {
	source_udp_port = source_port;
	this->buf_length = buf_length;
	receive_queue = rq;
	InitUDP();
}

bool UDPserver::InitUDP() {
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);//错误会返回WSASYSNOTREADY
	if (err != 0)
	{
		printf("error 1\n");
		return false;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||     //低字节为主版本
		HIBYTE(wsaData.wVersion) != 1)      //高字节为副版本
	{
		WSACleanup();
		printf("error 2\n");
		return false;
	}
	//创建用于监听的套接字
	server_socket = socket(AF_INET, SOCK_DGRAM, 0);//失败会返回 INVALID_SOCKET
	//printf("Failed. Error Code : %d",WSAGetLastError())//显示错误信息

	SOCKADDR_IN addrSrv1;     //定义sockSrv发送和接收数据包的地址
	addrSrv1.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv1.sin_family = AF_INET;
	addrSrv1.sin_port = htons(source_udp_port);

	//绑定套接字, 绑定到端口
	bind(server_socket, (SOCKADDR*)&addrSrv1, sizeof(SOCKADDR));//会返回一个SOCKET_ERROR
	//将套接字设为监听模式， 准备接收客户请求

	
	return true;
}


int UDPserver::receive_udp(){
	int recv_len = recvfrom(server_socket, (char*)recvBuf, buf_length, 0, (SOCKADDR*)&addrClient, &len);
	ReceiveData nrd;
	memcpy(nrd.transdata, recvBuf, UDP_DATA_LENGTH);
	receive_queue->push(nrd);
	return recv_len;
}



bool UDPserver::CloseUDP(){
	closesocket(server_socket);
	WSACleanup();
	return true;
}


UDPclient::UDPclient(const char* server_address, int server_port, LockedQueue* lq) {
	this->server_address = server_address;
	server_udp_port = server_port;
	send_queue = lq;
	InitUDPclient();
}

bool UDPclient::InitUDPclient() {
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);//错误会返回WSASYSNOTREADY
	if (err != 0)
	{
		printf("error 1\n");
		return false;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||     //低字节为主版本
		HIBYTE(wsaData.wVersion) != 1)      //高字节为副版本
	{
		WSACleanup();
		printf("error 2\n");
		return false;
	}
	
	client_socket = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKADDR_IN  addrSrv2;
	addrSrv2.sin_addr.s_addr = inet_addr(server_address);//输入你想通信的地址
	addrSrv2.sin_family = AF_INET;
	addrSrv2.sin_port = htons(server_udp_port);
}

int UDPclient::send_udp() {
	
	std::string send_data = send_queue->wait_pop().datatrans;
	//const char* send_buf;
	//send_buf = send_data.c_str();
	char send_buf[4] = "wds";
	int send_len = sendto(client_socket, send_buf, send_data.size(), 0, (SOCKADDR*)&addrClient, sizeof(addrClient));
	//int send_len = send(client_socket, send_buf, 4, 0);
	printf("send %d size data\n", send_len);
	return 0;
}

bool UDPclient::CloseUDP() {
	closesocket(client_socket);
	WSACleanup();
	return true;
}