//THIS PROGRAM WILL BLINK THE LED ON AND OFF

#include "stm32h7xx.h"
#include "utils.h"
#include <stdint.h>
#include "led.h"
#include "system_clock.h"

extern void(*const vector_table[])(void);

volatile uint32_t timer_millis = 0;

void SysTick_Handler(void){
    timer_millis++;
    if  (timer_millis >= 500){
        GPIOC->ODR ^= (1 << 7);
        timer_millis = 0;
    }  
}

int main(void){
    SCB->VTOR = (uint32_t)&vector_table;
    __enable_irq();
    system_clock_config();
    led_setup();
    SysTick_Config(400000);
    while(1){
    }
    return 0;
}
