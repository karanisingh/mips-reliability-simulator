#include "mips_reliability.h"
#include "reliability_logging.h"
#include "mips_helper.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

/////////////////////////////////////////////////////
// Function to initialize the random number generator. Call this once before calling fail_simulate.
void init_random() {
    // Use current time as seed for random number generator
    srand(time(NULL));
}

// Function to simulate failure based on exponential distribution.
// Returns 1 if the component fails by time 't', 0 otherwise.
int fail_simulate(double t, double MTTF) {
    double lambda = 1.0 / MTTF;
    double random_value = (double)rand() / RAND_MAX;    
    double L = exp(-lambda);
    double p = 1.0;
    int k = 0;

    do {
        k++;
        double u = ((double) rand() / RAND_MAX);
        p *= u;
    } while (p > L);

    return (random_value < k - 1);
}

//////////////////////////////////////////////















///////////////////////////////////////////////////////////////////////////////
//
//ALU_R ALU_RELIABILITY = { 2000, 0, 10, 0, 0, 0 }; // This should be uncommented out when we want spare redundancy implemented
ALU_R ALU_RELIABILITY = { 500, 0, 10, 6, 10, 0 };



int ALU_simFailure()
{
    // update lifetime     
    ALU_RELIABILITY.lifetime++;

    // int to hold whether a failure happened for logging
    int failure=0;

    if(ALU_RELIABILITY.N>0)       //if NMR
    {
        // one component can fail at a time
        if(fail_simulate(ALU_RELIABILITY.lifetime, ALU_RELIABILITY.MTTF)){
            log_write("ALU NMR structure fail");
            cycle += 1;                     // 1 cycle penalty
            ALU_RELIABILITY.spareId++;      // move to the next spare
            failure = 1;                    // failure flag
            ALU_RELIABILITY.lifetime = 1;   // reset lifetime (to 1, since spare redoes action)
            ALU_RELIABILITY.working--;
            if (ALU_RELIABILITY.working < ALU_RELIABILITY.total) {
                int failed = ALU_RELIABILITY.total-ALU_RELIABILITY.working;
                log_write("ALU NMR experienced %d total fails, there are %d remaining", failed, ALU_RELIABILITY.working);
            }
        }
    }
    else            // if STANDBY SPARING
    {
        // one component can fail at a time
        if(fail_simulate(ALU_RELIABILITY.lifetime, ALU_RELIABILITY.MTTF)){
            log_write("ALU Spare %d of %d structure fail", ALU_RELIABILITY.spareId, ALU_RELIABILITY.total);
            cycle += 1;                     // 1 cycle penalty
            ALU_RELIABILITY.spareId++;      // move to the next spare
            failure = 1;                    // failure flag
            ALU_RELIABILITY.lifetime = 1;   // reset lifetime (to 1, since spare redoes action)
        }
    }

    if(ALU_isFailed())
    {
        log_write("ALU unrecoverable failure");
        return 1;
    }
    else if(failure){
        log_write("ALU recovered from a failure");
        return 0;
    }

    return 0;
}

int ALU_isFailed()
{
    if(ALU_RELIABILITY.N)
    {
        if(ALU_RELIABILITY.working >= ALU_RELIABILITY.N)
            return 0;
        else   
            return 1;
    }
    else
    {
        if(ALU_RELIABILITY.spareId < ALU_RELIABILITY.total)
            return 0;
        else    
            return 1;
    }

}
/////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
//
// Checkpointing
proc_cp PROCESSOR_CHECKPOINT;

int saveProcessorState()
{
    memcpy(PROCESSOR_CHECKPOINT.dataMemoryCP, dataMemory, sizeof(dataMemory));
    memcpy(PROCESSOR_CHECKPOINT.registerFileCP, registerFile, sizeof(registerFile));

    // printf("REG");
    // for(int i = 0; i < 32; i++)
    // {
    //     printf("Saved: %s\nCurrent%s\n\n\n", PROCESSOR_CHECKPOINT.registerFileCP[i].registerData, registerFile[i].registerData);
    // }

    // printf("MEM");
    // for(int i = 0; i < 5; i++)
    //     printf("Saved: %s\nCurrent%s\n\n\n", PROCESSOR_CHECKPOINT.dataMemoryCP[i].DataLine, dataMemory[i].DataLine);


    PROCESSOR_CHECKPOINT.PC_CP = PC;
    // printf("PC: S:%d, C%d", PROCESSOR_CHECKPOINT.PC_CP, PC);
    log_write("Saving processor state at PC:%d", PC);

    return 0;
}

int resetProcessorState()
{
    // printf("-\n-\n-\n RESET\n");
    // printf("REG");
    // for(int i = 0; i < 32; i++)
    // {
    //     printf("Saved: %s\nCurrent%s\n\n\n", PROCESSOR_CHECKPOINT.registerFileCP[i].registerData, registerFile[i].registerData);
    // }

    // printf("MEM");
    // for(int i = 0; i < 5; i++)
    //     printf("Saved: %s\nCurrent%s\n\n\n", PROCESSOR_CHECKPOINT.dataMemoryCP[i].DataLine, dataMemory[i].DataLine);
    log_write("Reseting processor state from PC:%d back to saved PC:%d", PC, PROCESSOR_CHECKPOINT.PC_CP);

    memcpy( dataMemory,PROCESSOR_CHECKPOINT.dataMemoryCP, sizeof(dataMemory));
    memcpy( registerFile,PROCESSOR_CHECKPOINT.registerFileCP, sizeof(registerFile));


    PC = PROCESSOR_CHECKPOINT.PC_CP;
    // printf("PC: S:%d, C%d", PROCESSOR_CHECKPOINT.PC_CP, PC);
    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////
