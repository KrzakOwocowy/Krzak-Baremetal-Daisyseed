#include "dsp_gain.h"

//simple hardclip of the signal, threshold takes the values from 0 to 1
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

//applies a warm distortion, pre and post gain take values from 0 to 1
void gain_tanh_distortion(const float* audio_in, float* audio_out, int size, const float pregain, const float postgain){
    float gain_multiplier = 3 * (1 + pregain);
    for (int counter = 0; counter < size; counter++){
        audio_out[counter] = audio_in[counter] * gain_multiplier;
        if (audio_out[counter] < -3){
            audio_out[counter] = -1;
        }
        else if (audio_out[counter] > 3){
            audio_out[counter] = 1;
        }
        else{
            float squared = audio_out[counter] * audio_out[counter];
            audio_out[counter] *= (27 + squared) / (27 + 9 * squared);
        }
        audio_out[counter] *= postgain;
    }
}