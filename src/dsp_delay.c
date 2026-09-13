#include "dsp_delay.h"

void delay(const float* audio_in, float* audio_out, uint32_t size, float time, float feedback, float* buffer, uint32_t buffer_size, uint32_t* buffer_counter){
    uint32_t delay_time_samples = (uint32_t)(SAMPLE_RATE * time);
    for (uint32_t counter = 0; counter < size; counter++){
        float input_sample = audio_in[counter];
        int32_t read_index = (int32_t)(*buffer_counter) - (int32_t)delay_time_samples;
        while (read_index < 0){
            read_index += (int32_t)buffer_size;
        }
        float delayed_sample = input_sample + (buffer[read_index] * feedback);
        audio_out[counter] = delayed_sample;
        buffer[*buffer_counter] = delayed_sample;
        (*buffer_counter)++;
        if (*buffer_counter >= buffer_size){
            *buffer_counter = 0;
        }
    }
}