/*

MIT License

Copyright (c) 2018 Michael Heyns

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef I2CIO_H
#define I2CIO_H

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>

#define NAME        "i2cio"
#define VERSION     "v1.0 " __DATE__

bool i2cscan(uint8_t bus);          /* i2c bus number - (see /dev/i2c-*) */

bool i2cread(uint8_t bus,           /* i2c bus number - (see /dev/i2c-*) */
             uint8_t i2c_addr,      /* i2c device address - range: 0x03 - 0x77 */
             uint8_t reg_width,     /* 1, 2 or 3-byte wide register address (i.e. 8, 16 or 24 bits) */
             uint8_t val_width,     /* 1, 2 or 3-byte wide value to read */
             uint32_t reg_addr,     /* register to read */
             uint32_t* result);     /* where result will be written to */

bool i2cwrite(uint8_t bus,          /* i2c bus number - (see /dev/i2c-*) */
              uint8_t i2c_addr,     /* i2c device address - range: 0x03 - 0x77 */
              uint8_t reg_width,    /* 1, 2 or 3-byte wide register address (i.e. 8, 16 or 24 bits) */
              uint8_t val_width,    /* 1, 2 or 3-byte wide value to write */
              uint32_t reg_addr,    /* register to write to */
              uint32_t value);      /* the value to write into the register */
#endif

