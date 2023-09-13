/*============================ INCLUDES ======================================*/
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

/*============================ MACROS ========================================*/
#define SPI_PORT spi0
#define SPILCD_WIDTH 240
#define SPILCD_HEIGHT 240
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
static uint SPILCD_RST_PIN;
static uint SPILCD_DC_PIN;
static uint SPILCD_CS_PIN;
static uint SPILCD_CLK_PIN;
static uint SPILCD_MOSI_PIN;
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/
static void spilcd_send_cmd(uint8_t reg)
{
    gpio_put(SPILCD_DC_PIN, 0);
    gpio_put(SPILCD_CS_PIN, 0);
    spi_write_blocking(SPI_PORT, &reg, 1);
    gpio_put(SPILCD_CS_PIN, 1);
}

static void spilcd_send_data(uint8_t data)
{
    gpio_put(SPILCD_DC_PIN, 1);
    gpio_put(SPILCD_CS_PIN, 0);
    spi_write_blocking(SPI_PORT, &data, 1);
    gpio_put(SPILCD_CS_PIN, 1);
}

static void spilcd_set_windows(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)
{
    // set the X coordinates
    spilcd_send_cmd(0x2A);
    spilcd_send_data(x_start >> 8);
    spilcd_send_data(x_start & 0xFF);
    spilcd_send_data((x_end - 1) >> 8);
    spilcd_send_data((x_end - 1) & 0xFF);

    // set the Y coordinates
    spilcd_send_cmd(0x2B);
    spilcd_send_data(y_start >> 8);
    spilcd_send_data(y_start & 0xFF);
    spilcd_send_data((y_end - 1) >> 8);
    spilcd_send_data((y_end - 1) & 0xFF);

    spilcd_send_cmd(0X2C);
}

void spilcd_init(void)
{
    // GPIO PIN
    SPILCD_RST_PIN = 22;
    SPILCD_DC_PIN = 16;
    SPILCD_CS_PIN = 17;
    SPILCD_CLK_PIN = 18;
    SPILCD_MOSI_PIN = 19;

    spi_init(SPI_PORT, 100000 * 1000);
    gpio_set_function(SPILCD_CLK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPILCD_MOSI_PIN, GPIO_FUNC_SPI);

    gpio_init(SPILCD_DC_PIN);
    gpio_set_dir(SPILCD_DC_PIN, GPIO_OUT);
    gpio_put(SPILCD_DC_PIN, 0);

    gpio_init(SPILCD_CS_PIN);
    gpio_set_dir(SPILCD_CS_PIN, GPIO_OUT);
    gpio_put(SPILCD_CS_PIN, 1);

    gpio_init(SPILCD_RST_PIN);
    gpio_set_dir(SPILCD_RST_PIN, GPIO_OUT);
    // Hardware reset
    gpio_put(SPILCD_RST_PIN, 1);
    sleep_ms(100);
    gpio_put(SPILCD_RST_PIN, 0);
    sleep_ms(100);
    gpio_put(SPILCD_RST_PIN, 1);
    sleep_ms(100);
    // st7789v2
    spilcd_send_cmd(0xEF);

    spilcd_send_cmd(0xEB);
    spilcd_send_data(0x14);

    spilcd_send_cmd(0xFE);
    spilcd_send_cmd(0xEF);

    spilcd_send_cmd(0xEB);
    spilcd_send_data(0x14);

    spilcd_send_cmd(0x84);
    spilcd_send_data(0x40);

    spilcd_send_cmd(0x85);
    spilcd_send_data(0xFF);

    spilcd_send_cmd(0x86);
    spilcd_send_data(0xFF);

    spilcd_send_cmd(0x87);
    spilcd_send_data(0xFF);

    spilcd_send_cmd(0x88);
    spilcd_send_data(0x0A);

    spilcd_send_cmd(0x89);
    spilcd_send_data(0x21);

    spilcd_send_cmd(0x8A);
    spilcd_send_data(0x00);

    spilcd_send_cmd(0x8B);
    spilcd_send_data(0x80);

    spilcd_send_cmd(0x8C);
    spilcd_send_data(0x01);

    spilcd_send_cmd(0x8D);
    spilcd_send_data(0x01);

    spilcd_send_cmd(0x8E);
    spilcd_send_data(0xFF);

    spilcd_send_cmd(0x8F);
    spilcd_send_data(0xFF);

    spilcd_send_cmd(0xB6);
    spilcd_send_data(0x00);
    spilcd_send_data(0x00);

    spilcd_send_cmd(0x36);
    spilcd_send_data(0x00);

    spilcd_send_cmd(0x3A);
    spilcd_send_data(0x55);

    spilcd_send_cmd(0x90);
    spilcd_send_data(0x08);
    spilcd_send_data(0x08);
    spilcd_send_data(0x08);
    spilcd_send_data(0x08);

    spilcd_send_cmd(0xBD);
    spilcd_send_data(0x06);

    spilcd_send_cmd(0xBC);
    spilcd_send_data(0x00);

    spilcd_send_cmd(0xFF);
    spilcd_send_data(0x60);
    spilcd_send_data(0x01);
    spilcd_send_data(0x04);

    spilcd_send_cmd(0xC3);
    spilcd_send_data(0x13);
    spilcd_send_cmd(0xC4);
    spilcd_send_data(0x13);

    spilcd_send_cmd(0xC9);
    spilcd_send_data(0x22);

    spilcd_send_cmd(0xBE);
    spilcd_send_data(0x11);

    spilcd_send_cmd(0xE1);
    spilcd_send_data(0x10);
    spilcd_send_data(0x0E);

    spilcd_send_cmd(0xDF);
    spilcd_send_data(0x21);
    spilcd_send_data(0x0c);
    spilcd_send_data(0x02);

    spilcd_send_cmd(0xF0);
    spilcd_send_data(0x45);
    spilcd_send_data(0x09);
    spilcd_send_data(0x08);
    spilcd_send_data(0x08);
    spilcd_send_data(0x26);
    spilcd_send_data(0x2A);

    spilcd_send_cmd(0xF1);
    spilcd_send_data(0x43);
    spilcd_send_data(0x70);
    spilcd_send_data(0x72);
    spilcd_send_data(0x36);
    spilcd_send_data(0x37);
    spilcd_send_data(0x6F);

    spilcd_send_cmd(0xF2);
    spilcd_send_data(0x45);
    spilcd_send_data(0x09);
    spilcd_send_data(0x08);
    spilcd_send_data(0x08);
    spilcd_send_data(0x26);
    spilcd_send_data(0x2A);

    spilcd_send_cmd(0xF3);
    spilcd_send_data(0x43);
    spilcd_send_data(0x70);
    spilcd_send_data(0x72);
    spilcd_send_data(0x36);
    spilcd_send_data(0x37);
    spilcd_send_data(0x6F);

    spilcd_send_cmd(0xED);
    spilcd_send_data(0x1B);
    spilcd_send_data(0x0B);

    spilcd_send_cmd(0xAE);
    spilcd_send_data(0x77);

    spilcd_send_cmd(0xCD);
    spilcd_send_data(0x63);

    spilcd_send_cmd(0x70);
    spilcd_send_data(0x07);
    spilcd_send_data(0x07);
    spilcd_send_data(0x04);
    spilcd_send_data(0x0E);
    spilcd_send_data(0x0F);
    spilcd_send_data(0x09);
    spilcd_send_data(0x07);
    spilcd_send_data(0x08);
    spilcd_send_data(0x03);

    spilcd_send_cmd(0xE8);
    spilcd_send_data(0x04);

    spilcd_send_cmd(0xE9);
    spilcd_send_data(0x0A);

    spilcd_send_cmd(0x62);
    spilcd_send_data(0x18);
    spilcd_send_data(0x0D);
    spilcd_send_data(0x71);
    spilcd_send_data(0xED);
    spilcd_send_data(0x70);
    spilcd_send_data(0x70);
    spilcd_send_data(0x18);
    spilcd_send_data(0x0F);
    spilcd_send_data(0x71);
    spilcd_send_data(0xEF);
    spilcd_send_data(0x70);
    spilcd_send_data(0x70);

    spilcd_send_cmd(0x63);
    spilcd_send_data(0x18);
    spilcd_send_data(0x11);
    spilcd_send_data(0x71);
    spilcd_send_data(0xF1);
    spilcd_send_data(0x70);
    spilcd_send_data(0x70);
    spilcd_send_data(0x18);
    spilcd_send_data(0x13);
    spilcd_send_data(0x71);
    spilcd_send_data(0xF3);
    spilcd_send_data(0x70);
    spilcd_send_data(0x70);

    spilcd_send_cmd(0x64);
    spilcd_send_data(0x28);
    spilcd_send_data(0x29);
    spilcd_send_data(0xF1);
    spilcd_send_data(0x01);
    spilcd_send_data(0xF1);
    spilcd_send_data(0x00);
    spilcd_send_data(0x07);

    spilcd_send_cmd(0x66);
    spilcd_send_data(0x3C);
    spilcd_send_data(0x00);
    spilcd_send_data(0xCD);
    spilcd_send_data(0x67);
    spilcd_send_data(0x45);
    spilcd_send_data(0x45);
    spilcd_send_data(0x10);
    spilcd_send_data(0x00);
    spilcd_send_data(0x00);
    spilcd_send_data(0x00);

    spilcd_send_cmd(0x67);
    spilcd_send_data(0x00);
    spilcd_send_data(0x3C);
    spilcd_send_data(0x00);
    spilcd_send_data(0x00);
    spilcd_send_data(0x00);
    spilcd_send_data(0x01);
    spilcd_send_data(0x54);
    spilcd_send_data(0x10);
    spilcd_send_data(0x32);
    spilcd_send_data(0x98);

    spilcd_send_cmd(0x74);
    spilcd_send_data(0x10);
    spilcd_send_data(0x85);
    spilcd_send_data(0x80);
    spilcd_send_data(0x00);
    spilcd_send_data(0x00);
    spilcd_send_data(0x4E);
    spilcd_send_data(0x00);

    spilcd_send_cmd(0x98);
    spilcd_send_data(0x3e);
    spilcd_send_data(0x07);

    spilcd_send_cmd(0x35);
    spilcd_send_cmd(0x21);

    spilcd_send_cmd(0x11);

    spilcd_send_cmd(0x29);
}

void Disp0_DrawBitmap(int16_t x,
                      int16_t y,
                      int16_t width,
                      int16_t height,
                      const uint8_t *bitmap)
{
    spilcd_set_windows(x, y, x + width, y + height);
    gpio_put(SPILCD_DC_PIN, 1);
    gpio_put(SPILCD_CS_PIN, 0);
    spi_set_format(SPI_PORT, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    spi_write16_blocking(SPI_PORT, (const uint16_t *)bitmap, width * height);
    spi_set_format(SPI_PORT, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    gpio_put(SPILCD_CS_PIN, 1);
}
