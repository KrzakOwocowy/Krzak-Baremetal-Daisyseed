Uses a Electrosmith Daisyseed rev7 which runs on an STM32H750IBK6 mcu and a PCM3060 audio codec

Before building this project, ensure you have the following tools installed and added to your system `PATH`:

- ARM GNU Toolchain: `arm-none-eabi-gcc`
- Build System: `make` (GNU Make)
- Flashing Utility: `dfu-util`

Compile the project with `make` and flash it with `make flash` (make sure to put the board in DFU mode first)

You can place any DSP math inside the `for` loop in the `process_audio` function in main.c (currently a passthrough for demonstration purposes)

`system_setup` functionalities implemented:
-  basic control over the onboard LED (`led.c`)
-  setting up a 400MHz PLL clock for the system core and a second PLL for the audio codec (`system_clock.c`)
-  enabling communication with the audio codec using the Serial Audio Interface (`audio_codec.c`)
-  setting up DMA ping-pong buffers to recieve and transmit audio data (`dma.c`)
-  utilising the FPU (`fpu.c`)
