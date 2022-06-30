SHELL := /bin/bash

default: ## default with make will build project
	make -C build/

.PHONY: help
help: ## Show this help
	@egrep -h '\s##\s' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'


run: ## will run cmake and also build the project
	cmake . ./build
	make -C build/

cp: ## will copy files to rpi
	cp build/FlightController.uf2 /media/leojfrancis/RPI-RP2/

screen: ## will connect to serial monitor screen package is a dependency
	# screen /dev/serial/by-id/usb-Raspberry_Pi_Pico_E6609103C337932A-if00 115200;
	screen /dev/serial/by-id/usb-Raspberry_Pi_Pico_E6609103C385592A-if00 115200;

test: ## will run a test build, copy, and screen
	make
	sleep 1
	make cp
	sleep 1
	make screen