#include <stdint.h>
#include "dsp_filter.h"

void filter(const float* audio_in, float* audio_out, uint32_t size, filter_line* target_filter_line){
    for (uint32_t counter = 0; counter < size; counter++){
        audio_out[counter] = (target_filter_line->b[0] * audio_in[counter]) +
        (target_filter_line->b[1] * target_filter_line->x[0]) +
        (target_filter_line->b[2] * target_filter_line->x[1]) -
        (target_filter_line->a[1] * target_filter_line->y[0]) -
        (target_filter_line->a[2] * target_filter_line->y[1]);
        target_filter_line->x[1] = target_filter_line->x[0];
        target_filter_line->x[0] = audio_in[counter];
        target_filter_line->y[1] = target_filter_line->y[0];
        target_filter_line->y[0] = audio_out[counter];
    }
}

void comb_filter(const float* audio_in, float* audio_out, uint32_t size, uint32_t delay_time_samples, float feedback, comb_filter_line* target_filter_line){
    if (delay_time_samples >= target_filter_line->delay_filter_line.size){
        delay_time_samples = target_filter_line->delay_filter_line.size - 1;
    }
    for (uint32_t counter = 0; counter < size; counter++){
        int32_t read_index = (int32_t)target_filter_line->delay_filter_line.counter - (int32_t)delay_time_samples;
        if (read_index < 0){
            read_index += (int32_t)target_filter_line->delay_filter_line.size;
        }
        float damped_sample = filter_sample(&target_filter_line->lp_filter_line, target_filter_line->delay_filter_line.buffer[read_index]);
        target_filter_line->delay_filter_line.buffer[target_filter_line->delay_filter_line.counter] = audio_in[counter] + damped_sample * feedback;
        audio_out[counter] = damped_sample;
        target_filter_line->delay_filter_line.counter++;
        if (target_filter_line->delay_filter_line.counter >= target_filter_line->delay_filter_line.size){
            target_filter_line->delay_filter_line.counter = 0;
        }
    }
}