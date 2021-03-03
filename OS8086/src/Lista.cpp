/*
 * Lista.cpp
 *
 *  Created on: Jul 24, 2019
 *      Author: OS1
 */

#include "Lista.h"
#include "PCB.h"
#include "thread.h"
#include "SCHEDULE.H"
#include "Sistem.h"
#include "KerSem.h"

#include <stdlib.h>
#include <dos.h>

Lista::Lista(){prvi=NULL;posl=NULL;}

void Lista::dodaj(PCB* p)
{

	if(p!=NULL){
		Elem* novi=new Elem(p);
		if(prvi==NULL)prvi=novi;
		else posl->sled=novi;
		posl=novi;
		posl->sled=NULL;
	}
}

void Lista::izbaci(PCB* p)
{
	lock;
	if(p!=NULL){
	Elem * tek=prvi;
	Elem * preth=NULL;
	while(tek!=NULL && tek!= p){preth=tek;  tek=tek->sled;}
	if(tek==NULL){
		unlock;
		return;
	}
	if(preth==NULL)
		{
				if(tek!=posl){prvi=tek->sled; tek->sled=NULL;}
				else
					{prvi=posl=NULL;
					tek->sled=NULL;}
		}
	else
		{
			if(tek==posl){posl=preth; preth->sled=NULL;}
			else preth->sled=tek->sled;
			tek->sled=NULL;
		}
	unlock;

	}
}

PCB* Lista::uzmi()
{
	if(prvi==NULL) return NULL;
	else
	{
		Elem* oldp = prvi;
		prvi = prvi->sled;
		if(prvi== NULL)
			{
			posl = NULL;
			}
		else{
			oldp->pcb->status=READY;

			Sistem::lockFlag=0;
			Scheduler::put((PCB*)oldp->pcb);
			asm cli;
			Sistem::lockFlag=1;
		}

				return oldp->pcb;
	}
}

void Lista::odblokiraj_sve()
{
	lock;
	Elem * tek=prvi;
	while(tek!=NULL){
		tek->pcb->status=READY;
		tek->pcb->proteklo=0;
		Scheduler::put((PCB*)tek->pcb);
		tek=tek->sled;
	}
	unlock;
}


Thread* Lista::vrati_nit(int id)
{
	lock;
	Elem * tek=prvi;
	while(tek!=NULL && tek!=posl )
	{
		if( tek->pcb->id==id  ) {unlock;  return (Thread*) tek->pcb->myThread;}
		tek=tek->sled;
	}

	unlock;
	return NULL;

}

Lista::~Lista()
{
	lock;
	Elem *stari=NULL;
	while(prvi!=NULL){
		stari=prvi;
		prvi=prvi->sled;
		delete stari;
	}

	unlock;
}

int Lista::prazno()
{

	if(prvi==NULL) return 1;
	else return 0;
}


void Lista::azuriraj_vreme()
{
	if(prazno()==1)return;
	else{
		Elem *tek=prvi,*pomocna=NULL;
		while(tek!=NULL)
		{
			if((tek->pcb)->max_cekanje!=0 && (tek->pcb)->status!=READY )
			{
				(tek->pcb)->trenutno_cekanje++;
				if((tek->pcb)->trenutno_cekanje==(tek->pcb)->max_cekanje)
				{
					(tek->pcb)->isteklo_vreme=1;
					(tek->pcb)->status=READY;
					pomocna=tek;
					Scheduler::put((PCB*)(tek->pcb));
					izbaci((PCB*)pomocna->pcb);
				}
			}
			tek=tek->sled;
		}
	}
}

void Lista::deblock()
{

	if(this->prazno()==0)
	{
		lock;
			Elem* pom=prvi;
			if(pom!=NULL)
			{
				prvi=prvi->sled;
				(pom->pcb)->status=READY;
				(pom->pcb)->isteklo_vreme=0;
				Scheduler::put((PCB*)pom->pcb);
			}


		unlock;
	}


}

