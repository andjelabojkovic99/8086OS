/*
 * PCB.cpp
 *
 *  Created on: Jul 24, 2019
 *      Author: OS1
 */

#include "PCB.h"
#include "thread.h"
#include "Sistem.h"
#include "SCHEDULE.h"
#include "Lista.h"
#include <stdlib.h>
#include <stdio.h>
#include<iostream.h>
#include <dos.h>

unsigned PCB::posid=0;


PCB::PCB(Thread* t,unsigned long stack, unsigned int ts)
{
	myThread =t;

	Sistem::lockFlag=0;
	blokirane_niti=new Lista();
	asm cli;
	Sistem::lockFlag=1;

	timeS=ts;
	stackSize=stack;
	status=NEW;
	timeSlice=-1;

	this->stack=NULL;
	neograniceno=0;
	trenutno_cekanje=0;
	max_cekanje=1;
	isteklo_vreme=0;

	this->proteklo=0;
	this->id=++posid;

	this->blokirana_na_semaforu=0;

}



void PCB::wrapper()
{
	Sistem::running->myThread->run();

	lock;
	Sistem::running->status=FINISH;


	while((PCB*)Sistem::running->blokirane_niti->uzmi()!=NULL);
	unlock;

	Sistem::dispatch();



}

void PCB::napravi_stek()
{


	    if(this->stackSize>65535) this->stackSize=65535;
	    stackSize=stackSize/2;

		Sistem::lockFlag=0;
		stack=new unsigned [stackSize];
		asm cli;
		Sistem::lockFlag=1;

		stack[stackSize-1]=0x200;

		#ifndef BCC_BLOCK_IGNORE
		this->stack[stackSize-2]=FP_SEG(PCB::wrapper);
		this->stack[stackSize-3]=FP_OFF(PCB::wrapper);


		this->sp=FP_OFF(stack+stackSize-12); //PROVERI JOS JEDNOM KOLIKI JE POMERAJ
		this->ss=FP_SEG(stack+stackSize-12);
		this->bp=FP_OFF(stack+stackSize-12);

	#endif

		this->timeSlice=timeS;
		if(timeSlice==0) this->neograniceno=1;
			else this->neograniceno=0;
}


PCB::~PCB()
{
	delete blokirane_niti;
	delete stack;
}


