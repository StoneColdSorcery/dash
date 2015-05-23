################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/chardisplay.c \
../src/main_newbuffer7seg.c \
../src/sevenseg.c \
../src/steppermotor.c \
../src/stm32f4xx_it.c \
../src/system_stm32f4xx.c 

OBJS += \
./src/chardisplay.o \
./src/main_newbuffer7seg.o \
./src/sevenseg.o \
./src/steppermotor.o \
./src/stm32f4xx_it.o \
./src/system_stm32f4xx.o 

C_DEPS += \
./src/chardisplay.d \
./src/main_newbuffer7seg.d \
./src/sevenseg.d \
./src/steppermotor.d \
./src/stm32f4xx_it.d \
./src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DUSB_OTG_FS_CORE -DUSE_STM32F401 -DSTM32F401xx -DUSE_USB_OTG_FS -I"C:/Users/Crazyfingers/ARM Workspaces/STM32F4disc/Dashboard/inc" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Libraries\CMSIS\Device\ST\STM32F4xx\Include" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Utilities\STM32F401-Discovery" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Libraries\CMSIS\Include" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Libraries\STM32F4xx_StdPeriph_Driver\inc" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Libraries\STM32_USB_Device_Library\Core\inc" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Libraries\STM32_USB_OTG_Driver\inc" -O0 -g3 -Wall -c -fmessage-length=0 -mthumb -mcpu=cortex-m4 -mfloat-abi=soft -mfpu=fpv4-sp-d16 -fdata-sections -ffunction-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


