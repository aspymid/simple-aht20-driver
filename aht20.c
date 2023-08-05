#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <pigpio.h>

#define AHT20_STATUSBIT_BUSY = 7                    # The 7th bit is the Busy indication bit. 1 = Busy, 0 = not.
#define AHT20_I2CADDR 0X38
unsigned char AHT20_CMD_STATUS[2] = {AHT20_I2CADDR<<1|0,0x71};
unsigned char AHT20_CMD_SOFTRESET[2] = {AHT20_I2CADDR<<1|0,0xBA};
unsigned char AHT20_CMD_INITIALIZE[4] = {AHT20_I2CADDR<<1|0, 0xBE, 0x08, 0x00};
unsigned char AHT20_CMD_MEASURE[4] = {AHT20_I2CADDR<<1|0,0xAC, 0x33, 0x00};
unsigned char AHT20_CMD_READ[1] = {AHT20_I2CADDR<<1|1};

int handle;
unsigned char AHT20_GET_STATUS()
{
        unsigned char value;
        i2cWriteDevice(handle, AHT20_CMD_STATUS, sizeof(AHT20_CMD_STATUS));
        value = i2cReadByte(handle);
        if(value&0x18!=0x18) 
        {
                fprintf(stderr,"Device not found, bad status. should be 0x18 and is %0.2x\n",value);
        }

        //printf( "%0.2x\n", value);
        return value;
}
void aht20_measure(float * temperature, float * humidity)
{
        int i;
        int r;
        char aout;
        unsigned char command[2];
        unsigned char value[4];
        unsigned char str[8];
        unsigned char status;

        // loop in case it is occupied elsewhere 
        while (gpioInitialise() < 0) {
                perror("gpioInitialise failed: "); 
                sleep(1);
        }
        handle = i2cOpen(1, AHT20_I2CADDR, 0);
        if(handle < 0)
        {
                perror("Could not open the I2C device: ");
                exit(-1);
        }

        status=AHT20_GET_STATUS();

        usleep(40000);
        //i2cWriteDevice(handle, AHT20_CMD_SOFTRESET, sizeof(AHT20_CMD_SOFTRESET));
        //usleep(20000);
        if((status & 0x08) !=0x08)
        {
                i2cWriteDevice(handle, AHT20_CMD_INITIALIZE, sizeof(AHT20_CMD_INITIALIZE));
                usleep(10000);
        }

        i2cWriteDevice(handle, AHT20_CMD_MEASURE, sizeof(AHT20_CMD_MEASURE));
        while ((AHT20_GET_STATUS()&0x80)  == 0x80) usleep(80000);
        i2cWriteDevice(handle,AHT20_CMD_READ,sizeof(AHT20_CMD_READ));
        i2cReadDevice(handle,value,7);

        *humidity = (value[1] << 12) | (value[2] << 4) | (value[3] >> 4);
        *humidity = *humidity * 100 / pow(2,20);

        *temperature = ((value[3] & 0xF) << 16) | (value[4] << 8) | value[5];;
        *temperature = *temperature / (pow(2,20))*200-50;

        i2cClose(handle);
        gpioTerminate();

}

#if defined testing
int main(int argc, char *argv[])
{
        float temperature, humidity;
        while ( 1 )
        {
                aht20_measure(&temperature, &humidity);
                printf("Temperature=%f\n",temperature);
                printf("Humidity=%f\n",humidity);
                sleep(1);
        }
}
#endif
