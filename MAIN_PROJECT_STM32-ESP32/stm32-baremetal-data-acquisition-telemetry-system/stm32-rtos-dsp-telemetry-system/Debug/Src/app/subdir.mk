################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/app/app_tasks.c 

OBJS += \
./Src/app/app_tasks.o 

C_DEPS += \
./Src/app/app_tasks.d 


# Each subdirectory must supply rules for building sources it contributes
Src/app/%.o Src/app/%.su Src/app/%.cyclo: ../Src/app/%.c Src/app/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -c -I../Inc -I"D:/EMBEDDED SYSTEM/EMBEDDED_SYSTEM-main/EMBEDDED_SYSTEM-main/STM32-M4/stm32-rtos-dsp-temp-pressure-altitude-monitoring-mqtt-gsm-wifi-telemetry-system/Inc/drivers" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src-2f-app

clean-Src-2f-app:
	-$(RM) ./Src/app/app_tasks.cyclo ./Src/app/app_tasks.d ./Src/app/app_tasks.o ./Src/app/app_tasks.su

.PHONY: clean-Src-2f-app

