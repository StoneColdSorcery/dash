################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32F401_DISCOVERY/stm32f401_discovery.c \
../STM32F401_DISCOVERY/stm32f401_discovery_audio_codec.c \
../STM32F401_DISCOVERY/stm32f401_discovery_l3gd20.c \
../STM32F401_DISCOVERY/stm32f401_discovery_lsm303dlhc.c 

OBJS += \
./STM32F401_DISCOVERY/stm32f401_discovery.o \
./STM32F401_DISCOVERY/stm32f401_discovery_audio_codec.o \
./STM32F401_DISCOVERY/stm32f401_discovery_l3gd20.o \
./STM32F401_DISCOVERY/stm32f401_discovery_lsm303dlhc.o 

C_DEPS += \
./STM32F401_DISCOVERY/stm32f401_discovery.d \
./STM32F401_DISCOVERY/stm32f401_discovery_audio_codec.d \
./STM32F401_DISCOVERY/stm32f401_discovery_l3gd20.d \
./STM32F401_DISCOVERY/stm32f401_discovery_lsm303dlhc.d 


# Each subdirectory must supply rules for building sources it contributes
STM32F401_DISCOVERY/%.o: ../STM32F401_DISCOVERY/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Utilities\STM32F401-Discovery" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Libraries\STM32F4xx_StdPeriph_Driver\inc" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Libraries\STM32_USB_Device_Library\Core\inc" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Libraries\STM32_USB_OTG_Driver\inc" -I"C:/Users/Crazyfingers/ARM Workspaces/STM32F4disc/Template_Project/inc" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Libraries\CMSIS\Device\ST\STM32F4xx\Include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


