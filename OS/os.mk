################################################################################  
# OS modules: RTOS   : UCOSII UCOSIII NUTTX 
#             GUI    : EMWIN  STEMWIN
#             FATFS
#
################################################################################  

INCDIR          +=-IOS/

USE_UCOSII   = $(strip $(filter UCOSII , $(OS_MODULE)))
USE_UCOSIII  = $(strip $(filter UCOSIII, $(OS_MODULE)))
USE_GUI      = $(strip $(filter GUI , $(OS_MODULE)))
USE_FATFS    = $(strip $(filter FATFS , $(OS_MODULE)))

# check whether use multiple OSs
ifeq "$(USE_UCOSII)$(USE_UCOSIII)" "UCOSIIUCOSIII"
$(error Please just select one OS)
endif

include	OS/STM32F4/stm32f4.mk


