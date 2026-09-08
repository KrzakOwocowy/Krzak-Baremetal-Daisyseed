#ifndef DSP_GAIN_H
#define DSP_GAIN_H

void gain_hard_clip(const float* audio_in, float* audio_out, int size, const float threshold);

#endif