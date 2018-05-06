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

#include "main.h"

bool i2cread(uint8_t bus,           /* i2c bus number - (see /dev/i2c-*) */
             uint8_t i2c_addr,      /* i2c device address - range: 0x03 - 0x77 */
             uint8_t reg_width,     /* 1, 2 or 3-byte wide register address (i.e. 8, 16 or 24 bits) */
             uint8_t val_width,     /* 1, 2 or 3-byte wide value to read */
             uint32_t reg_addr,     /* register to read */
             uint32_t* result)      /* where result will be written to */
{
    char fname[15];
    int file_i2c;
    struct i2c_rdwr_ioctl_data data;
    struct i2c_msg msgs[2];
    uint8_t buf[sizeof(uint8_t) * 3] = {0}; // max 24bit

    // clear
    memset(&data, 0, sizeof(data));
    memset(&msgs[0], 0, sizeof(struct i2c_msg));
    memset(&msgs[1], 0, sizeof(struct i2c_msg));

    // create filename
    sprintf(fname, "/dev/i2c-%d", bus);
    
    // open bus
    if ((file_i2c = open(fname, O_RDWR | O_NONBLOCK)) < 0)
    {
        printf("ERROR: Failed to open the i2c bus. Try 'sudo'\n\n");
        return false;
    }

    // fill in register address
    if (3 == reg_width)
    {
        buf[0] = (uint8_t)(reg_addr >> 16);
        buf[1] = (uint8_t)((reg_addr >> 8) & 0xFF);
        buf[2] = (uint8_t)(reg_addr & 0xFF);
    }
    else if (2 == reg_width)
    {
        buf[0] = (uint8_t)(reg_addr >> 8);
        buf[1] = (uint8_t)(reg_addr & 0xFF);
    }
    else
    {
        buf[0] = (uint8_t)(reg_addr & 0xFF);
    }

    // register parameters
    msgs[0].addr = i2c_addr;
    msgs[0].flags = 0;
    msgs[0].len = reg_width;
    msgs[0].buf = buf;

    // expected value parameters
    msgs[1].addr = i2c_addr;
    msgs[1].flags = I2C_M_RD;
    msgs[1].len = val_width;
    msgs[1].buf = buf;

    // read
    data.msgs = msgs;
    data.nmsgs = 2;
    if (ioctl(file_i2c, I2C_RDWR, &data) < 0)
    {
        printf("ERROR: Read failure\n\n");
        close(file_i2c);
        return false;
    }

    // construct the result
    if (3 == val_width)
        *result = (buf[0] << 16) | (buf[1]  << 8) | buf[2];
    else if (2 == val_width)
        *result = (buf[0] << 8) | buf[1];
    else
        *result = buf[0];

    close(file_i2c);
    return true;
}

