#include "AT24C02.h"

AT24C02::AT24C02(I2C *i2c, uint8_t A)
    : i2c(i2c), A(A)
{
}

void AT24C02::write(uint8_t mem_addr, uint8_t byte)
{
    i2c->write(AT24C02_DevAddr | A, mem_addr, &byte, 1);
}

void AT24C02::read(uint8_t mem_addr, void *p, uint8_t size)
{
    i2c->read(AT24C02_DevAddr | A, mem_addr, p, size);
}
