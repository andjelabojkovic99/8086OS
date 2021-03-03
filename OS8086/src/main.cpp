/*
 * Main.cpp
 *
 *  Created on: Jul 27, 2019
 *      Author: OS1
 */

#include "Sistem.h"
#include <iostream.h>



extern int userMain(int argc, char* argv[]);



int main (int argc,char* argv[])
{
	Sistem::inic();

	int value=userMain(argc,argv);
	Sistem::restore();
	return value;

}
