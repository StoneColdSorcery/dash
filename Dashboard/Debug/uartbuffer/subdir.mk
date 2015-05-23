################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../uartbuffer/uartbuffer.c 

OBJS += \
./uartbuffer/uartbuffer.o 

C_DEPS += \
./uartbuffer/uartbuffer.d 


# Each subdirectory must supply rules for building sources it contributes
uartbuffer/%.o: ../uartbuffer/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DUSB_OTG_FS_CORE -DUSE_STM32F401 -DSTM32F401xx -DUSE_USB_OTG_FS -I"C:/Users/Crazyfingers/ARM Workspaces/STM32F4disc/Dashboard/inc" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Libraries\CMSIS\Device\ST\STM32F4xx\Include" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Utilities\STM32F401-Discovery" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Libraries\CMSIS\Include" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Libraries\STM32F4xx_StdPeriph_Driver\inc" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Libraries\STM32_USB_Device_Library\Core\inc" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Libraries\STM32_USB_OTG_Driver\inc" -O0 -g3 -Wall -c -fmessage-length=0 -mthumb -mcpu=cortex-m4 -mfloat-abi=soft -mfpu=fpv4-sp-d16 -fdata-sections -ffunction-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


