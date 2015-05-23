################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USB/usb_core.c \
../USB/usb_dcd.c \
../USB/usb_dcd_int.c \
../USB/usbd_core.c \
../USB/usbd_hid_core.c \
../USB/usbd_ioreq.c \
../USB/usbd_req.c 

OBJS += \
./USB/usb_core.o \
./USB/usb_dcd.o \
./USB/usb_dcd_int.o \
./USB/usbd_core.o \
./USB/usbd_hid_core.o \
./USB/usbd_ioreq.o \
./USB/usbd_req.o 

C_DEPS += \
./USB/usb_core.d \
./USB/usb_dcd.d \
./USB/usb_dcd_int.d \
./USB/usbd_core.d \
./USB/usbd_hid_core.d \
./USB/usbd_ioreq.d \
./USB/usbd_req.d 


# Each subdirectory must supply rules for building sources it contributes
USB/%.o: ../USB/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DUSB_OTG_FS_CORE -DUSE_STM32F401 -DSTM32F401xx -DUSE_USB_OTG_FS -I"C:/Users/Crazyfingers/ARM Workspaces/STM32F4disc/Dashboard/inc" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Libraries\CMSIS\Device\ST\STM32F4xx\Include" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Utilities\STM32F401-Discovery" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Libraries\CMSIS\Include" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Libraries\STM32F4xx_StdPeriph_Driver\inc" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Libraries\STM32_USB_Device_Library\Core\inc" -I"D:\STM32F401drivers\STM32F401-Discovery_FW_V1.0.0\Libraries\STM32_USB_OTG_Driver\inc" -O0 -g3 -Wall -c -fmessage-length=0 -mthumb -mcpu=cortex-m4 -mfloat-abi=soft -mfpu=fpv4-sp-d16 -fdata-sections -ffunction-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


