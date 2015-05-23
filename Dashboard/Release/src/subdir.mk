################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/selftest.c \
../src/stm32f4xx_it.c \
../src/system_stm32f4xx.c \
../src/usb_bsp.c \
../src/usbd_desc.c \
../src/usbd_usr.c 

OBJS += \
./src/main.o \
./src/selftest.o \
./src/stm32f4xx_it.o \
./src/system_stm32f4xx.o \
./src/usb_bsp.o \
./src/usbd_desc.o \
./src/usbd_usr.o 

C_DEPS += \
./src/main.d \
./src/selftest.d \
./src/stm32f4xx_it.d \
./src/system_stm32f4xx.d \
./src/usb_bsp.d \
./src/usbd_desc.d \
./src/usbd_usr.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Utilities\STM32F401-Discovery" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Libraries\STM32F4xx_StdPeriph_Driver\inc" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Libraries\STM32_USB_Device_Library\Core\inc" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Libraries\STM32_USB_OTG_Driver\inc" -I"C:/Users/Crazyfingers/ARM Workspaces/STM32F4disc/Template_Project/inc" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Libraries\CMSIS\Device\ST\STM32F4xx\Include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


