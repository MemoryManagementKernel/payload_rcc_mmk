
build:
	cd os && make build
	cp os/build/rcc.bin $(OUTPUT_PATH)/rcc_$(BOARD).bin
