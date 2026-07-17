################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Src/rtos/context_switch.s 

C_SRCS += \
../Src/rtos/event.c \
../Src/rtos/heap.c \
../Src/rtos/idle.c \
../Src/rtos/kernel.c \
../Src/rtos/mutex.c \
../Src/rtos/queue.c \
../Src/rtos/scheduler.c \
../Src/rtos/semaphore.c \
../Src/rtos/syscall.c \
../Src/rtos/task.c \
../Src/rtos/timer.c 

OBJS += \
./Src/rtos/context_switch.o \
./Src/rtos/event.o \
./Src/rtos/heap.o \
./Src/rtos/idle.o \
./Src/rtos/kernel.o \
./Src/rtos/mutex.o \
./Src/rtos/queue.o \
./Src/rtos/scheduler.o \
./Src/rtos/semaphore.o \
./Src/rtos/syscall.o \
./Src/rtos/task.o \
./Src/rtos/timer.o 

S_DEPS += \
./Src/rtos/context_switch.d 

C_DEPS += \
./Src/rtos/event.d \
./Src/rtos/heap.d \
./Src/rtos/idle.d \
./Src/rtos/kernel.d \
./Src/rtos/mutex.d \
./Src/rtos/queue.d \
./Src/rtos/scheduler.d \
./Src/rtos/semaphore.d \
./Src/rtos/syscall.d \
./Src/rtos/task.d \
./Src/rtos/timer.d 


# Each subdirectory must supply rules for building sources it contributes
Src/rtos/%.o: ../Src/rtos/%.s Src/rtos/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Src/rtos/%.o Src/rtos/%.su Src/rtos/%.cyclo: ../Src/rtos/%.c Src/rtos/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -c -I../Inc -I"D:/EMBEDDED SYSTEM/EMBEDDED_SYSTEM-main/EMBEDDED_SYSTEM-main/STM32-M4/stm32-rtos-dsp-temp-pressure-altitude-monitoring-mqtt-gsm-wifi-telemetry-system/Inc/drivers" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src-2f-rtos

clean-Src-2f-rtos:
	-$(RM) ./Src/rtos/context_switch.d ./Src/rtos/context_switch.o ./Src/rtos/event.cyclo ./Src/rtos/event.d ./Src/rtos/event.o ./Src/rtos/event.su ./Src/rtos/heap.cyclo ./Src/rtos/heap.d ./Src/rtos/heap.o ./Src/rtos/heap.su ./Src/rtos/idle.cyclo ./Src/rtos/idle.d ./Src/rtos/idle.o ./Src/rtos/idle.su ./Src/rtos/kernel.cyclo ./Src/rtos/kernel.d ./Src/rtos/kernel.o ./Src/rtos/kernel.su ./Src/rtos/mutex.cyclo ./Src/rtos/mutex.d ./Src/rtos/mutex.o ./Src/rtos/mutex.su ./Src/rtos/queue.cyclo ./Src/rtos/queue.d ./Src/rtos/queue.o ./Src/rtos/queue.su ./Src/rtos/scheduler.cyclo ./Src/rtos/scheduler.d ./Src/rtos/scheduler.o ./Src/rtos/scheduler.su ./Src/rtos/semaphore.cyclo ./Src/rtos/semaphore.d ./Src/rtos/semaphore.o ./Src/rtos/semaphore.su ./Src/rtos/syscall.cyclo ./Src/rtos/syscall.d ./Src/rtos/syscall.o ./Src/rtos/syscall.su ./Src/rtos/task.cyclo ./Src/rtos/task.d ./Src/rtos/task.o ./Src/rtos/task.su ./Src/rtos/timer.cyclo ./Src/rtos/timer.d ./Src/rtos/timer.o ./Src/rtos/timer.su

.PHONY: clean-Src-2f-rtos

