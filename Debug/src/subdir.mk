################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/main.cc 

C_SRCS += \
../src/stm32f10x_it.c \
../src/syscalls.c \
../src/system_stm32f10x.c 

C_DEPS += \
./src/stm32f10x_it.d \
./src/syscalls.d \
./src/system_stm32f10x.d 

CC_DEPS += \
./src/main.d 

OBJS += \
./src/main.o \
./src/stm32f10x_it.o \
./src/syscalls.o \
./src/system_stm32f10x.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C++ Compiler'
	arm-none-eabi-g++ -DSTM32F10X_MD_VL -DUSE_STDPERIPH_DRIVER -I"D:\CORTEX\F1_GlowingPainting\src" -I"D:\CORTEX\F1_GlowingPainting\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\CORTEX\F1_GlowingPainting\Libraries\CMSIS\CM3\CoreSupport" -I"D:\CORTEX\F1_GlowingPainting\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -O0 -fdata-sections -Wall -Wa,-adhlns="$@.lst" -fno-exceptions -fno-rtti -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -g -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD_VL -I"D:\CORTEX\F1_GlowingPainting\src" -I"D:\CORTEX\F1_GlowingPainting\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\CORTEX\F1_GlowingPainting\Libraries\CMSIS\CM3\CoreSupport" -I"D:\CORTEX\F1_GlowingPainting\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -O0 -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -g -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


