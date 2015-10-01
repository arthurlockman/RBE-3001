################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
Lab-3.lss \

FLASH_IMAGE += \
Lab-3.hex \

EEPROM_IMAGE += \
Lab-3.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: pre-build main-build

# Main-build Target
main-build: Lab-3.elf secondary-outputs

# Tool invocations
Lab-3.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,Lab-3.map -Wl,-u,vfprintf -lprintf_flt -lm -L"C:\Users\Tucker Haydon\Documents\eclipse workspaces\RBE3001\RBELib\Release" -mmcu=atmega644p -o "Lab-3.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Lab-3.lss: Lab-3.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S Lab-3.elf  >"Lab-3.lss"
	@echo 'Finished building: $@'
	@echo ' '

Lab-3.hex: Lab-3.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex Lab-3.elf  "Lab-3.hex"
	@echo 'Finished building: $@'
	@echo ' '

Lab-3.eep: Lab-3.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex Lab-3.elf  "Lab-3.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: Lab-3.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega644p Lab-3.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) Lab-3.elf
	-@echo ' '

pre-build:
	-make clean
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY: main-build pre-build

-include ../makefile.targets
