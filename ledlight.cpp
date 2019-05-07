#include "ledlight.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

LedLight::LedLight()
{
    fd = open("/dev/s3c2440_led0",O_RDWR);
    if (fd < 0) {
        printf("int LedLight()  ####Led device open fail####\n");
    }
}

LedLight::~LedLight()
{
    close(fd);
}

void LedLight::horizotal()
{
    unsigned char data[10] = {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18};
    if (fd >= 0) {
        for (int i = 0; i < 8; i++) {
            write(fd, data, 10);
        }
    }

}

void LedLight::vertical()
{
    unsigned char data[10] = {0x0, 0x0, 0x0, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0,0x0};
    if (fd >= 0) {
        for (int i = 0; i < 8; i++) {
            write(fd, data, 10);
        }
    }

}

void LedLight::setLedTime(int time)
{
    unsigned char LEDCODE[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
    unsigned int ledword;
    if (fd >= 0) {
        int right = time % 10;
        int left = (time / 10) % 10;
        ledword = LEDCODE[left] << 8 | LEDCODE[right];
        ioctl(fd, 0x12, ledword);
    }
}
