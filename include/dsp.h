#ifndef DSP
#define DSP
#include "dsp_gain.h"
#include "dsp_delay.h"

//passes the signal through without doing anything to it, not necessary if any other dsp is used
static inline void write_to_output(const float* audio_in, float* audio_out, uint32_t size){
    for (uint32_t counter = 0; counter < size; counter++){
        audio_out[counter] = audio_in[counter];
    }
}

#endif