/*
 * thread.cpp
 *
 *  Created on: Jul 24, 2019
 *      Author: OS1
 */


#include "thread.h"
#include "Lista.h"
#include "SCHEDULE.H"
#include "PCB.h"
#include "Sistem.h"
#include "Threadid.h"
#include <stdlib.h>
#include <dos.h>
#include <iostream.h>
#include <stdio.h>

Lista* Thread::lista_id=new Lista();

void Thread::start()
{
	lock;
	if(myPCB->status==NEW)
	{
		myPCB->napravi_stek();
		myPCB->status=READY;
		Scheduler::put(myPCB);
	}
	unlock;


}

Thread::Thread (StackSize stackSize , Time timeSlice )
{
	lock;

	Sistem::lockFlag=0;
	this->myPCB=new PCB((Thread*)this,stackSize, timeSlice);
	asm cli;
	Sistem::lockFlag=1;

    lista_id->dodaj((PCB*)myPCB);

    unlock;
}




void Thread::waitToComplete()
{
	lock;

	if(myPCB->id==Sistem::idle_nit->getId()){
			unlock;
			return ;
		}

	if(myPCB->id==Sistem::pocetna->myPCB->id)
		{
			unlock;
			return;
		}

	if(myPCB->status==FINISH){

		unlock;
		myPCB->blokirane_niti->odblokiraj_sve();

		return;
	}
	if(myPCB->id==(Sistem::running)->id )
	{

		unlock;
		while(1)
		{

			Sistem::dispatch();
			if(Sistem::running->status==FINISH){
				myPCB->blokirane_niti->odblokiraj_sve();
				return;
			}
		}

	}

	Sistem::running->status=BLOCKED;
	myPCB->blokirane_niti->dodaj((PCB*)Sistem::running);
	unlock;
	Sistem::dispatch();
}

Thread::~Thread()
{
	waitToComplete();//mora da se zavrsi run metoda pre nego sto se ugasi neka nit
	lista_id->izbaci((PCB*)myPCB);
	delete myPCB;
}


ID Thread::getId()
{
	return myPCB->id;

}


ID Thread::getRunningId()
{
	return ((PCB*)Sistem::running)->id;
}

Thread* Thread::getThreadById(ID id)
{
	Thread * nit= lista_id->vrati_nit(id);
	return nit;
}

void dispatch()
{
	Sistem::dispatch();

}

