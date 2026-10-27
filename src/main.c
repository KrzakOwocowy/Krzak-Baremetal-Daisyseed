#include "setup.h"
#include "dsp.h"

FILTER_LINE(filter_1, 1000, LOWPASS, 0.9);

void process_audio(const float* audio_in, float* audio_out, uint32_t size){
    filter(audio_in, audio_out, size, &filter_1);
}

int main(void){
    system_setup();
    FILTER_LINE_INIT(filter_1);
    system_start(process_audio);
    while(1){

    }
    return 0;
}
