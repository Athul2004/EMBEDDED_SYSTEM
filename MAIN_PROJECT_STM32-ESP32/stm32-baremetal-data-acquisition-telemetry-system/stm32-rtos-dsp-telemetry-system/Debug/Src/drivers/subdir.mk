################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/drivers/delay.c \
../Src/drivers/stm32f446xx_gpio_driver.c \
../Src/drivers/stm32f446xx_i2c_driver.c \
../Src/drivers/stm32f446xx_rcc_driver.c \
../Src/drivers/stm32f446xx_spi_driver.c \
../Src/drivers/stm32f446xx_usart_driver.c 

OBJS += \
./Src/drivers/delay.o \
./Src/drivers/stm32f446xx_gpio_driver.o \
./Src/drivers/stm32f446xx_i2c_driver.o \
./Src/drivers/stm32f446xx_rcc_driver.o \
./Src/drivers/stm32f446xx_spi_driver.o \
./Src/drivers/stm32f446xx_usart_driver.o 

C_DEPS += \
./Src/drivers/delay.d \
./Src/drivers/stm32f446xx_gpio_driver.d \
./Src/drivers/stm32f446xx_i2c_driver.d \
./Src/drivers/stm32f446xx_rcc_driver.d \
./Src/drivers/stm32f446xx_spi_driver.d \
./Src/drivers/stm32f446xx_usart_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Src/drivers/%.o Src/drivers/%.su Src/drivers/%.cyclo: ../Src/drivers/%.c Src/drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -c -I../Inc -I"D:/EMBEDDED SYSTEM/EMBEDDED_SYSTEM-main/EMBEDDED_SYSTEM-main/STM32-M4/stm32-rtos-dsp-temp-pressure-altitude-monitoring-mqtt-gsm-wifi-telemetry-system/Inc/drivers" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src-2f-drivers

clean-Src-2f-drivers:
	-$(RM) ./Src/drivers/delay.cyclo ./Src/drivers/delay.d ./Src/drivers/delay.o ./Src/drivers/delay.su ./Src/drivers/stm32f446xx_gpio_driver.cyclo ./Src/drivers/stm32f446xx_gpio_driver.d ./Src/drivers/stm32f446xx_gpio_driver.o ./Src/drivers/stm32f446xx_gpio_driver.su ./Src/drivers/stm32f446xx_i2c_driver.cyclo ./Src/drivers/stm32f446xx_i2c_driver.d ./Src/drivers/stm32f446xx_i2c_driver.o ./Src/drivers/stm32f446xx_i2c_driver.su ./Src/drivers/stm32f446xx_rcc_driver.cyclo ./Src/drivers/stm32f446xx_rcc_driver.d ./Src/drivers/stm32f446xx_rcc_driver.o ./Src/drivers/stm32f446xx_rcc_driver.su ./Src/drivers/stm32f446xx_spi_driver.cyclo ./Src/drivers/stm32f446xx_spi_driver.d ./Src/drivers/stm32f446xx_spi_driver.o ./Src/drivers/stm32f446xx_spi_driver.su ./Src/drivers/stm32f446xx_usart_driver.cyclo ./Src/drivers/stm32f446xx_usart_driver.d ./Src/drivers/stm32f446xx_usart_driver.o ./Src/drivers/stm32f446xx_usart_driver.su

.PHONY: clean-Src-2f-drivers

