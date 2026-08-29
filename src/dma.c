#include "stm32h7xx.h"
#include "utils.h"
#include <stdint.h>
#include "dma.h"

#define DMAMUX1_SAI1_A_DMA 87
#define DMAMUX1_SAI1_B_DMA 88

__attribute__((section(".dma_buffer"), aligned(32))) volatile uint32_t rx_buffer[BUFFER_SIZE];
__attribute__((section(".dma_buffer"), aligned(32))) volatile uint32_t tx_buffer[BUFFER_SIZE];

static float float_in[AUDIO_BLOCK_SIZE];
static float float_out[AUDIO_BLOCK_SIZE];

const float SCALE_DOWN = 1.0f/2147483648.0f;
const float SCALE_UP = 2147483647.0f;

static audio_callback current_callback = 0;

static inline void dma_clock_enable(void){
    set_bit(&RCC->AHB1ENR, RCC_AHB1ENR_DMA1EN_Pos, 1);
    volatile uint32_t dummy = RCC->AHB1ENR;
    (void)dummy;
}

static inline void dma_mux_configuration(void){
    DMAMUX1_Channel0->CCR = DMAMUX1_SAI1_B_DMA;
    DMAMUX1_Channel1->CCR = DMAMUX1_SAI1_A_DMA;
}

static inline void dma_configuration(void){
    DMA1_Stream0->CR &= ~DMA_SxCR_EN;
    while((DMA1_Stream0->CR & DMA_SxCR_EN) != 0){

    }
    DMA1_Stream0->PAR = (uint32_t)&SAI1_Block_B->DR;
    DMA1_Stream0->M0AR = (uint32_t)rx_buffer;
    DMA1_Stream0->NDTR = BUFFER_SIZE;
    DMA1_Stream0->CR = DMA_SxCR_MINC | DMA_SxCR_CIRC | DMA_SxCR_TCIE | DMA_SxCR_HTIE | DMA_SxCR_PSIZE_1 | DMA_SxCR_MSIZE_1;

    DMA1_Stream1->CR &= ~DMA_SxCR_EN;
    while((DMA1_Stream1->CR & DMA_SxCR_EN) != 0){

    }
    DMA1_Stream1->PAR = (uint32_t)&SAI1_Block_A->DR;
    DMA1_Stream1->M0AR = (uint32_t)tx_buffer;
    DMA1_Stream1->NDTR = BUFFER_SIZE;
    DMA1_Stream1->CR = DMA_SxCR_MINC | DMA_SxCR_CIRC | DMA_SxCR_PSIZE_1 | DMA_SxCR_MSIZE_1 | DMA_SxCR_DIR_0;
}

static inline void dma_start(void){
    NVIC_EnableIRQ(DMA1_Stream0_IRQn);
    DMA1_Stream0->CR |= DMA_SxCR_EN;
    DMA1_Stream1->CR |= DMA_SxCR_EN;
}

void dma_setup(void){
    dma_clock_enable();
    dma_mux_configuration();
    dma_configuration();
    for(int counter=0; counter<BUFFER_SIZE; counter++){
        rx_buffer[counter] = 0;
        tx_buffer[counter] = 0;
    }
    dma_start();
}

static inline void convert_and_callback(volatile uint32_t* audio_in, volatile uint32_t* audio_out, int size){
    for (int counter = 0; counter < size; counter++){
        int32_t raw_int = (int32_t)audio_in[counter];
        float_in[counter] = (float)raw_int * SCALE_DOWN;
    }

    if(current_callback){
        current_callback(float_in, float_out, size);
    }

    for (int counter = 0; counter < size; counter++){
        int32_t out_int = (int32_t)(float_out[counter] * SCALE_UP);
        audio_out[counter] = (uint32_t)out_int;
    }
}

void dma_set_audio_callback(audio_callback callback){
    current_callback = callback;
}

void DMA1_Stream0_IRQHandler(void){
    if ((DMA1->LISR & DMA_LISR_HTIF0) != 0){
        DMA1->LIFCR = DMA_LIFCR_CHTIF0;
        SCB_InvalidateDCache_by_Addr((uint32_t*)&rx_buffer[0], AUDIO_BLOCK_SIZE * 4);
        convert_and_callback(&rx_buffer[0], &tx_buffer[0], AUDIO_BLOCK_SIZE);
        SCB_CleanDCache_by_Addr((uint32_t*)&tx_buffer[0], AUDIO_BLOCK_SIZE * 4);
    }
    if ((DMA1->LISR & DMA_LISR_TCIF0) != 0){
        DMA1->LIFCR = DMA_LIFCR_CTCIF0;
        SCB_InvalidateDCache_by_Addr((uint32_t*)&rx_buffer[AUDIO_BLOCK_SIZE], AUDIO_BLOCK_SIZE * 4);
        convert_and_callback(&rx_buffer[AUDIO_BLOCK_SIZE], &tx_buffer[AUDIO_BLOCK_SIZE], AUDIO_BLOCK_SIZE);
        SCB_CleanDCache_by_Addr((uint32_t*)&tx_buffer[AUDIO_BLOCK_SIZE], AUDIO_BLOCK_SIZE * 4);
    }
}
