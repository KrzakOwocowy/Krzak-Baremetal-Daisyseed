#ifndef DSP_REVERB_H
#define DSP_REVERB_H

#include "dsp_filter.h"
#include <stdint.h>

#define SAMPLE_RATE 48000

#define FILTER_LINE_COUNT 6
#define FILTER_DAMPING_Q 0.5f

static const uint32_t comb_filter_values[FILTER_LINE_COUNT] = {1499, 1601, 1709, 1811, 1913, 2011};
#define REVERB_BUFFER_SIZE (1499+1601+1709+1811+1913+2011+FILTER_LINE_COUNT)

typedef struct {
    comb_filter_line comb_filter_lines[FILTER_LINE_COUNT];
} reverb_line;

static inline void reverb_line_init(reverb_line* target_reverb_line, float* buffer, float damping_cutoff){
    uint32_t offset = 0;
    for (uint32_t counter = 0; counter < FILTER_LINE_COUNT; counter++){
        comb_filter_line_init(&target_reverb_line->comb_filter_lines[counter], buffer+offset, comb_filter_values[counter]+1, damping_cutoff, FILTER_DAMPING_Q);
        offset += comb_filter_values[counter]+1;
    }
}

#define REVERB_LINE(name) \
    __attribute__((section(".sdram_data"))) static float name##_data[REVERB_BUFFER_SIZE]; \
    reverb_line name

#define REVERB_LINE_INIT(name, cutoff) \
    reverb_line_init(&name, name##_data, cutoff)

void reverb(const float* audio_in, float* audio_out, uint32_t size, float decay, reverb_line* target_reverb_line);

#endif