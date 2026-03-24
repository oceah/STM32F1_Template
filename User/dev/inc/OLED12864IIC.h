#ifndef _OLED12864IIC_H
#define _OLED12864IIC_H

#include "sys.h"
#include "i2c.h"

constexpr uint8_t OLED12864IIC_DevAddr = 0x78;

class OLED12864IIC
{
public:
    OLED12864IIC(I2C *i2c);

    void clear();

    /**
     * @brief set insert position
     * @param r row position 0-3
     * @param c col position 0-15
     */
    void seek(uint8_t r, uint8_t c);

    void print(char chr);
    void print(const char *str);

    void putimg(const uint8_t *img);

private:
    I2C *i2c;
    uint8_t r, c;

    void set_cursor(uint8_t x, uint8_t y);
};

#endif // OLED12864IIC
