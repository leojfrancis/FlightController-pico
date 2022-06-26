default:
	cmake . ./build
	make -C build/

run:
	make -C build/

cp:
	cp build/FlightController.uf2 /media/leojfrancis/RPI-RP2/

screen:
	screen /dev/serial/by-id/usb-Raspberry_Pi_Pico_E6609103C337932A-if00 115200

test:
	make run;
	sleep 1;
	make cp;
	sleep 1;
	make screen;