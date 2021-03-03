/*
 * semaphor.cpp
 *
 *  Created on: Aug 1, 2019
 *      Author: OS1
 */

#include "semaphor.h"
#include "Sistem.h"
#include "KerSem.h"
#include "semlista.h"
#include <stdlib.h>

SemLista* Semaphore::lista_semafora=new SemLista();

Semaphore::Semaphore(int init)
{
	lock;
	myImpl=new KernelSem(init);

	lista_semafora->dodaj(myImpl);
	unlock;
}

int Semaphore::signal(int n)
{
	return myImpl->signal(n);
}


int Semaphore::wait(Time maxTimeToWait)
{

	return myImpl->wait(maxTimeToWait);

}


int Semaphore::val() const
{
	return myImpl->value;
}



Semaphore::~Semaphore()
{

	lista_semafora->izbaci(myImpl);
	delete myImpl;
}
