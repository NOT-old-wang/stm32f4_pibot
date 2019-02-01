OPENOCD_PATH    	= 
OPENOCD_TCL_PATH	= 

BURN_TYPE      	= swd_openocd_flash
DEBUG_TYPE		= swd_openocd_debug
ERASE_TYPE		= swd_openocd_erase

##################################################################flash 
burn: $(BURN_TYPE)
debug: $(DEBUG_TYPE)
erase: $(ERASE_TYPE)

##################################################################
BURN_CONFIG_FILE = stm32f4x

swd_openocd_flash:$(PROJECT).bin 
	sudo $(OPENOCD_PATH)openocd -s $(OPENOCD_TCL_PATH)tcl -f interface/jlink.cfg -c "transport select swd" -f target/$(BURN_CONFIG_FILE).cfg -c "init" -c "reset halt" -c "sleep 100" -c "wait_halt 2" -c "flash write_image erase $(PROJECT).bin 0x08000000" -c "sleep 100" -c "verify_image $(PROJECT).bin 0x08000000" -c "sleep 100" -c "reset run" -c shutdown

swd_openocd_debug:$(PROJECT).bin 
	sudo xterm -e $(OPENOCD_PATH)openocd -s $(OPENOCD_TCL_PATH)tcl -f interface/jlink.cfg -c "transport select swd" -f target/$(BURN_CONFIG_FILE).cfg -c "init" -c "halt" -c "reset halt" &
	$(GDB) --eval-command="target remote localhost:3333" $(PROJECT).elf 

swd_openocd_erase:	
	sudo $(OPENOCD_PATH)openocd -s $(OPENOCD_TCL_PATH)tcl -f interface/jlink.cfg -c "transport select swd" -f target/$(BURN_CONFIG_FILE).cfg  -c "init" -c "reset halt" -c "sleep 100" -c "$(BURN_CONFIG_FILE) mass_erase 0" -c "sleep 100" -c shutdown 
##################################################################
stlink_openocd_flash: $(PROJECT).bin
	sudo $(OPENOCD_PATH)openocd -s $(OPENOCD_TCL_PATH)tcl -f interface/stlink-v2.cfg -f target/$(BURN_CONFIG_FILE)_stlink.cfg -c "init" -c "reset halt" -c "sleep 100" -c "wait_halt 2" -c "flash write_image erase $(PROJECT).bin 0x08000000" -c "sleep 100" -c "verify_image $(PROJECT).bin 0x08000000" -c "sleep 100" -c "reset run" -c shutdown

stlink_openocd_debug: $(PROJECT).elf stlink_openocd_flash
	xterm -e $(OPENOCD_PATH)openocd -s $(OPENOCD_TCL_PATH)tcl -f interface/stlink-v2.cfg -f target/$(BURN_CONFIG_FILE)_stlink.cfg -c "init" -c "halt" -c "reset halt" &
	$(GDB) --eval-command="target remote localhost:3333" $(PROJECT).elf 
		
stlink_openocd_erase:
	sudo $(OPENOCD_PATH)openocd -s $(OPENOCD_TCL_PATH)tcl -f interface/stlink-v2.cfg -f target/$(BURN_CONFIG_FILE)_stlink.cfg -c "init" -c "reset halt" -c "sleep 100" -c "$(BURN_CONFIG_FILE) mass_erase 0" -c "sleep 100" -c shutdown 
##################################################################


