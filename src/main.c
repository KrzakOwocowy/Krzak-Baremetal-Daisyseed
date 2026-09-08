#include "setup.h"
#include "dsp.h"

void process_audio(const float* audio_in, float* audio_out, int size){
    //gain_hard_clip(audio_in, audio_out, size, 0.8);
    //write_to_output(audio_in, audio_out, size);
    for(int counter = 0; counter < size; counter++){
        ((volatile float*)audio_out)[counter] = ((volatile float*)audio_in)[counter];
    }
}

int main(void){
    system_setup(process_audio);
    while(1){

    }
    return 0;
}
