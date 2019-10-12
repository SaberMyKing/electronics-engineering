################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := E:/Install_Folder/CCS_TM4C123G/ccsv6/tools/compiler/arm_5.1.6

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./main.obj" \
"./tm4c123gh6pm_startup_ccs.obj" \
"./include/Di_gpio.obj" \
"./include/Di_timer.obj" \
"./include/Di_uart.obj" \
"./include/IMU.obj" \
"./include/MPU6050.obj" \
"./include/control.obj" \
"./include/my_pwm.obj" \
"../tm4c123gh6pm.cmd" \
-l"libc.a" \
-l"C:\ti\TivaWare_C_Series-2.0.1.11577\driverlib\ccs\Debug\driverlib.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include include/subdir_vars.mk
-include subdir_rules.mk
-include include/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
demo.out \

EXE_OUTPUTS__QUOTED += \
"demo.out" \

BIN_OUTPUTS += \
demo.hex \

BIN_OUTPUTS__QUOTED += \
"demo.hex" \


# All Target
all: demo.out

# Tool invocations
demo.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"E:/Install_Folder/CCS_TM4C123G/ccsv6/tools/compiler/arm_5.1.6/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --display_error_number --diag_warning=225 --diag_wrap=off -z -m"demo.map" --heap_size=0 --stack_size=512 -i"E:/Install_Folder/CCS_TM4C123G/ccsv6/tools/compiler/arm_5.1.6/lib" -i"E:/Install_Folder/CCS_TM4C123G/ccsv6/tools/compiler/arm_5.1.6/include" --reread_libs --warn_sections --display_error_number --diag_wrap=off --xml_link_info="demo_linkInfo.xml" --rom_model -o "demo.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

demo.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: ARM Hex Utility'
	"E:/Install_Folder/CCS_TM4C123G/ccsv6/tools/compiler/arm_5.1.6/bin/armhex"  -o "demo.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "main.pp" "tm4c123gh6pm_startup_ccs.pp" "include\Di_gpio.pp" "include\Di_timer.pp" "include\Di_uart.pp" "include\IMU.pp" "include\MPU6050.pp" "include\control.pp" "include\my_pwm.pp" 
	-$(RM) "main.obj" "tm4c123gh6pm_startup_ccs.obj" "include\Di_gpio.obj" "include\Di_timer.obj" "include\Di_uart.obj" "include\IMU.obj" "include\MPU6050.obj" "include\control.obj" "include\my_pwm.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

