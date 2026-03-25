#include "AT24C02.h"

AT24C02::AT24C02(I2C *i2c, uint8_t A)
    : i2c(i2c), A(A)
{
}

void AT24C02::write(uint8_t mem_addr, uint8_t byte)
{
    uint8_t dev_addr = AT24C02_DevAddr | A;
    while (!i2c->ping(dev_addr));
    i2c->write(dev_addr, mem_addr, &byte, 1);
}

void AT24C02::read(uint8_t mem_addr, void *p, uint8_t size)
{
    uint8_t dev_addr = AT24C02_DevAddr | A;
    while (!i2c->ping(dev_addr));
    i2c->read(dev_addr, mem_addr, p, size);
}
