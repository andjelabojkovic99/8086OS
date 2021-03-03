/*
 * KerSem.h
 *
 *  Created on: Aug 1, 2019
 *      Author: OS1
 */

#ifndef KERSEM_H_
#define KERSEM_H_
#include "semaphor.h"

#include "Lista.h"


class PCB;
class KernelSem {
public:
	KernelSem(int in);
	virtual ~KernelSem();
protected:
	friend class Lista;
	friend class Semaphore;
	friend class SemLista;
	int pom;

	int wait(unsigned int t);
	int signal(int n);
	int value;
	Lista* sem_blokirane;


};

#endif /* KERSEM_H_ */
