#include <stdint.h>

extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

void main(void);

void Reset_Handler(void){
    uint32_t *src = &_sidata;
    uint32_t *dest = &_sdata;
    while (dest < &_edata){
        *dest++ = *src++;
    }
    dest = &_sbss;
    while (dest < &_ebss){
        *dest++ = 0;
    }
    main();
    while(1);
}

void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

void DMA1_Stream0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));

void Default_Handler(void){
    while(1);
}

__attribute__((section(".isr_vector"), used)) void (*const vector_table[28])(void) = {
    [0] = (void (*)(void))(&_estack),
    [1] = Reset_Handler,
    [2 ... 14] = Default_Handler,
    [15] = SysTick_Handler,
    [16 ... 26] = Default_Handler,
    [27] = DMA1_Stream0_IRQHandler
};
