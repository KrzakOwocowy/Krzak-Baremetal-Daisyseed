#include "stm32h7xx.h"
#include "utils.h"
#include <stdint.h>
#include "led.h"
#include "system_clock.h"
#include "audio_codec.h"

extern void(*const vector_table[])(void);

int main(void){
    SCB->VTOR = (uint32_t)&vector_table;
    led_setup();
    system_clock_config();
    audio_codec_setup();
    while(1){
        if ((SAI1_Block_B->SR & SAI_xSR_FLVL) != 0){
            volatile uint32_t incoming_sample = SAI1_Block_B->DR;
            SAI1_Block_A->DR = incoming_sample;
            led(on);
        }
    }
    return 0;
}
