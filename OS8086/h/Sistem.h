/*
 * Sistem.h
 *
 *  Created on: Jul 24, 2019
 *      Author: OS1
 */

#ifndef SISTEM_H_
#define SISTEM_H_

#include "PCB.h"
#include "thread.h"
#include "semlista.h"
#include "IVTEntry.h"
#include <stdlib.h>
#include <stddef.h>


typedef void interrupt (*pInterrupt)(...);

#define lock asm cli
#define unlock asm sti





class ThreadIdle;

class Sistem {
public:

	Sistem();

	static void inic();
	static void restore();
	static void dispatch();
	static volatile unsigned int lockFlag;
protected:
	static volatile int zahtevana_promena_konteksta;

	static PCB* running;
	static pInterrupt oldISR;

private:
	friend class Thread;
	friend class PCB;
	friend class Idle;
	friend class KernelSem;

	friend class KernelEv;

	static Thread* pocetna;
	static ThreadIdle* idle_nit;

	static void interrupt timer(...);



};

#endif /* SISTEM_H_ */
