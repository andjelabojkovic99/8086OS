/*
 * Sistem.cpp
 *
 *  Created on: Jul 24, 2019
 *      Author: OS1
 */

#include "Sistem.h"
#include "SCHEDULE.H"
#include "thread.h"
#include "PCB.h"
#include "Threadid.h"
#include "Lista.h"
#include "semaphor.h"
#include "semlista.h"
#include "KernelEv.h"
#include <iostream.h>
#include <stdio.h>
#include <dos.h>

volatile unsigned int Sistem::lockFlag=1;
ThreadIdle* Sistem::idle_nit=NULL;
Thread* Sistem::pocetna=NULL;
volatile int Sistem::zahtevana_promena_konteksta=0;
pInterrupt Sistem::oldISR=NULL;
PCB* Sistem::running=NULL;


unsigned tsp=0;
unsigned tss=0;
unsigned tbp=0;
PCB* newPCB=NULL;

#ifndef BCC_BLOCK_IGNORE

void Sistem::inic()
{
	lock;
	oldISR=getvect(0x08);
	setvect(0x08,timer);
	setvect(0x60,oldISR);

	lockFlag=0;
	pocetna=new Thread(1024,20);
	asm cli;
	lockFlag=1;

	pocetna->myPCB->status=READY;
	running=pocetna->myPCB;

	lockFlag=0;
	idle_nit=new ThreadIdle();
	asm cli;
	lockFlag=1;

	idle_nit->start();
	unlock;

}


void Sistem::restore()
{
	lock;
	if(running->id!=((Thread*)pocetna)->myPCB->id){unlock; return;}
	//oldISR=getvect(0x60);
	setvect(0x08,oldISR);
	unlock;
	delete pocetna;
	delete idle_nit;
	for (int i=0;i<256;i++)
	{
	    if(IVTEntry::niz[i]!=NULL) 	delete IVTEntry::niz[i];
	}
	delete [] IVTEntry::niz;
}
#endif

void interrupt Sistem::timer(...)
{

	if(!zahtevana_promena_konteksta){
		tick();
		asm int 60h;

		if(((PCB*)running)->neograniceno==0)
		{
			((PCB*)running)->proteklo++;
			if( ((PCB*)running)->proteklo==((PCB*)running)->timeSlice)zahtevana_promena_konteksta=1;
		}


		if(Semaphore::lista_semafora->prazno()==0 )
		{
			Semaphore::lista_semafora->proveri_vreme();
		}


	}

	if(zahtevana_promena_konteksta==1)
	{
		if(lockFlag==1)
		{
			zahtevana_promena_konteksta=0;
			asm{
				mov tsp,sp
				mov tss,ss
				mov tbp,bp
			}

			running->sp=tsp;
			running->bp=tbp;
			running->ss=tss;

			if(running->status== READY && running->id!=idle_nit->myPCB->id && running!=pocetna->myPCB){
				Scheduler::put((PCB*)running);

			}


			while(1){

				lockFlag=0;
							//while(1) i proverava se da li je izabrani pcb READY
							newPCB=Scheduler::get();
							asm cli;
							lockFlag=1;

				if(newPCB==0)
				{
					//ovde treba dodati i start ako je prazna lista ideva

					if(Thread::lista_id->prazno()==1)
					 newPCB=pocetna->myPCB;
					else newPCB=idle_nit->myPCB;
				}


				running=newPCB;
				if(((PCB*)running)->status==READY)break;

			}

			tsp=running->sp;
			tss=running->ss;
			tbp=running->bp;


			asm{
				mov sp,tsp
				mov bp,tbp
				mov ss,tss
			}

			((PCB*)running)->proteklo=0;



		}
		else zahtevana_promena_konteksta=1;


		if(zahtevana_promena_konteksta==0) asm int 0x60;
	}



}


void Sistem::dispatch()
{
	lock;
	zahtevana_promena_konteksta=1;
	asm int 0x08;
	//timer();
	unlock;

}
