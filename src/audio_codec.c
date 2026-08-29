#include "stm32h7xx.h"
#include "utils.h"
#include <stdint.h>
#include "audio_codec.h"

#define SAI_FRAME_LENGTH 63U
#define SAI_FS_ACTIVE_HALF 31U

static inline void audio_codec_gpio_clock(void){
    set_bit(&RCC->AHB4ENR, RCC_AHB4ENR_GPIOBEN_Pos, 1);
    set_bit(&RCC->AHB4ENR, RCC_AHB4ENR_GPIOEEN_Pos, 1);
    volatile uint32_t dummy = RCC->AHB4ENR;
    (void)dummy;
    GPIOB->MODER &= ~GPIO_MODER_MODE11_Msk;
    GPIOB->MODER |= GPIO_MODER_MODE11_0;
    GPIOB->ODR |= (1U << 11);
}

static inline void audio_codec_gpio_mode(void){
    GPIOE->MODER &= ~GPIO_MODER_MODE2_Msk;
    GPIOE->MODER |= GPIO_MODER_MODE2_1;
    GPIOE->MODER &= ~GPIO_MODER_MODE3_Msk;
    GPIOE->MODER |= GPIO_MODER_MODE3_1;
    GPIOE->MODER &= ~GPIO_MODER_MODE4_Msk;
    GPIOE->MODER |= GPIO_MODER_MODE4_1;
    GPIOE->MODER &= ~GPIO_MODER_MODE5_Msk;
    GPIOE->MODER |= GPIO_MODER_MODE5_1;
    GPIOE->MODER &= ~GPIO_MODER_MODE6_Msk;
    GPIOE->MODER |= GPIO_MODER_MODE6_1;
}

static inline void audio_codec_gpio_alternate_function(void){
    GPIOE->AFR[0] &= ~GPIO_AFRL_AFSEL2_Msk;
    GPIOE->AFR[0] |= GPIO_AFRL_AFSEL2_1;
    GPIOE->AFR[0] |= GPIO_AFRL_AFSEL2_2;
    GPIOE->AFR[0] &= ~GPIO_AFRL_AFSEL3_Msk;
    GPIOE->AFR[0] |= GPIO_AFRL_AFSEL3_1;
    GPIOE->AFR[0] |= GPIO_AFRL_AFSEL3_2;
    GPIOE->AFR[0] &= ~GPIO_AFRL_AFSEL4_Msk;
    GPIOE->AFR[0] |= GPIO_AFRL_AFSEL4_1;
    GPIOE->AFR[0] |= GPIO_AFRL_AFSEL4_2;
    GPIOE->AFR[0] &= ~GPIO_AFRL_AFSEL5_Msk;
    GPIOE->AFR[0] |= GPIO_AFRL_AFSEL5_1;
    GPIOE->AFR[0] |= GPIO_AFRL_AFSEL5_2;
    GPIOE->AFR[0] &= ~GPIO_AFRL_AFSEL6_Msk;
    GPIOE->AFR[0] |= GPIO_AFRL_AFSEL6_1;
    GPIOE->AFR[0] |= GPIO_AFRL_AFSEL6_2;
}

static inline void audio_codec_gpio_set_speed(void){
    GPIOE->OSPEEDR |= GPIO_OSPEEDR_OSPEED2_0;
    GPIOE->OSPEEDR |= GPIO_OSPEEDR_OSPEED2_1;
    GPIOE->OSPEEDR |= GPIO_OSPEEDR_OSPEED3_0;
    GPIOE->OSPEEDR |= GPIO_OSPEEDR_OSPEED3_1;
    GPIOE->OSPEEDR |= GPIO_OSPEEDR_OSPEED4_0;
    GPIOE->OSPEEDR |= GPIO_OSPEEDR_OSPEED4_1;
    GPIOE->OSPEEDR |= GPIO_OSPEEDR_OSPEED5_0;
    GPIOE->OSPEEDR |= GPIO_OSPEEDR_OSPEED5_1;
    GPIOE->OSPEEDR |= GPIO_OSPEEDR_OSPEED6_0;
    GPIOE->OSPEEDR |= GPIO_OSPEEDR_OSPEED6_1;
}

static inline void audio_codec_gpio(void){
    audio_codec_gpio_clock();
    audio_codec_gpio_mode();
    audio_codec_gpio_alternate_function();
    audio_codec_gpio_set_speed();
}

static inline void audio_codec_clock(void){
    RCC->D2CCIP1R &= ~RCC_D2CCIP1R_SAI1SEL_Msk;
    RCC->D2CCIP1R |= RCC_D2CCIP1R_SAI1SEL_0;
}

static inline void audio_codec_sai_enable(void){
    RCC->APB2ENR |= RCC_APB2ENR_SAI1EN;
}

static inline void audio_codec_sai_configuration(void){
    SAI1_Block_A->CR1 &= ~SAI_xCR1_SAIEN;
    SAI1_Block_A->CR1 |= SAI_xCR1_DS_2 | SAI_xCR1_DS_1 | SAI_xCR1_MCKEN | SAI_xCR1_DMAEN;
    SAI1_Block_A->FRCR = (SAI_FRAME_LENGTH << SAI_xFRCR_FRL_Pos) | (SAI_FS_ACTIVE_HALF << SAI_xFRCR_FSALL_Pos) | SAI_xFRCR_FSPOL;
    SAI1_Block_A->SLOTR = SAI_xSLOTR_NBSLOT_0 | SAI_xSLOTR_SLOTSZ_1 | (((1U << 0) | (1U << 1)) << SAI_xSLOTR_SLOTEN_Pos);
    SAI1_Block_B->FRCR = SAI1_Block_A->FRCR;
    SAI1_Block_B->SLOTR = SAI1_Block_A->SLOTR;
    SAI1_Block_B->CR1 &= ~SAI_xCR1_SAIEN;
    SAI1_Block_B->CR1 = SAI_xCR1_MODE_1 | SAI_xCR1_MODE_0 | SAI_xCR1_SYNCEN_0 | SAI_xCR1_DS_2 | SAI_xCR1_DS_1 | SAI_xCR1_DMAEN;
}

static inline void audio_codec_sai_start(void){
    SAI1_Block_B->CR1 |= SAI_xCR1_SAIEN;
    SAI1_Block_A->CR1 |= SAI_xCR1_SAIEN;
}

void audio_codec_setup(void){
    audio_codec_gpio();
    audio_codec_clock();
    audio_codec_sai_enable();
    audio_codec_sai_configuration();
    audio_codec_sai_start();
}
