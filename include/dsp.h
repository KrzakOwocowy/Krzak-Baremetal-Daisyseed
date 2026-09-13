#ifndef DSP
#define DSP
#include "dsp_gain.h"

//passes the signal through without doing anything to it, not necessary if any other dsp is used
static inline void write_to_output(const float* audio_in, float* audio_out, int size){
    for (int counter = 0; counter < size; counter++){
        audio_out[counter] = audio_in[counter];
    }
}

#endif