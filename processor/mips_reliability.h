// mips_reliability.h

#ifndef MIPS_RELIABILITY_H
#define MIPS_RELIABILITY_H


void init_random();


/////////////////////////////////////////////////////////////////
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




#endif // MIPS_RELIABILITY_H