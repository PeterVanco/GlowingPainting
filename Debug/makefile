################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := cs-rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include Libraries/STM32F10x_StdPeriph_Driver/src/subdir.mk
-include Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/TrueSTUDIO/subdir.mk
-include Libraries/CMSIS/CM3/CoreSupport/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
F1_GlowingPainting.hex \

SECONDARY_LIST += \
F1_GlowingPainting.lst \

SECONDARY_SIZE += \
F1_GlowingPainting.siz \


# All Target
all: F1_GlowingPainting.elf secondary-outputs

# Tool invocations
F1_GlowingPainting.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Sourcery Windows GCC C++ Linker'
	arm-none-eabi-g++ -T"..\stm32_flash.ld" -Xlinker --gc-sections -Wl,-Map,F1_GlowingPainting.map -Xlinker --cref -Wl,-cref,-u,Reset_Handler "-Wl,-Map=F1_GlowingPainting.map" -Wl,--gc-sections -Wl,--defsym=malloc_getpagesize_P=0x1000 -mcpu=cortex-m3 -mthumb -g -gdwarf-2 -o "F1_GlowingPainting.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

F1_GlowingPainting.hex: F1_GlowingPainting.elf
	@echo 'Invoking: ARM Sourcery Windows GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex F1_GlowingPainting.elf  "F1_GlowingPainting.hex"
	@echo 'Finished building: $@'
	@echo ' '

F1_GlowingPainting.lst: F1_GlowingPainting.elf
	@echo 'Invoking: ARM Sourcery Windows GNU Create Listing'
	arm-none-eabi-objdump -h -S F1_GlowingPainting.elf > "F1_GlowingPainting.lst"
	@echo 'Finished building: $@'
	@echo ' '

F1_GlowingPainting.siz: F1_GlowingPainting.elf
	@echo 'Invoking: ARM Sourcery Windows GNU Print Size'
	arm-none-eabi-size  --format=berkeley F1_GlowingPainting.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C_UPPER_DEPS)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(C_DEPS)$(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(OBJS)$(CXX_DEPS)$(SECONDARY_FLASH)$(ASM_DEPS)$(S_UPPER_DEPS)$(CPP_DEPS) F1_GlowingPainting.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
