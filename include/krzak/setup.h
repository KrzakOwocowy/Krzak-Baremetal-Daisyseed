#include "led.h"
#include "system_clock.h"
#include "audio_codec.h"
#include "dma.h"
#include "sdram.h"
#include "stm32h7xx.h"
#include "utils.h"
#include <stdint.h>

extern void(*const vector_table[])(void);

void system_setup(void);

void system_start(audio_callback callback);

