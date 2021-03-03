/*
 * KernelEv.h
 *
 *  Created on: Aug 5, 2019
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_
#include "IVTEntry.h"
#include <stdio.h>




typedef unsigned char IVTNo;

class Event;
class PCB;
class KernelEv {
public:
	KernelEv(IVTNo br);
	virtual ~KernelEv();
	void wait();
	void signal();



protected:
	friend class Event;
	friend class IVTEntry;


	unsigned value;
	IVTNo broj_ul;

private:
	PCB* myPCB;

};

#endif /* KERNELEV_H_ */
