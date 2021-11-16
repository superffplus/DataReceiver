// DataReceiver.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "udp.h"
#include "cytDataParser.h"
#include "SimpleIni.h"
#include "Parameters.h"
#include "LockQueue.h"
#include <stdlib.h>
#include <Windows.h>
#include <iostream>
#include <string.h>
#include <array>
#include <sstream>
#include <thread>

#pragma comment(lib,"CYTDATAPARSER.lib")

struct InitParameter{
	int source_port;
	std::string server_address;
	int server_port;
};

struct parser_data_struct {
	ReceivedQueue* rq;
	LockedQueue* lq;
	CParserResult cr;
};

void readini(const char* path, InitParameter* initParam) {
	//加载配置文件
	CSimpleIniA ini;
	ini.SetUnicode();
	SI_Error rc = ini.LoadFile(path);

	std::string source_address = "127.0.0.1";
	int source_port = SOURCE_PORT;
	std::string server_address = "219.242.247.1";
	int server_port = SERVER_PORT;

	//ip of data source
	source_address = ini.GetValue("datasource", "address", source_address.c_str());
	source_port = ini.GetLongValue("datasource", "port", source_port);

	//ip of server
	server_address = ini.GetValue("server", "address", server_address.c_str());
	server_port = ini.GetLongValue("server", "port", server_port);

	initParam->source_port = source_port;
	initParam->server_address = server_address;
	initParam->server_port = server_port;
}

DataStructure RetriveData(CParserResult cr) {
	DataStructure ds;
	std::stringstream ss;
	ss << "{count:\"" << cr.nInfoCount << "\"";
	for (int i = 0; i < cr.nInfoCount; i++) {
		ss << ", " << cr.nInfoEncoded[i] << ": \"" << cr.nInfoContent[i] << "\"";
	}
	ss << "}";
	ds.datatrans = ss.str();
	// std::cout << ds.datatrans;
	return ds;
}

void receive_data_task(UDPserver userver){
	while (true) {
		userver.receive_udp();
		Sleep(10);
	}
	userver.CloseUDP();
}

void parser_data_task(parser_data_struct pds){
	while (true) {
		ReceiveData rd = pds.rq->wait_pop();
		int lib_init_state = cyt_OnInit(NULL);
		int parser_state = cyt_OnParser(rd.transdata, UDP_DATA_LENGTH, &pds.cr);
		DataStructure ds = RetriveData(pds.cr);
		pds.lq->push(ds);
	}
}

void send_data_task(UDPclient* uclient){
	while (true)
	{
		uclient->send_udp();
	}
}


int main()
{
	//read init config
	const char* path = DEFAULT_PATH;
	InitParameter initparam;
	initparam.source_port = SOURCE_PORT;
	initparam.server_address = "219.242.247.112";
	initparam.server_port = SERVER_PORT;
	readini(path, &initparam);
	
	// UDP server初始化相关参数，用于接收simulation的数据
	ReceivedQueue rq;   //接收数据源数据的队列
	LockedQueue lq;   //存放准备发送的数据
	UDPserver userver = UDPserver(initparam.source_port, UDP_DATA_LENGTH, &rq);
	UDPclient uclient = UDPclient(initparam.server_address.c_str(), initparam.server_port, &lq);

	// 解析数据用
	CParserResult myParserResultForBig;
	parser_data_struct pds;
	pds.rq = &rq;
	pds.lq = &lq;
	pds.cr = myParserResultForBig;

	std::thread t_receive(receive_data_task, userver);
	std::thread t_retrive(parser_data_task, pds);
	std::thread t_send(send_data_task, &uclient);

	t_receive.join();
	t_retrive.join();
	t_send.join();
	
	return 0;
}
