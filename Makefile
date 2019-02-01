######################################define project name, board type, and path
include	param.mk

#####ROBOT_MODEL :   
ROBOT_MODEL     ?= pibot
#####project name
PROJECT			=  $(ROBOT_MODEL)_stm32

#########################################################################package

#PAKG: common robot_abstract math imu .....
PAKG        = board common data_holder tf imu robot transport kinematic_models motor encoder pid
              
#OS_MODULE: UCOSII UCOSIII GUI FAT
OS_MODULE = UCOSIII            

include	BSP/board.mk
include	User/makefile.mk
include OS/os.mk

CCPREFIX	?= arm-none-eabi-
CC   		= $(CCPREFIX)gcc
CXX         = $(CCPREFIX)g++
CP   		= $(CCPREFIX)objcopy
AS   		= $(CCPREFIX)gcc -x assembler-with-cpp
GDB		    = $(CCPREFIX)gdb
HEX  		= $(CP) -O ihex
BIN  		= $(CP) -O binary -S

# Define optimisation level here
OPT += -Os
OPT += -fsingle-precision-constant
OPT += -fno-common

OPT     += -mfloat-abi=hard
OPT     += -mfpu=fpv4-sp-d16

#run from Flash
DEFS    += $(DDEFS) -DRUN_FROM_FLASH=1 

MCFLAGS += -mcpu=$(MCU)

ASFLAGS  += $(MCFLAGS) $(OPT) -g -gdwarf-2 -mthumb 

CFLAGS   += $(MCFLAGS) $(OPT) -g -gdwarf-2 -mthumb \
	         -fomit-frame-pointer -Wall -fverbose-asm  \
	         $(DEFS)	 
	         
#-std=gnu++0x

CXXFLAGS += $(MCFLAGS) $(OPT) -g -gdwarf-2 -mthumb -std=c++11\
           -fomit-frame-pointer -Wall -fverbose-asm \
           -fno-exceptions -fno-rtti -fno-threadsafe-statics -fvisibility=hidden \
           $(DEFS)	         
           
# "-Xlinker --gc-sections" - removes unused code from the output binary - saves memory
LDFLAGS += $(MCFLAGS) $(OPT) -lm -g -gdwarf-2 -mthumb -nostartfiles -Xlinker --gc-sections -T$(LINKERSCRIPT) \
        -Wl,-Map=$(PROJECT).map,--cref,--no-warn-mismatch \
        $(LIBDIR) $(LIB)

##################################################################makefile rules

OBJS  	= $(filter %.o , $(ASM_SRC:.s=.o)) $(filter %.o , $(ASM_SRC:.asm=.o)) \
          $(C_SRC:.c=.o) $(CXX_SRC:.cpp=.o)

all: $(OBJS) $(PROJECT).elf  $(PROJECT).hex $(PROJECT).bin
	$(CCPREFIX)size $(PROJECT).elf
 
%.o: %.c
	$(CC) -c $(CFLAGS) -I . $(INCDIR) $< -o $@

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -I . $(INCDIR) $< -o $@

%.o: %.s
	$(AS) -c $(ASFLAGS) $< -o $@

%.o: %.asm
	$(AS) -c $(ASFLAGS) $< -o $@	

%.elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $@

%.hex: %.elf
	$(HEX) $< $@
    
%.bin: %.elf
	$(BIN)  $< $@
	
##################################################################makefile clean 
clean:
	-rm -rf $(OBJS)
	-rm -rf $(PROJECT).elf
	-rm -rf $(PROJECT).map
	-rm -rf $(PROJECT).hex
	-rm -rf $(PROJECT).bin

###########################################################################flash

include	flash.mk
     



