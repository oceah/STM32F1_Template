#include "AT24C02.h"

#include <algorithm>

AT24C02::AT24C02(I2C *i2c, uint8_t A)
    : i2c(i2c), A(A)
{
}

bool AT24C02::ping()
{
    return i2c->ping(AT24C02_DevAddr | A);
}

void AT24C02::write(uint8_t mem_addr, uint8_t byte)
{
    await();
    i2c->write(AT24C02_DevAddr | A, mem_addr, &byte, 1);
}

void AT24C02::write(uint8_t mem_addr, const void *p_, uint16_t size)
{
    auto p           = static_cast<const uint8_t *>(p_);
    uint8_t dev_addr = AT24C02_DevAddr | A;
    size             = std::min<uint16_t>(size, 0x100 - mem_addr);
    if (size && (mem_addr & 7)) {
        auto sz = std::min<uint16_t>(8 - (mem_addr & 7), size);
        await();
        i2c->write(dev_addr, mem_addr, p, sz);
        mem_addr += sz;
        p += sz;
        size -= sz;
    }
    while (size) {
        auto sz = std::min<uint16_t>(8, size);
        await();
        i2c->write(dev_addr, mem_addr, p, sz);
        mem_addr += sz;
        p += sz;
        size -= sz;
    }
}

uint8_t AT24C02::read(uint8_t mem_addr)
{
    uint8_t byte;
    await();
    i2c->read(AT24C02_DevAddr | A, mem_addr, &byte, 1);
    return byte;
}

void AT24C02::read(uint8_t mem_addr, void *p, uint16_t size)
{
    size = std::min<uint16_t>(size, 0x100 - mem_addr);
    await();
    i2c->read(AT24C02_DevAddr | A, mem_addr, p, size);
}

void AT24C02::await()
{
    uint8_t dev_addr = AT24C02_DevAddr | A;
    while (!i2c->ping(dev_addr));
}
