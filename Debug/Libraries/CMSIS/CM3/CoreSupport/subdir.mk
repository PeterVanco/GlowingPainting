################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/CMSIS/CM3/CoreSupport/core_cm3.c 

C_DEPS += \
./Libraries/CMSIS/CM3/CoreSupport/core_cm3.d 

OBJS += \
./Libraries/CMSIS/CM3/CoreSupport/core_cm3.o 


# Each subdirectory must supply rules for building sources it contributes
Libraries/CMSIS/CM3/CoreSupport/%.o: ../Libraries/CMSIS/CM3/CoreSupport/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD_VL -I"D:\CORTEX\F1_GlowingPainting\src" -I"D:\CORTEX\F1_GlowingPainting\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\CORTEX\F1_GlowingPainting\Libraries\CMSIS\CM3\CoreSupport" -I"D:\CORTEX\F1_GlowingPainting\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -O0 -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -g -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


