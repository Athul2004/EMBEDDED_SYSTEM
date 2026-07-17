################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/watchdog/subdir.mk
-include Src/sensors/subdir.mk
-include Src/rtos/subdir.mk
-include Src/gsm/subdir.mk
-include Src/esp32-telemetry/subdir.mk
-include Src/dsp/subdir.mk
-include Src/drivers/subdir.mk
-include Src/app/subdir.mk
-include Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := stm32-rtos-dsp-temp-pressure-altitude-monitoring-mqtt-gsm-wifi-telemetry-system
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
stm32-rtos-dsp-temp-pressure-altitude-monitoring-mqtt-gsm-wifi-telemetry-system.elf \

MAP_FILES += \
stm32-rtos-dsp-temp-pressure-altitude-monitoring-mqtt-gsm-wifi-telemetry-system.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
stm32-rtos-dsp-temp-pressure-altitude-monitoring-mqtt-gsm-wifi-telemetry-system.list \


# All Target
all: main-build

# Main-build Target
main-build: stm32-rtos-dsp-temp-pressure-altitude-monitoring-mqtt-gsm-wifi-telemetry-system.elf secondary-outputs

# Tool invocations
stm32-rtos-dsp-temp-pressure-altitude-monitoring-mqtt-gsm-wifi-telemetry-system.elf stm32-rtos-dsp-temp-pressure-altitude-monitoring-mqtt-gsm-wifi-telemetry-system.map: $(OBJS) $(USER_OBJS) D:\EMBEDDED\ SYSTEM\EMBEDDED_SYSTEM-main\EMBEDDED_SYSTEM-main\STM32-M4\stm32-rtos-dsp-temp-pressure-altitude-monitoring-mqtt-gsm-wifi-telemetry-system\STM32F446RETX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "stm32-rtos-dsp-temp-pressure-altitude-monitoring-mqtt-gsm-wifi-telemetry-system.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"D:\EMBEDDED SYSTEM\EMBEDDED_SYSTEM-main\EMBEDDED_SYSTEM-main\STM32-M4\stm32-rtos-dsp-temp-pressure-altitude-monitoring-mqtt-gsm-wifi-telemetry-system\STM32F446RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="stm32-rtos-dsp-temp-pressure-altitude-monitoring-mqtt-gsm-wifi-telemetry-system.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

stm32-rtos-dsp-temp-pressure-altitude-monitoring-mqtt-gsm-wifi-telemetry-system.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "stm32-rtos-dsp-temp-pressure-altitude-monitoring-mqtt-gsm-wifi-telemetry-system.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) default.size.stdout stm32-rtos-dsp-temp-pressure-altitude-monitoring-mqtt-gsm-wifi-telemetry-system.elf stm32-rtos-dsp-temp-pressure-altitude-monitoring-mqtt-gsm-wifi-telemetry-system.list stm32-rtos-dsp-temp-pressure-altitude-monitoring-mqtt-gsm-wifi-telemetry-system.map
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
