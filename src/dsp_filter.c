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