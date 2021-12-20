#include "udp.h"

UDP::UDP(int source_port, int buf_length, ReceivedQueue* rq, const char* server_address, int server_port, LockedQueue* lq) {
	source_udp_port = source_port;
	this->buf_length = buf_length;
	receive_queue = rq;
	this->server_address = server_address;
	server_udp_port = server_port;
	send_queue = lq;
	InitUDP();
}

bool UDP::InitUDP() {
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


	client_socket = socket(AF_INET, SOCK_DGRAM, 0);
	addrClient.sin_addr.s_addr = inet_addr(server_address);//��������ͨ�ŵĵ�ַ
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(server_udp_port);
	
	return true;
}


int UDP::receive_udp(){
	int recv_len = recvfrom(server_socket, (char*)recvBuf, buf_length, 0, (SOCKADDR*)&addrClient, &len);
	ReceiveData nrd;
	memcpy(nrd.transdata, recvBuf, UDP_DATA_LENGTH);
	receive_queue->push(nrd);
	return recv_len;
}

int UDP::send_udp() {
	std::string send_data = send_queue->wait_pop().datatrans;
	const char* send_buf = send_data.c_str();
	int send_len = sendto(client_socket, send_buf, send_data.size(), 0, (SOCKADDR*)&addrClient, sizeof(addrClient));
	printf("send %d size data.\n", send_len);
	return 0;
}



bool UDP::CloseUDP(){
	closesocket(server_socket);
	WSACleanup();
	return true;
}