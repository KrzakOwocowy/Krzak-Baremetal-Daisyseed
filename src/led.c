#include "stm32h7xx.h"
#include "utils.h"
#include <stdint.h>
#include "led.h"

void led_setup(void){
    GPIOC->MODER &= ~GPIO_MODER_MODE7_Msk;
    GPIOC->MODER |= GPIO_MODER_MODE7_0;
}

void led(led_state led_state){
    switch(led_state){
        case on:
            set_bit(&GPIOC->ODR, 7, 1);
            break;
        case off:
            set_bit(&GPIOC->ODR, 7, 0);
            break;
    }
}
