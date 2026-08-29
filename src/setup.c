#include "setup.h"

void system_setup(audio_callback callback){
    enable_fpu();
    SCB_EnableDCache();
    SCB_EnableICache();
    SCB->VTOR = (uint32_t)&vector_table;
    led_setup();
    system_clock_setup();
    audio_codec_setup();
    dma_set_audio_callback(callback);
    dma_setup();
}
