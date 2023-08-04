# simple aht20 driver
Contains and aht20 driver written in "C"

The humidity and temperature sensor AHT20 by Asair, coupled with a BMP280 from Bosch are a good replacement for the BME280 that is hard to find or expensive at the time I wrote this driver. It is possible to buy, for a good price, a breakout board that contains both chips.

The code works for me. Use at your own risk. I do not accept any responsibility related to this code and procedures. It is provided as is with no guarantee or warranty what so ever.

Positive comments and suggestions are welcomed.

Installation and testing

To compile and run as a test, open the code and define the symbol "testing". This will enable the compilation to the testing code i.e. it defines the "main" procedure.

Prerequisite:
  -linux on a Raspberry Pi
  -a correctly connected aht20 device on the default i2c bus.
  -pigpio library

To compile: gcc aht20.c -lpigpio -o testing

To test, as root: ./testing

The folloing is the relevent datasheet : http://www.aosong.com/userfiles/files/media/Data%20Sheet%20AHT20.pdf
