################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/esp32-telemetry/spi_telemetry.c 

OBJS += \
./Src/esp32-telemetry/spi_telemetry.o 

C_DEPS += \
./Src/esp32-telemetry/spi_telemetry.d 


# Each subdirectory must supply rules for building sources it contributes
Src/esp32-telemetry/%.o Src/esp32-telemetry/%.su Src/esp32-telemetry/%.cyclo: ../Src/esp32-telemetry/%.c Src/esp32-telemetry/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -c -I../Inc -I"D:/EMBEDDED SYSTEM/EMBEDDED_SYSTEM-main/EMBEDDED_SYSTEM-main/STM32-M4/stm32-rtos-dsp-temp-pressure-altitude-monitoring-mqtt-gsm-wifi-telemetry-system/Inc/drivers" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src-2f-esp32-2d-telemetry

clean-Src-2f-esp32-2d-telemetry:
	-$(RM) ./Src/esp32-telemetry/spi_telemetry.cyclo ./Src/esp32-telemetry/spi_telemetry.d ./Src/esp32-telemetry/spi_telemetry.o ./Src/esp32-telemetry/spi_telemetry.su

.PHONY: clean-Src-2f-esp32-2d-telemetry

