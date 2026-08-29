#include "setup.h"

void process_audio(const float* audio_in, float* audio_out, int size){
    for (int counter = 0; counter < size; counter++){
        audio_out[counter] = audio_in[counter];
    }
}

int main(void){
    system_setup(process_audio);
    while(1){

    }
    return 0;
}
