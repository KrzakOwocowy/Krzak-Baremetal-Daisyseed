#ifndef LED_H
#define LED_H

//setup onboard led
void led_setup(void);

typedef enum led_state{
    on,
    off
} led_state;

//set the onboard led on or off
void led(led_state led_state);

#endif