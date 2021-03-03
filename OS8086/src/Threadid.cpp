/*
 * Threadidle.cpp
 *
 *  Created on: Jul 26, 2019
 *      Author: OS1
 */

#include <iostream.h>
#include <stdio.h>
#include "Threadid.h"
#include "thread.h"
#include "PCB.h"

ThreadIdle::ThreadIdle():Thread(2048,2){}

void ThreadIdle::run()
{
	while(1);

}

void ThreadIdle::start()
{
	myPCB->napravi_stek();
	myPCB->status=READY;
}

ThreadIdle::~ThreadIdle()
{
	myPCB->status=FINISH;
}
