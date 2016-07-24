################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/TrueSTUDIO/startup_stm32f10x_md_vl.S 

OBJS += \
./Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/TrueSTUDIO/startup_stm32f10x_md_vl.o 

S_UPPER_DEPS += \
./Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/TrueSTUDIO/startup_stm32f10x_md_vl.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/TrueSTUDIO/%.o: ../Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/TrueSTUDIO/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC Assembler'
	arm-none-eabi-gcc -c -x assembler-with-cpp -I"D:\CORTEX\F1_GlowingPainting\src" -I"D:\CORTEX\F1_GlowingPainting\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\CORTEX\F1_GlowingPainting\Libraries\CMSIS\CM3\CoreSupport" -I"D:\CORTEX\F1_GlowingPainting\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -g -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


