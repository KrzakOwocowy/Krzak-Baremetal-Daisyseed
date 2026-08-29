#include "stm32h7xx.h"
#include "utils.h"
#include <stdint.h>
#include "led.h"

static inline void led_setup_gpio_clock(void){
    set_bit(&RCC->AHB4ENR, RCC_AHB4ENR_GPIOCEN_Pos, 1);
    volatile uint32_t dummy = RCC->AHB4ENR;
    (void)dummy;
}

static inline void led_setup_pin(void){
    GPIOC->MODER &= ~GPIO_MODER_MODE7_Msk;
    GPIOC->MODER |= GPIO_MODER_MODE7_0;
}

void led_setup(void){
    led_setup_gpio_clock();
    led_setup_pin();
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
