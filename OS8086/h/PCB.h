/*
 * PCB.h
 *
 *  Created on: Jul 24, 2019
 *      Author: OS1
 */


#ifndef PCB_H_
#define PCB_H_



typedef enum {READY,RUNNING,FINISH,NEW,BLOCKED } Status;

class Lista;
class Thread;


class PCB {
public:
	PCB(Thread* mt, unsigned long ss, unsigned int st);

	virtual ~PCB();
	static void wrapper();
	unsigned* stack;
	unsigned ss;
	unsigned sp;
	unsigned bp;
	unsigned timeSlice;

	int proteklo;
	unsigned neograniceno;


	unsigned int trenutno_cekanje;
	unsigned int max_cekanje;
	volatile int  isteklo_vreme;

	static unsigned posid;
    Status status;
	Thread * myThread;
	Lista * blokirane_niti;
	int id;
	void napravi_stek();
	int blokirana_na_semaforu;

protected:
	friend class Thread;
	friend class ThreadIdle;

private:
	unsigned long stackSize;
	unsigned int timeS;



};

#endif /* PCB_H_ */

