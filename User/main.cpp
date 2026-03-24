#include "sys.h"
#include "gpio.h"

#include "i2c.h"

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
    OLED12864IIC oled(&i2c1);

    char str[4 * 16];
    uint32_t i = 0;
    while (true) {
        sprintf(str, "Hello, Ocean!\ntimer: %d", i++);
        oled.seek(0, 0);
        oled.print(str);
        LED = 0;
        delay_ms(500);
        LED = 1;
        delay_ms(500);
    }

    return 0;
}
