################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/adc_dma_driver.c \
../Src/energy_meter.c \
../Src/fonts.c \
../Src/i2c_driver.c \
../Src/main.c \
../Src/ssd1306.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/timer_driver.c \
../Src/uart_driver.c 

OBJS += \
./Src/adc_dma_driver.o \
./Src/energy_meter.o \
./Src/fonts.o \
./Src/i2c_driver.o \
./Src/main.o \
./Src/ssd1306.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/timer_driver.o \
./Src/uart_driver.o 

C_DEPS += \
./Src/adc_dma_driver.d \
./Src/energy_meter.d \
./Src/fonts.d \
./Src/i2c_driver.d \
./Src/main.d \
./Src/ssd1306.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/timer_driver.d \
./Src/uart_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -c -I../Inc -I"C:/Users/KICHU/Desktop/EMBEDDED_SYSTEM_D/EMBEDDED_SYSTEM/STM32-M4/Energy_Data_Aquisition_With_Custom_Dsp/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/adc_dma_driver.cyclo ./Src/adc_dma_driver.d ./Src/adc_dma_driver.o ./Src/adc_dma_driver.su ./Src/energy_meter.cyclo ./Src/energy_meter.d ./Src/energy_meter.o ./Src/energy_meter.su ./Src/fonts.cyclo ./Src/fonts.d ./Src/fonts.o ./Src/fonts.su ./Src/i2c_driver.cyclo ./Src/i2c_driver.d ./Src/i2c_driver.o ./Src/i2c_driver.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/ssd1306.cyclo ./Src/ssd1306.d ./Src/ssd1306.o ./Src/ssd1306.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/timer_driver.cyclo ./Src/timer_driver.d ./Src/timer_driver.o ./Src/timer_driver.su ./Src/uart_driver.cyclo ./Src/uart_driver.d ./Src/uart_driver.o ./Src/uart_driver.su

.PHONY: clean-Src

