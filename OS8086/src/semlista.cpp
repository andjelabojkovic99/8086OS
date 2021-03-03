/*
 * semlista.cpp
 *
 *  Created on: Aug 1, 2019
 *      Author: OS1
 */

#include "semlista.h"
#include "KerSem.h"
#include "Sistem.h"
#include "SCHEDULE.H"
#include "Lista.h"

#include <dos.h>
#include <stdlib.h>

SemLista::SemLista(){prvi=NULL; posl=NULL;}

void SemLista::dodaj(KernelSem* kersem)
{
	Sistem::lockFlag=0;
	Elem * novi=new Elem (kersem);
	asm cli;
	Sistem::lockFlag=1;

	if(prvi==NULL)prvi=novi;
	else posl->sled=novi;
	posl=novi;
	posl->sled=NULL;

}

void SemLista::izbaci(KernelSem* k)
{

		Elem * tek=prvi;
		Elem * preth=NULL;
		while(tek!=NULL && tek!= k){preth=tek;  tek=tek->sled;}

		if(tek==NULL)return;

		if(preth==NULL)
		{
			if(tek!=posl){prvi=tek->sled; tek->sled=NULL;delete tek;}
			else
				{prvi=posl=NULL;
					tek->sled=NULL;
					delete tek;
				}
		}
		else
		{
			if(tek==posl){posl=preth; preth->sled=NULL;}
			else preth->sled=tek->sled;
			tek->sled=NULL;
			delete tek;
		}

}
SemLista::~SemLista()
{
	lock;
		Elem *stari=NULL;
		while(prvi!=NULL){
			stari=prvi;
			prvi=prvi->sled;
			//stari->kersem->sem_blokirane->odblokiraj_sve_niti_iz_liste();
			delete stari;
		}

	unlock;

}

int SemLista::prazno()
{
	if(prvi==NULL) return 1;
	else return 0;
}

void SemLista::proveri_vreme()
{
	Elem* tek=prvi;
	while(tek!=NULL)
	{
		tek->kersem->sem_blokirane->azuriraj_vreme();
		tek=tek->sled;
	}
}

