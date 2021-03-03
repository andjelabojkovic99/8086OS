/*
 * IVTEntry.cpp
 *
 *  Created on: Aug 5, 2019
 *      Author: OS1
 */

#include "IVTEntry.h"
#include "Sistem.h"
#include "KernelEv.h"
#include <dos.h>
#include <stdio.h>


IVTEntry** IVTEntry::niz=new IVTEntry* [256];


IVTEntry::IVTEntry(unsigned char broj,pInterrupt interr) {
	lock;
	staraRutina=NULL;
	staraRutina2=NULL;
	broj_ulaza=broj;
	br2=broj;
	novaRutina=interr;
	myImpl=NULL;
	int i;

	if(niz[broj_ulaza-1]->myImpl==NULL)
	{

		niz[broj_ulaza-1]=(IVTEntry*)this;
#ifndef BCC_BLOCK_IGNORE
		staraRutina=getvect(broj_ulaza);
		setvect(broj_ulaza,novaRutina);

#endif

		staraRutina2=staraRutina;
	}
	else{
		//ako je za neki dogadjaj vec pripremljen ovaj ulza, onda cemo samo da nadjemo novo mesto za ovaj dogadjaj


		staraRutina=niz[broj_ulaza-1]->staraRutina;
		for(i=0;i<256;i++)
		{
			if(niz[i]->myImpl==NULL){ i++;break;}
		}
		br2=i;
		#ifndef BCC_BLOCK_IGNORE
			staraRutina2=getvect(i);
			setvect(i,novaRutina);
		#endif
	}
	unlock;
}

IVTEntry::~IVTEntry() {
	//zakljucati?
	lock;
#ifndef BCC_BLOCK_IGNORE
	setvect(br2,staraRutina2);
#endif
	unlock;


}

void IVTEntry::callOld()
{
	(*staraRutina)();
}


void IVTEntry::signal()
{

	if(myImpl!=NULL) ((KernelEv*)myImpl)->signal();

}

