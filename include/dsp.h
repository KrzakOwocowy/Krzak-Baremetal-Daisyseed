#ifndef DSP
#include "dsp_gain.h"

static inline void write_to_output(const float* audio_in, float* audio_out, int size){
    for (int counter = 0; counter < size; counter++){
        audio_out[counter] = audio_in[counter];
    }
}

#endif