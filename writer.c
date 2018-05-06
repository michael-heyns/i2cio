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

bool i2cwrite(uint8_t bus,          /* i2c bus number - (see /dev/i2c-*) */
              uint8_t i2c_addr,     /* i2c device address - range: 0x03 - 0x77 */
              uint8_t reg_width,    /* 1, 2 or 3-byte wide register address (i.e. 8, 16 or 24 bits) */
              uint8_t val_width,    /* 1, 2 or 3-byte wide value to write */
              uint32_t reg_addr,    /* register to write to */
              uint32_t value)       /* the value to write into the register */
{
    char fname[15];
    int file_i2c;
    struct i2c_rdwr_ioctl_data data;
    struct i2c_msg msgs;
    uint8_t buf[sizeof(uint8_t) * 6] = {0}; // max 3 x 24bit

    // clear
    memset(&data, 0, sizeof(data));
    memset(&msgs, 0, sizeof(struct i2c_msg));

    // create filename
    sprintf(fname, "/dev/i2c-%d", bus);
    
    // open bus
    if ((file_i2c = open(fname, O_RDWR | O_NONBLOCK)) < 0)
    {
        printf("ERROR: Failed to open the i2c bus. Try 'sudo'\n\n");
        return false;
    }

    // fill in the register address
    int i;
    if (3 == reg_width)
    {
        buf[0] = (uint8_t)(reg_addr >> 16);
        buf[1] = (uint8_t)((reg_addr >> 8) & 0xFF);
        buf[2] = (uint8_t)(reg_addr & 0xFF);
        i = 3;
    }
    else if (2 == reg_width)
    {
        buf[0] = (uint8_t)(reg_addr >> 8);
        buf[1] = (uint8_t)(reg_addr & 0xFF);
        i = 2;
    }
    else
    {
        buf[0] = (uint8_t)(reg_addr & 0xFF);
        i = 1;
    }

    // fill in the value
    if (3 == val_width)
    {
        buf[i+0] = (uint8_t)(value >> 16);
        buf[i+1] = (uint8_t)((value >> 8) & 0xFF);
        buf[i+2] = (uint8_t)(value & 0xFF);
    }
    else if (2 == val_width)
    {
        buf[i+0] = (uint8_t)(value >> 8);
        buf[i+1] = (uint8_t)(value & 0xFF);
    }
    else
    {
        buf[i+0] = (uint8_t)(value & 0xFF);
    }

    // register + value detail
    msgs.addr = i2c_addr;
    msgs.flags = 0;
    msgs.len = reg_width + val_width;
    msgs.buf = buf;

    // write
    data.msgs = &msgs;
    data.nmsgs = 1;
    if (ioctl(file_i2c, I2C_RDWR, &data) < 0)
    {
        printf("ERROR: Write failure\n\n");
        close(file_i2c);
        return false;
    }

    close(file_i2c);
    return true;
}

