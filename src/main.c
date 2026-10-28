#include "setup.h"
#include "dsp.h"

REVERB_LINE(reverb_1);

void process_audio(const float* audio_in, float* audio_out, uint32_t size){
    write_to_output(audio_in, audio_out, size);
    //reverb(audio_in, audio_out, size, 0.85, &reverb_1);
}

int main(void){
    system_setup();
    REVERB_LINE_INIT(reverb_1, 10000);
    system_start(process_audio);
    while(1){

    }
    return 0;
}
