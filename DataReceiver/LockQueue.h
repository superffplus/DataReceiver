#pragma once

#include <mutex>
#include <thread>
#include <queue>
#include <list>
#include "Parameters.h"

struct DataStructure {
	std::string datatrans;
};

struct ReceiveData {
	unsigned char transdata[UDP_DATA_LENGTH];
};

class LockedQueue {
public:
	LockedQueue() {};
	~LockedQueue() {};

	void push(DataStructure);
	DataStructure wait_pop();

private:
	std::mutex mx;
	std::queue<DataStructure> queue;
};


class ReceivedQueue {
public:
	ReceivedQueue() {};
	~ReceivedQueue() {};

	void push(ReceiveData);
	ReceiveData wait_pop();

private:
	std::mutex mx;
	std::queue<ReceiveData> queue;
};
