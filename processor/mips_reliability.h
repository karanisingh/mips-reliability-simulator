// mips_reliability.h

#ifndef MIPS_RELIABILITY_H
#define MIPS_RELIABILITY_H

#include "mips_helper.h"

void init_random();


/////////////////////////////////////////////////////////////////
//
// ALU STRUCTURAL DUPLICATION
typedef struct ALU_Reliability {
	int MTTF;       // mean time (iteration) to fail
	int lifetime;   // current lifetime
    int total;      // Amount of redundant structures

    // NMR
	int N;          // N of NMR (or 0 if not NMR)
	int working;    // current working structures

    // Standby Sparing (no overhead for switching)
	int spareId;    // current spare ID
} ALU_R;

extern ALU_R ALU_RELIABILITY;
int ALU_simFailure();
int ALU_isFailed();
//////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////
//
// Checkpointing
typedef struct processor_state {
    struct instruction instructionMemoryCP[16];
    struct data dataMemoryCP[5];
    struct registers registerFileCP[32];
    int PC_CP;
} proc_cp;

extern proc_cp PROCESSOR_CHECKPOINT;

int saveProcessorState();
int resetProcessorState();
///////////////////////////////////////////////////////////////////














#endif // MIPS_RELIABILITY_H