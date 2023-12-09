#include "mips_reliability.h"
#include "reliability_logging.h"
#include "mips_helper.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

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
    double cdf = 1 - exp(-lambda * t);
    
    return (random_value < cdf);
}

//////////////////////////////////////////////


ALU_R ALU_RELIABILITY = { 500, 0, 0, 3, 0, 0 };

// 1 if unrecoverable failure

int ALU_simFailure()
{
    // update lifetime idk lol
    ALU_RELIABILITY.lifetime++;

    // int to hold whether a failure happened for logging
    int failure=0;

    if(ALU_RELIABILITY.N)       //if NMR
    {
        //failures in the NMR system
        int fails=0;
        // any of the working components can fail
        for(int i = 0; i < ALU_RELIABILITY.working; i++)
        {
            // if fail
            if(fail_simulate(ALU_RELIABILITY.lifetime, ALU_RELIABILITY.MTTF)){
                fails++;
                log_write("ALU NMR structure fail");
            }

        }
        failure=fails;
        // need to remove the modules that fail
        ALU_RELIABILITY.working -= fails;

    }
    else            // if STANDBY SPARING
    {
        // one component canfail at a time
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

