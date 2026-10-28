#include "setup.h"

void system_setup(void){
    SCB_EnableDCache();
    SCB_EnableICache();
    SCB->VTOR = (uint32_t)&vector_table;
    led_setup();
    system_clock_setup();
    sdram_setup();
    audio_codec_setup();
}

void system_start(audio_callback callback){
    dma_set_audio_callback(callback);
    dma_setup();
    audio_codec_sai_start();
}
