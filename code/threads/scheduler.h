// scheduler.h 
//	Data structures for the thread dispatcher and scheduler.
//	Primarily, the list of threads that are ready to run.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef SCHEDULER_H
#define SCHEDULER_H

#define MAXTHREADNUM 128

#include "copyright.h"
#include "list.h"
#include "thread.h"
// The following class defines the scheduler/dispatcher abstraction -- 
// the data structures and operations needed to keep track of which 
// thread is running, and which threads are ready but not running.

class Scheduler {
  public:
    Scheduler();			// Initialize list of ready threads 
    ~Scheduler();			// De-allocate ready list

    void ReadyToRun(Thread* thread);	// Thread can be dispatched.
    Thread* FindNextToRun();		// Dequeue first thread on the ready 
					// list, if any, and return thread.
    void Run(Thread* nextThread);	// Cause nextThread to start running
    void Print();			// Print contents of ready list
    
    //void setLastSwitchTick(int tick){lastSwitchTick = tick;}
    //int getLastSwitchTick(){return lastSwitchTick;}
  public:
    List *readyList;  		// queue of threads that are ready to run,
		//int lastSwitchTick;
    		// but not running

};






//------------------GlobalThreadManager Declaration---------------------
//
//
class GlobalThreadManager{
public :
GlobalThreadManager();
~GlobalThreadManager();

bool AddNewThreadtoList(Thread *thread);
bool RemoveThreadFromList(Thread *thread);
void ShowListInfo();

private:
  Thread * GlobalThreadList [MAXTHREADNUM];
};


#endif // SCHEDULER_H


