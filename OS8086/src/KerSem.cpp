/*

 * KerSem.cpp
 *
 *  Created on: Aug 1, 2019
 *      Author: OS1
 */

#include "KerSem.h"
#include "Sistem.h"
#include "Lista.h"
#include "PCB.h"
#include "SCHEDULE.H"

#include <stdlib.h>


KernelSem::KernelSem(int i)
{
	this->value=i;
	pom=0;
	sem_blokirane=new Lista();
}

int KernelSem::signal(int n)
{
	lock;
	if(n<0){ unlock; return n;}

	if(n==0){

		if(this->value++<0){
			unlock;
			this->sem_blokirane->deblock();
			Sistem::dispatch();
			return 1;

		}
		else
		{
			unlock;
			return 0;
		}

	}
	//n vece od nula
	else{
		if(value>=0){value=n+value; pom=0; }

		else if((value+n) <0)
		{
				value=value+n;
				pom=n;
				for(int i=0;i<n;i++)sem_blokirane->deblock();

		}
		else{
				pom=0;
				while(value <0){value=value+1; pom++;}
				value=n-pom;
				unlock;
				sem_blokirane->odblokiraj_sve();


		}
		return this->pom;
	}
}

int KernelSem::wait(Time time)
{

	lock;

	if(--value<0)
	{

		((PCB*)Sistem::running)->status=BLOCKED;
		((PCB*)Sistem::running)->max_cekanje=time;
		((PCB*)Sistem::running)->trenutno_cekanje=0;
		((PCB*)Sistem::running)->isteklo_vreme=0;
		sem_blokirane->dodaj((PCB*)Sistem::running);

		unlock;
		Sistem::dispatch();


	   return (!(Sistem::running)->isteklo_vreme);
	}
	else
	{
		unlock;
		return 1;
	}


}






KernelSem::~KernelSem()
{
	sem_blokirane->odblokiraj_sve();
	delete sem_blokirane;
}

