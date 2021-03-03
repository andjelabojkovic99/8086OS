/*
 * KernelEv.cpp
 *
 *  Created on: Aug 5, 2019
 *      Author: OS1
 */

#include "KernelEv.h"
#include "Sistem.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>




KernelEv::KernelEv(IVTNo broj)
{
	lock;
	broj_ul=broj;
	value=0;
	myPCB=(PCB*)Sistem::running;

	if(IVTEntry::niz[broj_ul-1]!=NULL)
	{
		if(IVTEntry::niz[broj_ul-1]->myImpl==NULL)
		{

			(IVTEntry::niz[broj_ul-1])->myImpl=(KernelEv*)this;

		}
		else
			{
				for(int i=0;i<256;i++)
				{
					if((IVTEntry::niz[i]->broj_ulaza)== broj && IVTEntry::niz[i]->myImpl==NULL )
					{

						(IVTEntry::niz[broj_ul-1])->myImpl=(KernelEv*)this;
					}
				}
			}
	}
	else
	{
		for(int i=0;i<256;i++)
		{
			if((IVTEntry::niz[i]->broj_ulaza)== broj && IVTEntry::niz[i]->myImpl==NULL )
			{
				(IVTEntry::niz[broj_ul-1])->myImpl=(KernelEv*)this;
			}
		}
	}
	unlock;
}

void KernelEv::wait()
{
	if((myPCB->id)  == (Sistem::running)->id)
	{
		if(value==0)
		{
			myPCB->status=BLOCKED;
			dispatch();
		}
		else
			{
				value=0;
			}
	}

}


void KernelEv::signal()
{
	if(myPCB->status!=FINISH && myPCB->status!=BLOCKED) value=1;
	if(myPCB->status==BLOCKED) {
		myPCB->status=READY;
		value=1;
		Scheduler::put((PCB*)myPCB);

	}
	//UVEK TREBA RADITI PREUZIMANJE,
   Sistem::zahtevana_promena_konteksta=1;


}

KernelEv::~KernelEv()
{
	lock;

	if(myPCB->status==BLOCKED)
	{
		myPCB->status=READY;
		Scheduler::put((PCB*)myPCB);
				//Scheduler::put(myPCB)
	}
	unlock;

}









