// thread.h 
//	Data structures for managing threads.  A thread represents
//	sequential execution of code within a program.
//	So the state of a thread includes the program counter,
//	the processor registers, and the execution stack.
//	
// 	Note that because we allocate a fixed size stack for each
//	thread, it is possible to overflow the stack -- for instance,
//	by recursing to too deep a level.  The most common reason
//	for this occuring is allocating large data structures
//	on the stack.  For instance, this will cause problems:
//
//		void foo() { int buf[1000]; ...}
//
//	Instead, you should allocate all data structures dynamically:
//
//		void foo() { int *buf = new int[1000]; ...}
//
//
// 	Bad things happen if you overflow the stack, and in the worst 
//	case, the problem may not be caught explicitly.  Instead,
//	the only symptom may be bizarre segmentation faults.  (Of course,
//	other problems can cause seg faults, so that isn't a sure sign
//	that your thread stacks are too small.)
//	
//	One thing to try if you find yourself with seg faults is to
//	increase the size of thread stack -- ThreadStackSize.
//
//  	In this interface, forking a thread takes two steps.
//	We must first allocate a data structure for it: "t = new Thread".
//	Only then can we do the fork: "t->fork(f, arg)".
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef THREAD_H
#define THREAD_H

#include "copyright.h"
#include "utility.h"

#ifdef USER_PROGRAM
#include "machine.h"
#include "addrspace.h"
#endif

// CPU register state to be saved on context switch.  
// The SPARC and MIPS only need 10 registers, but the Snake needs 18.
// For simplicity, this is just the max over all architectures.
#define MachineStateSize 18 


// Size of the thread's private execution stack.
// WATCH OUT IF THIS ISN'T BIG ENOUGH!!!!!
#define StackSize	(4 * 1024)	// in words


// Thread state
enum ThreadStatus { JUST_CREATED, RUNNING, READY, BLOCKED };
//char *ThreadStatusStr[] = {"JUST_CREATED","RUNNING","BLOCKED"};

// external function, dummy routine whose sole job is to call Thread::Print
extern void ThreadPrint(int arg);	 

// The following class defines a "thread control block" -- which
// represents a single thread of execution.
//
//  Every thread has:
//     an execution stack for activation records ("stackTop" and "stack")
//     space to save CPU registers while not running ("machineState")
//     a "status" (running/ready/blocked)
//    
//  Some threads also belong to a user address space; threads
//  that only run in the kernel have a NULL address space.

class Thread {
  private:
    // NOTE: DO NOT CHANGE the order of these first two members.
    // THEY MUST be in this position for SWITCH to work.
    int* stackTop;			 // the current stack pointer
    int machineState[MachineStateSize];  // all registers except for stackTop


  public:

    int new_add_uid;
    int new_add_pid;
    int used_time_slice;
    
    int fileHandler;
    int filePointer;



    Thread(char* debugName);		// initialize a Thread 
    ~Thread(); 				// deallocate a Thread
					// NOTE -- thread being deleted
					// must not be running when delete 
					// is called

    // basic thread operations

    void Fork(VoidFunctionPtr func, int arg); 	// Make thread run (*func)(arg)
    void Yield();  				// Relinquish the CPU if any 
						// other thread is runnable
    void Sleep();  				// Put the thread to sleep and 
						// relinquish the processor
    void Finish();  				// The thread is done executing
    
    void CheckOverflow();   			// Check if thread has 
						// overflowed its stack
    
    char* getName() { return (name); };

    //
    int getPid(){return (new_add_pid) ;}
    int getUid(){return (new_add_uid) ;}

    //
    void setStatus(ThreadStatus st) { status = st; };
    char* getStatus()
    {
      switch(status){
        //enum ThreadStatus { JUST_CREATED, RUNNING, READY, BLOCKED };
      case 0:return (char*)"JUST_CREATED";
      case 1:return (char*)"RUNNING";
      case 2:return (char*)"READY";
      case 3:return (char*)"BLOCKED";
      }
    } 

    //Priority operations.
    int getPriority(){return priority;}
    bool setPriority(int p){ if(p>=1&&p<=10) {priority = p ;return true;}else{printf("SET WRONG PRIORITY"); return false;}}


    void Print() { printf("%s\t ", name); };
    //void PrintNewid(){printf("Thread NewUid : %d\t  Thread newPid :%d\t",new_add_pid,new_add_uid);};
    /*void PintStatus(){
      switch(status){
        //enum ThreadStatus { JUST_CREATED, RUNNING, READY, BLOCKED };
      case 0:printf("Thread Status :JUST_CREATED\t");break;
      case 1:printf("Thread Status :RUNNING\t");break;
      case 2:printf("Thread Status :READY\t");break;
      case 3:printf("Thread Status :BLOCKED\t");break;
      }
      };
*/
  private:
    // some of the private data for this class is listed above
    
    int* stack; 	 		
        // Bottom of the stack 
				// NULL if this is the main thread
				// (If NULL, don't deallocate stack)
    ThreadStatus status; // ready, running or blocked
    char* name;
    int priority;//1~10. For 1 is the most priorir level.For 10 is least priorir level.
    
    void StackAllocate(VoidFunctionPtr func, int arg);
    			// Allocate a stack for thread.
					// Used internally by Fork()

#ifdef USER_PROGRAM
// A thread running a user program actually has *two* sets of CPU registers -- 
// one for its state while executing user code, one for its state 
// while executing kernel code.

  int userRegisters[NumTotalRegs];	// user-level CPU register state

  public:
    void SaveUserState();		// save user-level register state
    void RestoreUserState();		// restore user-level register state
    char* filename;
    AddrSpace *space;			// User code this thread is running.
#endif
};

// Magical machine-dependent routines, defined in switch.s

extern "C" {
// First frame on thread execution stack; 
//   	enable interrupts
//	call "func"
//	(when func returns, if ever) call ThreadFinish()
void ThreadRoot();

// Stop running oldThread and start running newThread
void SWITCH(Thread *oldThread, Thread *newThread);
}

#endif // THREAD_H
