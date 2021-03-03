/*
 * semlista.h
 *
 *  Created on: Aug 1, 2019
 *      Author: OS1
 */

#ifndef SEMLISTA_H_
#define SEMLISTA_H_
#include "KerSem.h"


class SemLista {
public:
	struct Elem{
		KernelSem* kersem;
		Elem* sled;
		Elem(KernelSem* k, Elem* s=0): kersem(k){sled=s;}
	};



	SemLista();

	void dodaj(KernelSem * k);
	void izbaci(KernelSem* k);

	int prazno();
	void proveri_vreme();


	virtual ~SemLista();
protected:
	friend class KernelSem;
	Elem * prvi;
	Elem* posl;
};

#endif /* SEMLISTA_H_ */
