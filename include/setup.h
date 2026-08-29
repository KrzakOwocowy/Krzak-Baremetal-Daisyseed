#include "led.h"
#include "system_clock.h"
#include "audio_codec.h"
#include "dma.h"
#include "fpu.h"
#include "stm32h7xx.h"
#include "utils.h"
#include <stdint.h>

extern void(*const vector_table[])(void);

void system_setup(audio_callback callback);

