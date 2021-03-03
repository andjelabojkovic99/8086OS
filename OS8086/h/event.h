/*
 * event.h
 *
 *  Created on: Aug 5, 2019
 *      Author: OS1
 */

#ifndef EVENT_H_
#define EVENT_H_


#include "KernelEv.h"
typedef unsigned char IVTNo;



class Event {
public:
	Event(IVTNo ivtno);
	~Event();
	void wait();

protected:
	friend class KernelEv;
	void signal();

private:
	KernelEv* myImpl;

};

#endif /* EVENT_H_ */
