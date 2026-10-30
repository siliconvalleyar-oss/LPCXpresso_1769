TARGET = om13085_rgb
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

CFLAGS = -mcpu=cortex-m0 -mthumb -Wall -O0 -g -std=c99
LDFLAGS = -mcpu=cortex-m0 -mthumb -nostartfiles -Tld/lpc11u35.ld -Wl,--gc-sections -Wl,-Map=$(TARGET).map

SRCS = src/main.c src/startup.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET).elf $(TARGET).bin $(TARGET).hex size

$(TARGET).elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

size: $(TARGET).elf
	$(SIZE) $<

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin $(TARGET).hex $(TARGET).map

flash: $(TARGET).bin
	@echo "Usa: lpc21isp $(TARGET).bin /dev/ttyUSB0 115200 12000"
	@echo "o: openocd con tu interfaz"

.PHONY: all clean size flash
