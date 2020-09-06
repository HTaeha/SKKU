#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"

int thread_create(void *(*function)(void *), int priority, void *arg, void *stack){
    return clone(function, arg, stack);
}

void thread_exit(void *retval){
    return clone1(retval);
}

int thread_join(int tid, void **retval){
    return clone2(tid, retval);
}

int gettid(void){
	return clone3();
}
