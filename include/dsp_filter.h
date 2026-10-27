#ifndef DSP_FILTER_H
#define DSP_FILTER_H

#include <math.h>
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

#define FILTER_LINE(name, cutoff, type, q) \
    static filter_line name; \
    static float name##_cutoff = cutoff; \
    static float name##_q = q; \
    static filter_type name##_filter_type = type

#define FILTER_LINE_INIT(name) \
    filter_line_init(&name, name##_cutoff, name##_q, name##_filter_type)

void filter(const float* audio_in, float* audio_out, uint32_t size, filter_line* target_filter_line);

#endif