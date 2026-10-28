#include "dsp_reverb.h"

void reverb(const float* audio_in, float* audio_out, uint32_t size, float decay, reverb_line* target_reverb_line){
    float temp_buffer[size];
    float wet[size];
    for (uint32_t counter=0; counter < size; counter++){
        wet[counter] = 0.0f;
    }
    for (uint32_t counter=0; counter<FILTER_LINE_COUNT; counter++){
        comb_filter(audio_in, temp_buffer, size, comb_filter_values[counter], powf(10.0f, -3.0f * comb_filter_values[counter] / (decay * SAMPLE_RATE)), &target_reverb_line->comb_filter_lines[counter]);
        for (uint32_t inner_counter = 0; inner_counter < size; inner_counter++){
            wet[inner_counter] += temp_buffer[inner_counter];
        }
    }
    for (uint32_t counter = 0; counter < size; counter++){
        audio_out[counter] = audio_in[counter] + (wet[counter] / FILTER_LINE_COUNT);
    }
}