#include "sys.h"
#include "gpio.h"

#include "i2c.h"

#include "AT24C02.h"
#include "OLED12864IIC.h"

#include <stdio.h>

#include "random.h"

static void init()
{
    sys_init();
    gpio_init();
}

struct ETA_TypeDef {
    AT24C02 *rom;
    bool ok;
    char msg[68];
};

void ETA_Byte_WR(ETA_TypeDef *eta, int epochs)
{
    AT24C02 &rom = *eta->rom;
    eta->ok      = true;
    eta->msg[0]  = '\0';
    uint8_t map[256];
    for (int i = 0; i < 256; ++i) {
        uint8_t v = randint(0, 0xff);
        map[i]    = v;
        rom.write(i, v);
    }
    for (int i = 0; i < epochs; ++i) {
        uint8_t j = randint(0, 0xff);
        uint8_t v;
        rom.read(j, &v);
        if (v != map[j]) {
            eta->ok = false;
            sprintf(eta->msg, "[ADDR] = RV(EV)\n[%02X] = %02X(%02X)", j, v, map[j]);
            return;
        }
        v      = randint(0, 0xff);
        map[j] = v;
        rom.write(j, v);
    }
}

int main()
{
    init();

    I2C i2c1(PBout(6), PBin(7), PBout(7));
    AT24C02 rom1(&i2c1, 0b000);
    OLED12864IIC oled(&i2c1);

    char str[17];
    ETA_TypeDef eta;
    eta.rom  = &rom1;
    int pass = 0, failed = 0;
    while (true) {
        oled.seek(0, 0);
        oled.print("[Byte W/R]\n");
        ETA_Byte_WR(&eta, 256);
        if (eta.ok) {
            ++pass;
            sprintf(str, "PASS: %d\n", pass);
            oled.print(str);
        } else {
            ++failed;
            sprintf(str, "FAILED: %d\n", failed);
            oled.print(str);
            oled.print(eta.msg);
        }
        delay_ms(500);
    }

    return 0;
}
