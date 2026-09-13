#include "setup.h"
#include "dsp.h"

void process_audio(const float* audio_in, float* audio_out, int size){
    write_to_output(audio_in, audio_out, size);
}

int main(void){
    system_setup(process_audio);
    while(1){

    }
    return 0;
}
