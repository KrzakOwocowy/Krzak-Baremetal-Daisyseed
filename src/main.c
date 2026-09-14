#include "setup.h"
#include "dsp.h"

DELAY_LINE(delay_1, 600);

void process_audio(const float* audio_in, float* audio_out, uint32_t size){
    delay(audio_in, audio_out, size, 500, 0.8, &delay_1);
}

int main(void){
    DELAY_LINE_INIT(delay_1);
    system_setup(process_audio);
    while(1){

    }
    return 0;
}
