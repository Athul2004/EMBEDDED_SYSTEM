################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Energy_monitor/src/adc_dma_driver.c \
../Energy_monitor/src/energy_meter.c \
../Energy_monitor/src/fonts.c \
../Energy_monitor/src/i2c_driver.c \
../Energy_monitor/src/main.c \
../Energy_monitor/src/ssd1306.c \
../Energy_monitor/src/syscalls.c \
../Energy_monitor/src/sysmem.c \
../Energy_monitor/src/timer_driver.c \
../Energy_monitor/src/uart_driver.c 

OBJS += \
./Energy_monitor/src/adc_dma_driver.o \
./Energy_monitor/src/energy_meter.o \
./Energy_monitor/src/fonts.o \
./Energy_monitor/src/i2c_driver.o \
./Energy_monitor/src/main.o \
./Energy_monitor/src/ssd1306.o \
./Energy_monitor/src/syscalls.o \
./Energy_monitor/src/sysmem.o \
./Energy_monitor/src/timer_driver.o \
./Energy_monitor/src/uart_driver.o 

C_DEPS += \
./Energy_monitor/src/adc_dma_driver.d \
./Energy_monitor/src/energy_meter.d \
./Energy_monitor/src/fonts.d \
./Energy_monitor/src/i2c_driver.d \
./Energy_monitor/src/main.d \
./Energy_monitor/src/ssd1306.d \
./Energy_monitor/src/syscalls.d \
./Energy_monitor/src/sysmem.d \
./Energy_monitor/src/timer_driver.d \
./Energy_monitor/src/uart_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Energy_monitor/src/%.o Energy_monitor/src/%.su Energy_monitor/src/%.cyclo: ../Energy_monitor/src/%.c Energy_monitor/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -c -I../Inc -I"C:/Users/KICHU/Desktop/EMBEDDED_SYSTEM_D/EMBEDDED_SYSTEM/STM32-M4/Energy_Data_Aquisition_With_Custom_Dsp/Energy_monitor/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Energy_monitor-2f-src

clean-Energy_monitor-2f-src:
	-$(RM) ./Energy_monitor/src/adc_dma_driver.cyclo ./Energy_monitor/src/adc_dma_driver.d ./Energy_monitor/src/adc_dma_driver.o ./Energy_monitor/src/adc_dma_driver.su ./Energy_monitor/src/energy_meter.cyclo ./Energy_monitor/src/energy_meter.d ./Energy_monitor/src/energy_meter.o ./Energy_monitor/src/energy_meter.su ./Energy_monitor/src/fonts.cyclo ./Energy_monitor/src/fonts.d ./Energy_monitor/src/fonts.o ./Energy_monitor/src/fonts.su ./Energy_monitor/src/i2c_driver.cyclo ./Energy_monitor/src/i2c_driver.d ./Energy_monitor/src/i2c_driver.o ./Energy_monitor/src/i2c_driver.su ./Energy_monitor/src/main.cyclo ./Energy_monitor/src/main.d ./Energy_monitor/src/main.o ./Energy_monitor/src/main.su ./Energy_monitor/src/ssd1306.cyclo ./Energy_monitor/src/ssd1306.d ./Energy_monitor/src/ssd1306.o ./Energy_monitor/src/ssd1306.su ./Energy_monitor/src/syscalls.cyclo ./Energy_monitor/src/syscalls.d ./Energy_monitor/src/syscalls.o ./Energy_monitor/src/syscalls.su ./Energy_monitor/src/sysmem.cyclo ./Energy_monitor/src/sysmem.d ./Energy_monitor/src/sysmem.o ./Energy_monitor/src/sysmem.su ./Energy_monitor/src/timer_driver.cyclo ./Energy_monitor/src/timer_driver.d ./Energy_monitor/src/timer_driver.o ./Energy_monitor/src/timer_driver.su ./Energy_monitor/src/uart_driver.cyclo ./Energy_monitor/src/uart_driver.d ./Energy_monitor/src/uart_driver.o ./Energy_monitor/src/uart_driver.su

.PHONY: clean-Energy_monitor-2f-src

