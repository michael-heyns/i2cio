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

bool i2cscan(uint8_t bus)
{
    uint8_t buffer[4];
    char fname[15];
    int file_i2c;

    // create filename
    sprintf(fname, "/dev/i2c-%d", bus);
    
    printf("\n%s:\n", fname);
    
    // open device file
    if ((file_i2c = open(fname, O_RDWR | O_NONBLOCK)) < 0)
    {
        printf("ERROR: Failed to open the i2c bus. Try 'sudo'\n\n");
        return false;
    }

    // run through all the possible I2C addresses (0x03-0x77) and show the
    // ones accupied by a device driver and the ones in user space
    printf("     0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
    for (uint8_t addr = 0; addr < 0x7F; addr += 16)
    {
        printf("%02X: ", addr);
        for (uint8_t offset = 0; offset < 16; offset++)
        {
            if ((addr + offset) >= 0x03 && (addr + offset) <= 0x77)
            {
                if (ioctl(file_i2c, I2C_SLAVE, addr + offset) < 0)
                {
                    printf("DD-");   /* device + driver */
                }
                else
                {
                    uint8_t length = 1;
                    if (read(file_i2c, buffer, length) != length)
                        printf("-- ");                        /* nothing here */
                    else
                        printf("%02X ", addr + offset);       /* available */
                }
            }
            else
                printf("   ");
        }
        printf("\n");
    }
    printf("    --=No device  DD=Device+Driver  03-77=Available\n");

    close(file_i2c);
    printf("\n");
    
    return true;
}

