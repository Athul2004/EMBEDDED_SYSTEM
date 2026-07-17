################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/sensors/bmp280.c 

OBJS += \
./Src/sensors/bmp280.o 

C_DEPS += \
./Src/sensors/bmp280.d 


# Each subdirectory must supply rules for building sources it contributes
Src/sensors/%.o Src/sensors/%.su Src/sensors/%.cyclo: ../Src/sensors/%.c Src/sensors/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -c -I../Inc -I"D:/EMBEDDED SYSTEM/EMBEDDED_SYSTEM-main/EMBEDDED_SYSTEM-main/STM32-M4/stm32-rtos-dsp-temp-pressure-altitude-monitoring-mqtt-gsm-wifi-telemetry-system/Inc/drivers" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src-2f-sensors

clean-Src-2f-sensors:
	-$(RM) ./Src/sensors/bmp280.cyclo ./Src/sensors/bmp280.d ./Src/sensors/bmp280.o ./Src/sensors/bmp280.su

.PHONY: clean-Src-2f-sensors

