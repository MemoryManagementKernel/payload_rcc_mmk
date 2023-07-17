
build:
	cd os && make build
	cp build/rcc.bin $(OUTPUT_PATH)/rcc_$(BOARD).bin
