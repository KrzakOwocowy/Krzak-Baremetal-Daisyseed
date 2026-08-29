#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

//write value on bit of target_register
static inline void set_bit(volatile uint32_t *target_register, uint8_t bit, uint8_t value){
    switch (value){
        case 0:
            *target_register &= ~(1U << bit);
            break;
        case 1:
            *target_register |= (1U << bit);
            break;
    }
};

#endif