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

	err = WSAStartup(wVersionRequested, &wsaData);//����᷵��WSASYSNOTREADY
	if (err != 0)
	{
		printf("error 1\n");
		return false;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||     //���ֽ�Ϊ���汾
		HIBYTE(wsaData.wVersion) != 1)      //���ֽ�Ϊ���汾
	{
		WSACleanup();
		printf("error 2\n");
		return false;
	}
	//�������ڼ������׽���
	server_socket = socket(AF_INET, SOCK_DGRAM, 0);//ʧ�ܻ᷵�� INVALID_SOCKET
	//printf("Failed. Error Code : %d",WSAGetLastError())//��ʾ������Ϣ

	SOCKADDR_IN addrSrv1;     //����sockSrv���ͺͽ������ݰ��ĵ�ַ
	addrSrv1.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv1.sin_family = AF_INET;
	addrSrv1.sin_port = htons(source_udp_port);

	//���׽���, �󶨵��˿�
	bind(server_socket, (SOCKADDR*)&addrSrv1, sizeof(SOCKADDR));//�᷵��һ��SOCKET_ERROR
	//���׽�����Ϊ����ģʽ�� ׼�����տͻ�����

	
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

	err = WSAStartup(wVersionRequested, &wsaData);//����᷵��WSASYSNOTREADY
	if (err != 0)
	{
		printf("error 1\n");
		return false;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||     //���ֽ�Ϊ���汾
		HIBYTE(wsaData.wVersion) != 1)      //���ֽ�Ϊ���汾
	{
		WSACleanup();
		printf("error 2\n");
		return false;
	}
	
	client_socket = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKADDR_IN  addrSrv2;
	addrSrv2.sin_addr.s_addr = inet_addr(server_address);//��������ͨ�ŵĵ�ַ
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