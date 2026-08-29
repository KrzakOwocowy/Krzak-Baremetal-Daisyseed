#include "stm32h7xx.h"
#include "utils.h"
#include <stdint.h>
#include "system_clock.h"

static inline void set_voltage_scale(void){
    PWR->CR3 |= PWR_CR3_LDOEN;
    while((PWR->CSR1 & PWR_CSR1_ACTVOSRDY) == 0){
        
    }
    PWR->D3CR &= ~PWR_D3CR_VOS_Msk;
    PWR->D3CR |= (3 << PWR_D3CR_VOS_Pos);
    while((PWR->D3CR & PWR_D3CR_VOSRDY) == 0){

    }
}

static inline void enable_hse_osc(void){
    RCC->CR |= RCC_CR_HSEON;
    while((RCC->CR & RCC_CR_HSERDY) == 0){

    }
}

static inline void flash_latency_setup(void){
    FLASH->ACR = FLASH_ACR_WRHIGHFREQ_1 | FLASH_ACR_LATENCY_4WS;
    while((FLASH->ACR & FLASH_ACR_LATENCY_Msk) != FLASH_ACR_LATENCY_4WS){

    }
}

static inline void pll_setup(void){
    //PLL1 - core clock
    RCC->CR &= ~RCC_CR_PLL1ON;
    while((RCC->CR & RCC_CR_PLL1RDY) != 0){

    }
    RCC->PLLCKSELR &= ~(RCC_PLLCKSELR_PLLSRC_Msk | RCC_PLLCKSELR_DIVM1_Msk);
    RCC->PLLCKSELR |= RCC_PLLCKSELR_PLLSRC_HSE | (1 << RCC_PLLCKSELR_DIVM1_Pos);
    RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLL1VCOSEL_Msk | RCC_PLLCFGR_PLL1RGE_Msk);
    RCC->PLLCFGR |= RCC_PLLCFGR_DIVP1EN | (3 << RCC_PLLCFGR_PLL1RGE_Pos);
    RCC->PLL1DIVR &= ~(RCC_PLL1DIVR_N1_Msk | RCC_PLL1DIVR_P1_Msk);
    RCC->PLL1DIVR |= (49 << RCC_PLL1DIVR_N1_Pos) | (1 << RCC_PLL1DIVR_P1_Pos);
    RCC->CR |= RCC_CR_PLL1ON;
    while((RCC->CR & RCC_CR_PLL1RDY) == 0){

    }
    //PLL2 - PCM3060 clock
    RCC->CR &= ~RCC_CR_PLL2ON;
    while((RCC->CR & RCC_CR_PLL2RDY) != 0){

    }
    RCC->PLLCKSELR &= ~(RCC_PLLCKSELR_PLLSRC_Msk | RCC_PLLCKSELR_DIVM2_Msk);
    RCC->PLLCKSELR |= RCC_PLLCKSELR_PLLSRC_HSE | (5 << RCC_PLLCKSELR_DIVM2_Pos);
    RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLL2VCOSEL_Msk | RCC_PLLCFGR_PLL2RGE_Msk);
    RCC->PLLCFGR |= RCC_PLLCFGR_DIVP2EN | (1 << RCC_PLLCFGR_PLL2RGE_Pos);
    RCC->PLL2DIVR &= ~(RCC_PLL2DIVR_N2_Msk);
    RCC->PLL2DIVR |= (191 << RCC_PLL2DIVR_N2_Pos) | (49 << RCC_PLL2DIVR_P2_Pos);
    RCC->CR |= RCC_CR_PLL2ON;
    while((RCC->CR & RCC_CR_PLL2RDY) == 0){

    }
}

static inline void configure_bus_prescaler(void){
    RCC->D1CFGR = RCC_D1CFGR_D1CPRE_DIV1 | RCC_D1CFGR_D1PPRE_DIV2 | RCC_D1CFGR_HPRE_DIV2;
    RCC->D2CFGR = RCC_D2CFGR_D2PPRE1_DIV2 | RCC_D2CFGR_D2PPRE2_DIV2;
    RCC->D3CFGR = RCC_D3CFGR_D3PPRE_DIV2;
}

static inline void enable_pll(void){
    RCC->CFGR |= RCC_CFGR_SW_PLL1;
    while((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_PLL1){

    }
}

void system_clock_setup(void){
    set_voltage_scale();
    enable_hse_osc();
    flash_latency_setup();
    pll_setup();
    configure_bus_prescaler();
    enable_pll();
}

