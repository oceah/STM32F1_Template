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

// 测试芯片与测试系统的连通性
void ETA_OS(ETA_TypeDef *eta, int epochs);
// 测试芯片随机字节读写功能
void ETA_Byte_WR(ETA_TypeDef *eta, int epochs);
// 测试芯片按页读写功能
void ETA_Page_WR(ETA_TypeDef *eta, int epochs);

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
    auto test = [&](const char *title, void f(ETA_TypeDef *, int), int epochs) {
        f(&eta, epochs);
        oled.seek(0, 0);
        char str[17];
        sprintf(str, "[%s]\n", title);
        oled.print(str);
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
    };
    while (true) {
        test("Ping", ETA_OS, 1000);
        delay_ms(500);
        test("Byte W/R", ETA_Byte_WR, 10);
        delay_ms(500);
        test("Page W/R", ETA_Page_WR, 10);
        delay_ms(500);
    }

    return 0;
}

void ETA_OS(ETA_TypeDef *eta, int epochs)
{
    AT24C02 &rom = *eta->rom;
    eta->ok      = true;
    eta->msg[0]  = '\0';
    int nack     = 0;
    for (int e = 0; e < epochs; ++e)
        if (!rom.ping())
            ++nack;
    if (nack) {
        eta->ok = false;
        sprintf(eta->msg, "NACK/PING = %d/%d", nack, epochs);
        return;
    }
}

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
    for (int e = 0; e < epochs; ++e) {
        uint8_t i = randint(0, 0xff);
        uint8_t v = rom.read(i);
        if (v != map[i]) {
            eta->ok = false;
            sprintf(eta->msg, "[ADDR] = RV(EV)\n[%02X] = %02X(%02X)", i, v, map[i]);
            return;
        }
        v      = randint(0, 0xff);
        map[i] = v;
        rom.write(i, v);
    }
}

void ETA_Page_WR(ETA_TypeDef *eta, int epochs)
{
    AT24C02 &rom = *eta->rom;
    eta->ok      = true;
    eta->msg[0]  = '\0';
    uint8_t map[256];
    uint8_t page[8];
    for (int i = 0; i < 256; ++i)
        map[i] = randint(0, 0xff);
    rom.write(0, map, 256);
    for (int e = 0; e < epochs; ++e) {
        int i = randint(0, 31);
        rom.read(i * 8, page, 8);
        for (int j = 0; j < 8; ++j)
            if (page[j] != map[i * 8 + j]) {
                eta->ok = false;
                sprintf(
                    eta->msg,
                    "%02X%02X%02X%02X%02X%02X%02X%02X\n%02X%02X%02X%02X%02X%02X%02X%02X",
                    page[0], page[1], page[2], page[3],
                    page[4], page[5], page[6], page[7],
                    map[i * 8], map[i * 8 + 1], map[i * 8 + 2], map[i * 8 + 3],
                    map[i * 8 + 4], map[i * 8 + 5], map[i * 8 + 6], map[i * 8 + 7]);
                return;
            }
        for (int j = 0; j < 8; ++j)
            map[i * 8 + j] = randint(0, 0xff);
        rom.write(i * 8, map + i * 8, 8);
    }
}
