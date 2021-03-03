/*
 * IVTEntry.h
 *
 *  Created on: Aug 5, 2019
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_
#include <stdlib.h>

#define PREPAREENTRY(numE,callO)\
	void interrupt inter##numE(...);\
	IVTEntry newE##numE(numE,inter##numE);\
	void interrupt inter##numE(...){\
		newE##numE.signal();\
		if(callO == 1)\
		newE##numE.callOld();\
	}



typedef void interrupt(*pInterrupt)(...);
class KernelEv;


class IVTEntry {
public:
	IVTEntry(unsigned char broj,pInterrupt inter);
	virtual ~IVTEntry();
	void callOld();
	void signal();
	static IVTEntry** niz;
protected:

	friend class KernelEv;
	pInterrupt staraRutina;
	pInterrupt staraRutina2;
	unsigned char broj_ulaza;
	unsigned char br2;
	pInterrupt novaRutina;
	volatile KernelEv* myImpl;
};

#endif /* IVTENTRY_H_ */
