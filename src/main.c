#include "setup.h"
#include "dsp.h"

DELAY_LINE(delay_1, 3300);

void process_audio(const float* audio_in, float* audio_out, uint32_t size){
    delay(audio_in, audio_out, size, 1000, 0.6, &delay_1);
}

int main(void){
    system_setup();
    DELAY_LINE_INIT(delay_1);
    system_start(process_audio);
    while(1){

    }
    return 0;
}
