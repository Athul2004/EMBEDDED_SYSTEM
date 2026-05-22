################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Energy_monitor/src/subdir.mk
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


BUILD_ARTIFACT_NAME := Energy_Data_Acquisition_With_Custom_Dsp
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Energy_Data_Acquisition_With_Custom_Dsp.elf \

MAP_FILES += \
Energy_Data_Acquisition_With_Custom_Dsp.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Energy_Data_Acquisition_With_Custom_Dsp.list \


# All Target
all: main-build

# Main-build Target
main-build: Energy_Data_Acquisition_With_Custom_Dsp.elf secondary-outputs

# Tool invocations
Energy_Data_Acquisition_With_Custom_Dsp.elf Energy_Data_Acquisition_With_Custom_Dsp.map: $(OBJS) $(USER_OBJS) C:\Users\KICHU\Desktop\EMBEDDED_SYSTEM_D\EMBEDDED_SYSTEM\STM32-M4\Energy_Data_Aquisition_With_Custom_Dsp\STM32F446RETX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Energy_Data_Acquisition_With_Custom_Dsp.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\KICHU\Desktop\EMBEDDED_SYSTEM_D\EMBEDDED_SYSTEM\STM32-M4\Energy_Data_Aquisition_With_Custom_Dsp\STM32F446RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Energy_Data_Acquisition_With_Custom_Dsp.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Energy_Data_Acquisition_With_Custom_Dsp.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Energy_Data_Acquisition_With_Custom_Dsp.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Energy_Data_Acquisition_With_Custom_Dsp.elf Energy_Data_Acquisition_With_Custom_Dsp.list Energy_Data_Acquisition_With_Custom_Dsp.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
