/*
 * Lista.h
 *
 *  Created on: Jul 24, 2019
 *      Author: OS1
 */

#ifndef LISTA_H_
#define LISTA_H_


class PCB;
class Thread;
class KerSem;

class Lista {
public:
	struct Elem{
		PCB* pcb;
		Elem * sled;
		Elem(PCB* p,Elem* s=0): pcb(p){sled=s;}
	};



	int prazno();
	void dodaj(PCB* p);
	void izbaci(PCB*);
	Thread* vrati_nit(int id);
	void odblokiraj_sve();
	void azuriraj_vreme();
	void deblock();
	PCB* uzmi();

	Lista();
	virtual ~Lista();

protected:
	friend class KernelSem;
	Elem *prvi;
	Elem *posl;



};

#endif /* LISTA_H_ */
