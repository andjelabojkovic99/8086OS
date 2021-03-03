/*
 * Threadid.h
 *
 *  Created on: Jul 26, 2019
 *      Author: OS1
 */

#ifndef THREADID_H_
#define THREADID_H_
#include "thread.h"

class Thread;


class ThreadIdle:public Thread{

public:
	ThreadIdle();
	~ThreadIdle();
	virtual void run();
	void start();
protected:
	friend class PCB;

};

#endif /* THREADIDLE_H_ */
