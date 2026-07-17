################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/gsm/gsm.c 

OBJS += \
./Src/gsm/gsm.o 

C_DEPS += \
./Src/gsm/gsm.d 


# Each subdirectory must supply rules for building sources it contributes
Src/gsm/%.o Src/gsm/%.su Src/gsm/%.cyclo: ../Src/gsm/%.c Src/gsm/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -c -I../Inc -I"D:/EMBEDDED SYSTEM/EMBEDDED_SYSTEM-main/EMBEDDED_SYSTEM-main/STM32-M4/stm32-rtos-dsp-temp-pressure-altitude-monitoring-mqtt-gsm-wifi-telemetry-system/Inc/drivers" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src-2f-gsm

clean-Src-2f-gsm:
	-$(RM) ./Src/gsm/gsm.cyclo ./Src/gsm/gsm.d ./Src/gsm/gsm.o ./Src/gsm/gsm.su

.PHONY: clean-Src-2f-gsm

