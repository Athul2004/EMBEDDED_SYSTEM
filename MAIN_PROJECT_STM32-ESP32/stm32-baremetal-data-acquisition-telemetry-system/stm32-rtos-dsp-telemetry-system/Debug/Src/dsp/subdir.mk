################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/dsp/dsp_engine.c \
../Src/dsp/filter_cascade.c \
../Src/dsp/filter_iir.c \
../Src/dsp/filter_kalman.c \
../Src/dsp/filter_median.c 

OBJS += \
./Src/dsp/dsp_engine.o \
./Src/dsp/filter_cascade.o \
./Src/dsp/filter_iir.o \
./Src/dsp/filter_kalman.o \
./Src/dsp/filter_median.o 

C_DEPS += \
./Src/dsp/dsp_engine.d \
./Src/dsp/filter_cascade.d \
./Src/dsp/filter_iir.d \
./Src/dsp/filter_kalman.d \
./Src/dsp/filter_median.d 


# Each subdirectory must supply rules for building sources it contributes
Src/dsp/%.o Src/dsp/%.su Src/dsp/%.cyclo: ../Src/dsp/%.c Src/dsp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -c -I../Inc -I"D:/EMBEDDED SYSTEM/EMBEDDED_SYSTEM-main/EMBEDDED_SYSTEM-main/STM32-M4/stm32-rtos-dsp-temp-pressure-altitude-monitoring-mqtt-gsm-wifi-telemetry-system/Inc/drivers" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src-2f-dsp

clean-Src-2f-dsp:
	-$(RM) ./Src/dsp/dsp_engine.cyclo ./Src/dsp/dsp_engine.d ./Src/dsp/dsp_engine.o ./Src/dsp/dsp_engine.su ./Src/dsp/filter_cascade.cyclo ./Src/dsp/filter_cascade.d ./Src/dsp/filter_cascade.o ./Src/dsp/filter_cascade.su ./Src/dsp/filter_iir.cyclo ./Src/dsp/filter_iir.d ./Src/dsp/filter_iir.o ./Src/dsp/filter_iir.su ./Src/dsp/filter_kalman.cyclo ./Src/dsp/filter_kalman.d ./Src/dsp/filter_kalman.o ./Src/dsp/filter_kalman.su ./Src/dsp/filter_median.cyclo ./Src/dsp/filter_median.d ./Src/dsp/filter_median.o ./Src/dsp/filter_median.su

.PHONY: clean-Src-2f-dsp

