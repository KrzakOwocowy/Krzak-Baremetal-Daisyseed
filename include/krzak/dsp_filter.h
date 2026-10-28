#ifndef DSP_FILTER_H
#define DSP_FILTER_H

#include <math.h>
#include "dsp_delay.h"

#define SAMPLE_RATE 48000
#define PI 3.14159265f

typedef struct {
    float theta;
    float K;
    float W;
    float alpha;
    float a[3];
    float b[3];
    float x[2];
    float y[2];
} filter_line;

typedef struct {
    filter_line lp_filter_line;
    delay_line delay_filter_line;
} comb_filter_line;

typedef enum {
    LOWPASS,
    HIGHPASS
} filter_type;

static inline void filter_line_init(filter_line* target_filter_line, float cutoff_frequency, float Q, filter_type filter_type){
    target_filter_line->x[0] = 0.0f;
    target_filter_line->x[1] = 0.0f;
    target_filter_line->y[0] = 0.0f;
    target_filter_line->y[1] = 0.0f;
    target_filter_line->theta = 2.0f * PI * cutoff_frequency / SAMPLE_RATE;
    target_filter_line->K = tanf(target_filter_line->theta / 2.0f);
    target_filter_line->W = target_filter_line->K * target_filter_line->K;
    target_filter_line->alpha = 1.0f + (target_filter_line->K / Q) + target_filter_line->W;
    target_filter_line->a[0] = 1.0f;
    target_filter_line->a[1] = 2.0f * (target_filter_line->W - 1.0f) / target_filter_line->alpha;
    target_filter_line->a[2] = (1.0f - target_filter_line->K / Q + target_filter_line->W) / target_filter_line->alpha;
    switch (filter_type){
        case(LOWPASS):
            target_filter_line->b[0] = target_filter_line->W / target_filter_line->alpha;
            target_filter_line->b[1] = 2.0f * target_filter_line->W / target_filter_line->alpha;
            target_filter_line->b[2] = target_filter_line->b[0];
            break;
        case(HIGHPASS):
            target_filter_line->b[0] = 1.0f / target_filter_line->alpha;
            target_filter_line->b[1] = -2.0f / target_filter_line->alpha;
            target_filter_line->b[2] = target_filter_line->b[0];
            break;
        default: //default to low-pass
            target_filter_line->b[0] = target_filter_line->W / target_filter_line->alpha;
            target_filter_line->b[1] = 2.0f * target_filter_line->W / target_filter_line->alpha;
            target_filter_line->b[2] = target_filter_line->b[0];
            break;
    }
}

static inline float filter_sample(filter_line* target_filter_line, float audio_in){
    float audio_out = (target_filter_line->b[0] * audio_in) + (target_filter_line->b[1] * target_filter_line->x[0]) + (target_filter_line->b[2] * target_filter_line->x[1]) -
    (target_filter_line->a[1] * target_filter_line->y[0]) - (target_filter_line->a[2] * target_filter_line->y[1]);
    target_filter_line->x[1] = target_filter_line->x[0];
    target_filter_line->x[0] = audio_in;
    target_filter_line->y[1] = target_filter_line->y[0];
    target_filter_line->y[0] = audio_out;
    return audio_out;
}

static inline void comb_filter_line_init(comb_filter_line* target_filter_line, float* buffer, uint32_t buffer_size, float damping_cutoff, float Q){
    delay_line_init(&target_filter_line->delay_filter_line, buffer, buffer_size);
    filter_line_init(&target_filter_line->lp_filter_line, damping_cutoff, Q, LOWPASS);
}

#define FILTER_LINE(name, cutoff, type, q) \
    static filter_line name; \
    static float name##_cutoff = cutoff; \
    static float name##_q = q; \
    static filter_type name##_filter_type = type

#define FILTER_LINE_INIT(name) \
    filter_line_init(&name, name##_cutoff, name##_q, name##_filter_type)

#define COMB_FILTER_LINE(name, max_samples, damping_cutoff, q) \
    __attribute__((section(".sdram_data"))) static float name##_data[(max_samples) + 1]; \
    static comb_filter_line name; \
    static float name##_cutoff = damping_cutoff; \
    static float name##_q = q

#define COMB_FILTER_LINE_INIT(name) \
    comb_filter_line_init(&name, name##_data, sizeof(name##_data)/sizeof(float), name##_cutoff, name##_q)

void filter(const float* audio_in, float* audio_out, uint32_t size, filter_line* target_filter_line);

void comb_filter(const float* audio_in, float* audio_out, uint32_t size, uint32_t delay_time_samples, float feedback, comb_filter_line* target_filter_line);

#endif