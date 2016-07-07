
# Define project name and Ram/Flash mode here
PROJECT = myapp

# Define optimisation level here
OPT = -Os

# define warning options here
CXX_WARNINGS = -Wall -Wextra
#C_WARNINGS = -Wall -Wstrict-prototypes -Wextra
C_WARNINGS = -Wall -Wextra

# C language standard ("c89" / "iso9899:1990", "iso9899:199409",
# "c99" / "iso9899:1999", "gnu89" - default, "gnu99")
C_STD = gnu99

# C++ language standard ("c++98", "gnu++98" - default, "c++0x", "gnu++0x")
CXX_STD = c++11

##
# gcc setting
##

GCC_BIN = 

TRGT    = $(GCC_BIN)arm-none-eabi-
AS      = $(TRGT)as
CC      = $(TRGT)gcc
CPP     = $(TRGT)g++

OBJCOPY = $(TRGT)objcopy
OBJDUMP = $(TRGT)objdump
SIZE    = $(TRGT)size 

LD      = $(TRGT)gcc

HEX  = $(OBJCOPY) -O ihex 
BIN  = $(OBJCOPY) -O binary 


#CCFLAGS=-mcpu=cortex-m0 -mthumb -g  
# Tell the linker where to find the libraries -> important: use thumb versions
#LIBSPEC=-L /usr/local/gcc-arm-none-eabi/lib/gcc/arm-none-eabi/4.8.4/armv6-m

# List the object files involved in this project
ASRC = $(shell find -L $(SRCDIR) -name '*.s')
SRC = $(shell find -L $(SRCDIR) -name '*.c')
#CXXSRC = $(shell find -L $(SRCDIR) -name '*.cpp')
#OBJS    = $(ASRC:.s=.o) $(SRC:.c=.o) $(SRC:.cpp=.o)
OBJS    = $(ASRC:.s=.o) $(SRC:.c=.o)

MCFLAGS =-mcpu=cortex-m0 -mthumb -g 
#MCFLAGS =-mcpu=cortex-m0 -mthumb 

#ASFLAGS = $(MCFLAGS) -g -gdwarf-2 -Wa,-amhls=$(<:.s=.lst) $(ADEFS)
ASFLAGS = $(MCFLAGS) -gdwarf-2 -Wa,-amhls=$(<:.s=.lst) $(ADEFS)
CPFLAGS = $(MCFLAGS) $(OPT) -std=$(C_STD) -gdwarf-2 -fomit-frame-pointer -Wall -Wstrict-prototypes -fverbose-asm -D__USE_CMSIS $(DEFS)
#CPFLAGS += -Wa,-ahlms=$(<:.c=.lst) 
LDFLAGS = $(MCFLAGS) -nostartfiles -T linker_script.ld -Wl,-Map=$(PROJECT).map,--cref,--no-warn-mismatch $(LIBDIR)

LDFLAGS += --specs=nano.specs -lm -lc -lgcc -lnosys
#LDFLAGS += -Wl,--gc-sections --specs=nano.specs -lm -lc -lgcc -lnosys

# Generate dependency information
CPFLAGS += -MD -MP -MF .dep/$(@F).d

#
# makefile rules
#

all: $(OBJS) $(PROJECT).elf $(PROJECT).hex $(PROJECT).bin size


%.o : %.c
	$(CC) -c $(CPFLAGS) -I . $(INCDIR) $< -o $@

%.o : %.s
	$(AS) -c $(ASFLAGS) $< -o $@

%elf: $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) $(LIBS) -o $@
  
%hex: %elf
	$(HEX) $< $@

%bin: %elf
	$(BIN) $< $@

dump: $(PROJECT).elf
	$(OBJDUMP) -S --disassemble $(PROJECT).elf > $(PROJECT)_dump.asm

size: $(PROJECT).elf
	@echo 'Size of modules:'
	$(SIZE) -B -t --common $(OBJS)
	@echo ' '
	@echo 'Size of target .elf file:'
	$(SIZE) -B $(PROJECT).elf
	@echo ' '

clean:
	-rm -f $(OBJS)
	-rm -f $(PROJECT).elf
	-rm -f $(PROJECT).map
	-rm -f $(PROJECT).hex
	-rm -f $(PROJECT).bin
	-rm -f $(PROJECT)_dump.asm
	-rm -f $(SRC:.c=.c.bak)
	-rm -f $(SRC:.c=.lst)
	-rm -f $(ASRC:.s=.s.bak)
	-rm -f $(ASRC:.s=.lst)
	-rm -fR .dep

# 
# Include the dependency files, should be the last of the makefile
#
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)
