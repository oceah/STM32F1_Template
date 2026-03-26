#ifndef _I2C_H
#define _I2C_H

#include "sys.h"

class I2C
{
public:
    I2C(GPIO_PinType &SCL, GPIO_PinType &SDA_I, GPIO_PinType &SDA_O);

    /**
     * @brief ping device at dev_addr
     * @return true if success else false
     */
    bool ping(uint8_t dev_addr);

    /**
     * @brief read data sized 'size' from 'dev_addr:mem_addr' to 'p'
     * @param dev_addr 7 bit device address << 1
     * @param mem_addr device register address
     * @param p pointer of data
     * @param size bytes of data
     */
    void read(uint8_t dev_addr, uint8_t mem_addr, void *p, uint32_t size);

    /**
     * @brief write data sized 'size' from 'p' to 'dev_addr:mem_addr'
     * @param dev_addr 7 bit device address << 1
     * @param mem_addr device register address
     * @param p pointer of data
     * @param size bytes of data
     */
    void write(uint8_t dev_addr, uint8_t mem_addr, const void *p, uint32_t size);

private:
    GPIO_PinType &SCL;
    GPIO_PinType &SDA_I;
    GPIO_PinType &SDA_O;

    void set_scl(uint8_t bit);
    void set_sda(uint8_t bit);
    uint8_t get_sda();

    void start();
    void stop();

    void tx_byte(uint8_t byte);
    uint8_t rx_byte();
    void tx_ack(uint8_t bit);
    uint8_t rx_ack();
};

#endif
