#include "setup.h"
#include "dsp.h"

#define DELAY_BUFFER_SIZE (96 * 300)

float delay_buffer[DELAY_BUFFER_SIZE];
uint32_t delay_counter = 0;

void process_audio(const float* audio_in, float* audio_out, uint32_t size){
    delay(audio_in, audio_out, size, 0.5f, 0.5f, delay_buffer, DELAY_BUFFER_SIZE, &delay_counter);
}

int main(void){
    system_setup(process_audio);
    while(1){

    }
    return 0;
}
