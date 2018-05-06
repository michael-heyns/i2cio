# i2cio
Generic alternative to 'i2cdetect', 'i2cget' and 'i2cset' which can handle 8, 16 and 24bit register and value widths.

Typicallly used on (but not limited to) ARM-based Linux platforms (such as the Raspberry Pie, NVIDIA TX1/TX2 and others) to detect and access the I2C busses.

I found i2cget and i2cset cumbersome to use and not able to do 16 bit operations - at least not the version I was stuck with.

Improvements to and comments on i2cio are welcome. 

**Install & Build**
```
gcc -O3 -o i2cio *.c
```
_or_
```
./build.sh
```

**Syntax**
```
        i2cio scan | read | write <parameters>
```
    
***scan***
```
    i2cio scan b
```
        b           = The bus number taken from the device filename.
                      Example: ```# i2cio scan 4```   --> will scan /dev/i2c-4

***read***
```
    i2cio read b addr [w/]reg [w/]
```
        b           = The bus number taken from the device filename.
                      Example: The number 5 means/dev/i2c-5
        addr        = The I2C device address
                      Example: 38 (or 0x38)
        [w/]        = An optional width specifier: Options are: 8/, 16/, 24/
                      Example: 16/3006 (or 16/0x3006) means 16-bit value 0x3006.
                      If nothing specified, then 8/ is assumed.
        reg         = The I2C device register to read
                      Example: 52 (or 0x52) means register 0x52
                      The stand-alone [w/] is the width of the result

***write***
```
    i2cio write b addr [w/]reg [w/]value
```
        b           = The bus number taken from the device filename.
                      Example: The number 5 means/dev/i2c-5
        addr        = The I2C device address
                      Example: 38 (or 0x38)
        [w/]        = An optional width specifier: Options are: 8/, 16/, 24/
                      Example: 16/3006 (or 16/0x3006) means 16-bit value 0x3006.
                      If nothing specified, then 8/ is assumed.
        reg         = The I2C device register to read
                      Example: 52 (or 0x52) or 16/3006 or 24/0x3f4408
        value       = The value to write into the register
                      Example: 2d (or 0x2d) or 24/3fff5 or 16/ff04
    
***Other commands***
```
        i2cio --version
        i2cio -h | --help
```

***Examples***
```
        i2cio scan 2                 --> scans /dev/i2c-2
        i2cio read 2 70 0            --> reads /dev/i2c-2 device 0x70, register 0
        i2cio read 2 70 0 16/        --> ..as above but request a 16-bit return value
        i2cio read 2 70 16/3006 /16  --> ..as above but from 16-bit register 0x3006
        i2cio write 2 70 0 4f              --> writes 0x47 to register 0
        i2cio write 2 70 0 16/1e4f         --> writes 0x1e47 to register 0
        i2cio write 2 70 16/3006 16/1e4f   --> writes 0x1e47 to register 0x3006
```

Michael Heyns

