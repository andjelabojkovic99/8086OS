/*
 * event.cpp
 *
 *  Created on: Aug 5, 2019
 *      Author: OS1
 */

#include "event.h"
#include "Sistem.h"
#include "KernelEv.h"
#include <stdio.h>

Event::Event(IVTNo broj)
{
	myImpl=new KernelEv(broj);
}

Event::~Event()
{
	delete myImpl;
}

void Event::wait()
{
	lock;
	myImpl->wait();
	unlock;
}

void Event::signal()
{
	lock;
	myImpl->signal();
	unlock;
}
