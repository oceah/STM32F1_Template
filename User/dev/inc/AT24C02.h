#ifndef _AT24C02_H
#define _AT24C02_H

#include "sys.h"
#include "i2c.h"

constexpr uint8_t AT24C02_DevAddr = 0xa0;

class AT24C02
{
public:
    AT24C02(I2C *i2c, uint8_t A);

    void write(uint8_t mem_addr, uint8_t byte);
    void read(uint8_t mem_addr, void *p, uint8_t size = 1);

private:
    I2C *i2c;
    uint8_t A;
};

#endif // _AT24C02_H
