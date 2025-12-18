TOOLCHAIN_PATH = "/cygdrive/c/Program Files (x86)/GNU Tools ARM Embedded/6 2017-q2-update/bin/"
CROSS_COMPILE =arm-none-eabi-
STM32_FLASH = flash/stm32flash/

CC		= $(TOOLCHAIN_PATH)$(CROSS_COMPILE)gcc
LD		= $(TOOLCHAIN_PATH)$(CROSS_COMPILE)ld
AR		= $(TOOLCHAIN_PATH)$(CROSS_COMPILE)ar
AS		= $(TOOLCHAIN_PATH)$(CROSS_COMPILE)as
OC		= $(TOOLCHAIN_PATH)$(CROSS_COMPILE)objcopy
OD		= $(TOOLCHAIN_PATH)$(CROSS_COMPILE)objdump
SZ		= $(TOOLCHAIN_PATH)$(CROSS_COMPILE)size

CFLAGS	= 	-c -fno-common \
			-ffunction-sections \
			-fdata-sections \
			-O0 \
			-g3 \
			-mcpu=cortex-m3 \
			-mthumb \
			-Wall

LDSCRIPT= ld/stm32_rom.ld

LDFLAGS	=	--gc-sections,-T$(LDSCRIPT),-no-startup,-nostdlib,-lgcc,-lm,-lc,

OCFLAGS	=	-Obinary

ODFLAGS	=	-S

OUTPUT_DIR = output

TARGET  = 	$(OUTPUT_DIR)/main

INCLUDE =   -I./src -I./src/stdlib -I./src/usblib

SRCS	=	src/stdlib/startup_stm32f10x_md.c \
			src/stdlib/system_stm32f10x.c	\
      		src/stdlib/stm32f10x_gpio.c 	\
			src/stdlib/stm32f10x_rcc.c 		\
			src/stdlib/stm32f10x_can.c 		\
			src/stdlib/stm32f10x_usart.c	\
			src/stdlib/stm32f10x_tim.c		\
			src/stdlib/stm32f10x_adc.c		\
			src/stdlib/misc.c 				\
			src/stm32_olimexino.c			\
			src/core_cm3.c 			\
			src/syscalls.c			\
			src/hw_config.c			\
			src/stm32_it.c			\
			src/crc16-ccitt.c		\
			src/CO_driver.c     	\
			src/CO_SDO.c           	\
			src/CO_Emergency.c     	\
			src/CO_NMT_Heartbeat.c 	\
			src/CO_SYNC.c          	\
			src/CO_PDO.c           	\
			src/CO_HBconsumer.c    	\
			src/CO_SDOmaster.c     	\
			src/CANopen.c        	\
			src/CO_OD.c             \
			src/application.c		\
			src/main.c


OBJS=$(SRCS:.c=.o)


.PHONY : clean all help

help: ## This help message
	@echo -e "$$(grep -hE '^\S+:.*##' $(MAKEFILE_LIST) | sed -e 's/:.*##\s*/:/' -e 's/^\(.\+\):\(.*\)/\\x1b[36m\1\\x1b[m:\2/' | column -c2 -t -s :)"

all: $(TARGET).bin  $(TARGET).list  ## Build CANOpenNode project.
	@echo "  SIZE $(TARGET).elf"
	$(SZ) $(TARGET).elf

install:														## Flash CANopenNode to target. Use COM parameter to specify com port, e.g. COM=COM8
	$(STM32_FLASH)stm32flash -w output\main.bin $(COM)

clean:															## Tidy up output directory
	@echo "Removing files..."
	rm -f $(OBJS)
	rm -f $(OUTPUT_DIR)\main.bin $(OUTPUT_DIR)\main.elf $(OUTPUT_DIR)\main.map $(OUTPUT_DIR)\main.lst


$(TARGET).list: $(TARGET).elf
	@echo "  OBJDUMP $(TARGET).list"
	$(OD) $(ODFLAGS) $< > $(TARGET).lst



$(TARGET).bin: $(TARGET).elf
	@echo "  OBJCOPY $(TARGET).bin"
	$(OC) $(OCFLAGS) $(TARGET).elf $(TARGET).bin



$(TARGET).elf: $(OBJS)
	@echo "  LD $(TARGET).elf"
	$(CC) -mcpu=cortex-m3 -mthumb -Wl,$(LDFLAGS),-o$(TARGET).elf,-Map,$(TARGET).map $(OBJS)


%.o: %.c
	@echo "  CC $<"
	$(CC) $(INCLUDE) $(CFLAGS)  $< -o $*.o

%.o: %.S
	@echo "  CC $<"
	$(CC) $(INCLUDE) $(CFLAGS)  $< -o $*.o
