#ifndef DMA_H
#define DMA_H

#include <stdint.h>

//set up DMA
void dma_setup(void);

#define AUDIO_BLOCK_SIZE 48
#define BUFFER_SIZE (AUDIO_BLOCK_SIZE * 2 )

extern volatile uint32_t rx_buffer[BUFFER_SIZE];
extern volatile uint32_t tx_buffer[BUFFER_SIZE];

typedef void (*audio_callback)(const float* audio_in, float* audio_out, int size);

void dma_set_audio_callback(audio_callback callback);

#endif