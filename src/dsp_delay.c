#include "dsp_delay.h"

void delay(const float* audio_in, float* audio_out, uint32_t size, uint32_t time_ms, float feedback, delay_line* target_delay_line){
    uint32_t delay_time_samples = (uint32_t)((SAMPLE_RATE * time_ms) / 1000.0);
    for (uint32_t counter = 0; counter < size; counter++){
        float input_sample = audio_in[counter];
        int32_t read_index = (int32_t)(target_delay_line->counter) - (int32_t)delay_time_samples;
        while (read_index < 0){
            read_index += (int32_t)target_delay_line->size;
        }
        float delayed_sample = input_sample + (target_delay_line->buffer[read_index] * feedback);
        audio_out[counter] = delayed_sample;
        target_delay_line->buffer[target_delay_line->counter] = delayed_sample;
        (target_delay_line->counter)++;
        if (target_delay_line->counter >= target_delay_line->size){
            target_delay_line->counter = 0;
        }
    }
}