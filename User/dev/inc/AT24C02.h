#ifndef _AT24C02_H
#define _AT24C02_H

#include "sys.h"
#include "i2c.h"

constexpr uint8_t AT24C02_DevAddr = 0xa0;

class AT24C02
{
public:
    AT24C02(I2C *i2c, uint8_t A);

    /**
     * @brief ping AT24C02
     * @return true if success else false
     */
    bool ping();

    void write(uint8_t mem_addr, uint8_t byte);
    void write(uint8_t mem_addr, const void *p, uint16_t size);
    uint8_t read(uint8_t mem_addr);
    void read(uint8_t mem_addr, void *p, uint16_t size);

private:
    I2C *i2c;
    uint8_t A;

    void await();
};

#endif // _AT24C02_H
