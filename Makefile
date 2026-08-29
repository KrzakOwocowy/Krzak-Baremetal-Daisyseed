CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

TARGET = build/firmware
SRC = src/main.c src/startup_stm32h750.c src/led.c  src/system_clock.c src/audio_codec.c src/dma.c src/setup.c src/fpu.c
INC = -Iinclude

CPU = -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16

CFLAGS = $(CPU) $(INC) -O2 -Wall -Wextra -g3 -DSTM32H750xx
LDFLAGS = $(CPU) -T stm32h750.ld -Wl,-gc-sections --specs=nosys.specs

OBJ = $(SRC:src/%.c=build/%.o)

all: $(TARGET).bin
	$(SIZE) $(TARGET).elf

$(TARGET).elf: $(OBJ)
	@if not exist build mkdir build
	$(CC) $(OBJ) $(LDFLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

build/%.o: src/%.c
	@if not exist build mkdir build
	$(CC) $(CFLAGS) -c $< -o $@

flash: $(TARGET).bin
	dfu-util -a 0 -s 0x08000000:leave -D $(TARGET).bin

clean:
	@if exist build rmdir /s /q build

.PHONY: all flash clean

