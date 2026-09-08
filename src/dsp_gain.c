#include "dsp_gain.h"

void gain_hard_clip(const float* audio_in, float* audio_out, int size, const float threshold){
    for (int counter = 0; counter < size; counter++){
        if (audio_in[counter] > threshold){
            audio_out[counter] = threshold;
        }
        else if (audio_in[counter] < -threshold){
            audio_out[counter] = -threshold;
        }
        else{
            audio_out[counter] = audio_in[counter];
        }
    }
}
