# simple aht20 driver
Contains and aht20 driver written in "C"

The AHT20 when coupled with a BMP280 for Bosch are a good replacement for the BME280 that is hard to find or expensive at the time I wrote this driver. It is possible to buy for a good price a breakout board that contains both chips.

The code works for me. Use at your own risk. I do not accept any responsibility related to this code and procedures. It is provided as is with to guarantee or warranty what so ever.

Positive comments and suggestions are welcomed.

Installation and testing

To compile and run as a test, open the code and define the symbol "testing". This will enable the compilation to the testing code i.e. it defines the "main" procedure.

Make sure that you have installed the pigpio library.

To compile: gcc aht20.c -lpigpio -o testing
To test, as root, ./testing
