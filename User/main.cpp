#include "sys.h"
#include "gpio.h"

#include "i2c.h"

#include "AT24C02.h"
#include "OLED12864IIC.h"

#include <stdio.h>

static void init()
{
    sys_init();
    gpio_init();
}

int main()
{
    init();

    auto &LED = PCout(13);
    I2C i2c1(PBout(6), PBin(7), PBout(7));
    AT24C02 rom1(&i2c1, 0b000);
    OLED12864IIC oled(&i2c1);

    for (int i = 0; i < 0xff; ++i)
        rom1.write(i, i);

    char str[65];
    uint32_t i = 0;
    while (true) {
        oled.seek(0, 0);
        sprintf(str, "Hello, Ocean!\ntimer: %d\n", i);
        oled.print(str);
        rom1.read(i % 0xff, str, 16);
        for (int j = 0; j < 16; ++j) {
            char str2[3];
            sprintf(str2, "%02X", str[j]);
            oled.print(str2);
        }
        ++i;

        LED = 0;
        delay_ms(500);
        LED = 1;
        delay_ms(500);
    }

    return 0;
}
