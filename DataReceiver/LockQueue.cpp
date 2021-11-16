#include "LockQueue.h"

void LockedQueue::push(DataStructure ds){
	mx.lock();
	queue.push(ds);
	mx.unlock();
}

DataStructure LockedQueue::wait_pop(){
	while (queue.empty());
	mx.lock();
	DataStructure nds = queue.front();
	queue.pop();
	mx.unlock();
	return nds;
}

void ReceivedQueue::push(ReceiveData rd) {
	mx.lock();
	queue.push(rd);
	mx.unlock();
}

ReceiveData ReceivedQueue::wait_pop() {
	while (queue.empty());
	mx.lock();
	ReceiveData nrd = queue.front();
	queue.pop();
	mx.unlock();
	return nrd;
}