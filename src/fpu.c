#include "fpu.h"
#include "stm32h7xx.h"

#define SCB_CPACR_CP10 (3UL << 20)
#define SCB_CPACR_CP11 (3UL << 22)

void enable_fpu(){
    SCB->CPACR |= (SCB_CPACR_CP10 | SCB_CPACR_CP11);
}
