#include "i2c.h"

I2C::I2C(GPIO_PinType &SCL, GPIO_PinType &SDA_I, GPIO_PinType &SDA_O)
    : SCL(SCL), SDA_I(SDA_I), SDA_O(SDA_O)
{
    SCL   = 1;
    SDA_O = 1;
}

void I2C::set_scl(uint8_t bit)
{
    SCL = bit;
    delay_us(1);
}

void I2C::set_sda(uint8_t bit)
{
    SDA_O = bit;
    delay_us(1);
}

uint8_t I2C::get_sda()
{
    delay_us(1);
    return SDA_I;
}

void I2C::start()
{
    set_sda(1);
    set_scl(1);
    set_sda(0);
    set_scl(0);
}

void I2C::stop()
{
    set_sda(0);
    set_scl(1);
    set_sda(1);
}

void I2C::tx_byte(uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++) {
        set_sda(byte >> 7);
        set_scl(1);
        set_scl(0);
        byte <<= 1;
    }
}

uint8_t I2C::rx_byte()
{
    uint8_t byte = 0x00;
    set_sda(1);
    for (uint8_t i = 0; i < 8; i++) {
        byte <<= 1;
        set_scl(1);
        if (get_sda())
            byte |= 0x01;
        set_scl(0);
    }
    return byte;
}

void I2C::tx_ack(uint8_t bit)
{
    set_sda(bit);
    set_scl(1);
    set_scl(0);
}

uint8_t I2C::rx_ack()
{
    set_sda(1);
    set_scl(1);
    uint8_t ack = get_sda();
    set_scl(0);
    return ack;
}

bool I2C::ping(uint8_t dev_addr)
{
    // send start signal
    start();
    // send device address
    tx_byte(dev_addr);
    bool ack = rx_ack() == 0;
    stop();
    return ack;
}

void I2C::read(uint8_t dev_addr, uint8_t mem_addr, void *p_, uint32_t size)
{
    auto p = static_cast<uint8_t *>(p_);
    // send start signal
    start();
    // send device address
    tx_byte(dev_addr);
    rx_ack();
    // send device register address
    tx_byte(mem_addr);
    rx_ack();

    // resend start signal
    start();
    // resend device address
    dev_addr |= 0x01;
    tx_byte(dev_addr);
    rx_ack();
    // receive data
    while (--size) {
        *p++ = rx_byte();
        tx_ack(0);
    }
    *p = rx_byte();
    tx_ack(1);
    // send termination signal
    stop();
}

void I2C::write(uint8_t dev_addr, uint8_t mem_addr, const void *p_, uint32_t size)
{
    auto p = static_cast<const uint8_t *>(p_);
    // send start signal
    start();
    // send device address
    tx_byte(dev_addr);
    rx_ack();
    // send device register address
    tx_byte(mem_addr);
    rx_ack();
    // send data
    while (size--) {
        tx_byte(*p++);
        rx_ack();
    }
    // send stop signal
    stop();
}
