# generated makefile with help of https://github.com/duro80/Makefile4CubeMX?fbclid=IwAR3Dsr90cgwaRnMR4vw0n6t4CD-8AUt3FfFGtnMh8wwuV-Wna3lrD9pR-x8

TARGET = jezap_sikor_flac
DEBUG = 1
OPT = -Og
BUILD_DIR = build

C_SOURCES =  \
Src/main.c \
Src/fatfs.c \
Src/fatfs_platform.c \
Src/usbh_diskio.c \
Src/freertos.c \
Src/syscalls.c \
Src/usb_host.c \
Src/usbh_conf.c \
Src/usbh_platform.c \
Src/stm32f7xx_it.c \
Src/stm32f7xx_hal_msp.c \
Src/stm32f7xx_hal_timebase_TIM.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_eth.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_hcd.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_usb.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc_ex.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_crc.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_crc_ex.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dcmi.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dcmi_ex.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma2d.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_fmc.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sdram.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_ltdc.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_ltdc_ex.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dsi.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_qspi.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rtc.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rtc_ex.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sai.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sai_ex.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_sdmmc.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sd.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spdifrx.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash_ex.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma_ex.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c \
Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c \
Src/system_stm32f7xx.c \
Middlewares/Third_Party/FatFs/src/option/syscall.c \
Middlewares/Third_Party/FatFs/src/option/ccsbcs.c \
Middlewares/Third_Party/FatFs/src/ff.c \
Middlewares/Third_Party/FatFs/src/ff_gen_drv.c \
Middlewares/Third_Party/FatFs/src/diskio.c \
Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c \
Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.c \
Middlewares/Third_Party/FreeRTOS/Source/queue.c \
Middlewares/Third_Party/FreeRTOS/Source/croutine.c \
Middlewares/Third_Party/FreeRTOS/Source/timers.c \
Middlewares/Third_Party/FreeRTOS/Source/list.c \
Middlewares/Third_Party/FreeRTOS/Source/tasks.c \
Middlewares/Third_Party/FreeRTOS/Source/event_groups.c \
Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c \
Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.c \
Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.c \
Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.c \
Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.c \
Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.c \
Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.c \
Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.c


PROJECT_SRC = \
Src/dbgu.c \
Src/term_io.c \
Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_audio.c \
Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_lcd.c \
Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_sdram.c \
Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_ts.c \
Drivers/BSP/STM32746G-Discovery/stm32746g_discovery.c \
Drivers/BSP/Components/wm8994/wm8994.c \
Drivers/BSP/Components/ft5336/ft5336.c \
LIBFLAC/src/libFLAC/stream_decoder.c \
LIBFLAC/src/libFLAC/bitreader.c \
LIBFLAC/src/libFLAC/cpu.c \
LIBFLAC/src/libFLAC/format.c \
LIBFLAC/src/libFLAC/bitwriter.c \
LIBFLAC/src/libFLAC/crc.c \
LIBFLAC/src/libFLAC/lpc.c \
LIBFLAC/src/libFLAC/md5.c \
LIBFLAC/src/libFLAC/memory.c \
LIBFLAC/src/libFLAC/fixed.c \
core/src/stm32f476g-disco-LJMS-flac_input_stream.c \
core/src/stm32f476g-disco-LJMS-flac.c \
core/src/stm32f476g-disco-LJMS-flac_frame_buffer.c \
core/src/stm32f476g-disco-LJMS-flac_screen.c \
core/src/stm32f476g-disco-LJMS-flac_search_files.c \
core/src/stm32f476g-disco-LJMS-flac_player.c \
core/src/stm32f476g-disco-LJMS-flac_main.c

C_SOURCES += $(PROJECT_SRC)

ASM_SOURCES =  \
startup_stm32f746xx.s

PREFIX = arm-none-eabi-
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
CPU = -mcpu=cortex-m7

FPU = -mfpu=fpv5-sp-d16

FLOAT-ABI = -mfloat-abi=hard

MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

AS_DEFS = 

C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F746xx \
-DHAVE_CONFIG_H

AS_INCLUDES =  \
-I/Inc

C_INCLUDES =  \
-IInc \
-IDrivers/STM32F7xx_HAL_Driver/Inc \
-IDrivers/BSP/STM32746G-Discovery \
-IDrivers/BSP/Components \
-I. \
-IDrivers/STM32F7xx_HAL_Driver/Inc/Legacy \
-IMiddlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 \
-IMiddlewares/ST/STM32_USB_Host_Library/Core/Inc \
-IMiddlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc \
-IDrivers/CMSIS/Device/ST/STM32F7xx/Include \
-IMiddlewares/Third_Party/FatFs/src \
-IMiddlewares/Third_Party/FreeRTOS/Source/include \
-IMiddlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS \
-IDrivers/CMSIS/Include \
-Ilibflac \
-ILIBFLAC/include \
-ILIBFLAC/src/libFLAC/include

ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

LDSCRIPT = STM32F746NGHx_FLASH.ld
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

compile: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

clean:
	-del /S /Q $(BUILD_DIR)

flash:
	$(OPENOCD_PATH)/openocd -f "board/stm32f7discovery.cfg" -c "program $(BUILD_DIR)/$(TARGET).elf verify reset exit"

all:
	make compile
	make flash

-include $(wildcard $(BUILD_DIR)/*.d)
