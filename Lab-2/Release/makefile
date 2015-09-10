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
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
AVR-Template.lss \

FLASH_IMAGE += \
AVR-Template.hex \

EEPROM_IMAGE += \
AVR-Template.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: AVR-Template.elf secondary-outputs

# Tool invocations
AVR-Template.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,AVR-Template.map -Wl,-u,vfprintf -lprintf_flt -lm -L"/home/arthurlockman/Dropbox/workspace/RBELib/Release" -mmcu=atmega644p -o "AVR-Template.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

AVR-Template.lss: AVR-Template.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S AVR-Template.elf  >"AVR-Template.lss"
	@echo 'Finished building: $@'
	@echo ' '

AVR-Template.hex: AVR-Template.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex AVR-Template.elf  "AVR-Template.hex"
	@echo 'Finished building: $@'
	@echo ' '

AVR-Template.eep: AVR-Template.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex AVR-Template.elf  "AVR-Template.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: AVR-Template.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega644p AVR-Template.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) AVR-Template.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
