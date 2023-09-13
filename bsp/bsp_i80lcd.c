/*============================ INCLUDES ======================================*/
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "pico/stdlib.h"

/*============================ MACROS ========================================*/
#define I80LCD_WIDTH 240
#define I80LCD_HEIGHT 320
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
static uint I80LCD_RST_PIN;
static uint I80LCD_RS_PIN;
static uint I80LCD_CS_PIN;
static uint I80LCD_WR_PIN;
static uint I80LCD_RD_PIN;
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/
static void i80lcd_send_cmd(uint8_t reg)
{
    gpio_put(I80LCD_CS_PIN, 0);
    gpio_put(I80LCD_RS_PIN, 0);
    gpio_put_masked(0xFFFF, reg);
    gpio_put(I80LCD_WR_PIN, 0);
    __NOP();
    gpio_put(I80LCD_WR_PIN, 1);
    gpio_put(I80LCD_CS_PIN, 1);
}

static void i80lcd_send_data(uint16_t data)
{
    gpio_put(I80LCD_CS_PIN, 0);
    gpio_put(I80LCD_RS_PIN, 1);
    gpio_put_masked(0xFFFF, data);
    gpio_put(I80LCD_WR_PIN, 0);
    __NOP();
    gpio_put(I80LCD_WR_PIN, 1);
    gpio_put(I80LCD_CS_PIN, 1);
}

static void i80lcd_set_windows(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)
{
    // set the X coordinates
    i80lcd_send_cmd(0x2A);
    i80lcd_send_data(x_start >> 8);
    i80lcd_send_data(x_start & 0xFF);
    i80lcd_send_data((x_end - 1) >> 8);
    i80lcd_send_data((x_end - 1) & 0xFF);

    // set the Y coordinates
    i80lcd_send_cmd(0x2B);
    i80lcd_send_data(y_start >> 8);
    i80lcd_send_data(y_start & 0xFF);
    i80lcd_send_data((y_end - 1) >> 8);
    i80lcd_send_data((y_end - 1) & 0xFF);

    i80lcd_send_cmd(0X2C);
}

void i80lcd_init(void)
{
    // GPIO PIN
    I80LCD_RST_PIN = 22;
    I80LCD_RS_PIN = 16;
    I80LCD_CS_PIN = 17;
    I80LCD_RD_PIN = 18;
    I80LCD_WR_PIN = 19;

    gpio_init(I80LCD_RS_PIN);
    gpio_set_dir(I80LCD_RS_PIN, GPIO_OUT);
    gpio_put(I80LCD_RS_PIN, 0);

    gpio_init(I80LCD_CS_PIN);
    gpio_set_dir(I80LCD_CS_PIN, GPIO_OUT);
    gpio_put(I80LCD_CS_PIN, 1);

    gpio_init(I80LCD_RD_PIN);
    gpio_set_dir(I80LCD_RD_PIN, GPIO_OUT);
    gpio_put(I80LCD_RD_PIN, 1);

    gpio_init(I80LCD_WR_PIN);
    gpio_set_dir(I80LCD_WR_PIN, GPIO_OUT);
    gpio_put(I80LCD_WR_PIN, 1);
    
    gpio_init_mask(0xFFFF);
    gpio_set_dir_masked(0xFFFF, 0xFFFF);
    gpio_put_masked(0xFFFF, 0);

    gpio_init(I80LCD_RST_PIN);
    gpio_set_dir(I80LCD_RST_PIN, GPIO_OUT);
    // Hardware reset
    gpio_put(I80LCD_RST_PIN, 1);
    sleep_ms(100);
    gpio_put(I80LCD_RST_PIN, 0);
    sleep_ms(100);
    gpio_put(I80LCD_RST_PIN, 1);
    sleep_ms(100);

    //FW028TFT
    i80lcd_send_cmd(0xB2);
    i80lcd_send_data(0x0C);
    i80lcd_send_data(0x0C);
    i80lcd_send_data(0x00);
    i80lcd_send_data(0x33);
    i80lcd_send_data(0x33);
    i80lcd_send_cmd(0xB7);
    i80lcd_send_data(0x35);

    i80lcd_send_cmd(0xBB);
    i80lcd_send_data(0x2B);
    i80lcd_send_cmd(0xC3);
    i80lcd_send_data(0x17);
    i80lcd_send_cmd(0xC4);
    i80lcd_send_data(0x20);
    i80lcd_send_cmd(0xC6);
    i80lcd_send_data(0x0F);

    i80lcd_send_cmd(0x3A);
    i80lcd_send_data(0x55);
    i80lcd_send_cmd(0x36);
    i80lcd_send_data(0x00);

    i80lcd_send_cmd(0xD0);
    i80lcd_send_data(0xA4);
    i80lcd_send_data(0xA2);

    i80lcd_send_cmd(0xE0);
    i80lcd_send_data(0xF0);
    i80lcd_send_data(0x00);
    i80lcd_send_data(0x0A);
    i80lcd_send_data(0x10);
    i80lcd_send_data(0x12);
    i80lcd_send_data(0x1B);
    i80lcd_send_data(0x39);
    i80lcd_send_data(0x44);
    i80lcd_send_data(0x47);
    i80lcd_send_data(0x28);
    i80lcd_send_data(0x12);
    i80lcd_send_data(0x10);
    i80lcd_send_data(0x16);
    i80lcd_send_data(0x1B);
    i80lcd_send_cmd(0xE1);
    i80lcd_send_data(0xF0);
    i80lcd_send_data(0x00);
    i80lcd_send_data(0x0A);
    i80lcd_send_data(0x10);
    i80lcd_send_data(0x11);
    i80lcd_send_data(0x1A);
    i80lcd_send_data(0x3B);
    i80lcd_send_data(0x34);
    i80lcd_send_data(0x4E);
    i80lcd_send_data(0x3A);
    i80lcd_send_data(0x17);
    i80lcd_send_data(0x16);
    i80lcd_send_data(0x21);
    i80lcd_send_data(0x22);

    i80lcd_send_cmd(0x11);
    i80lcd_send_cmd(0x29);

    i80lcd_set_windows(0, 0, 240, 320);
    gpio_put(I80LCD_CS_PIN, 0);
    gpio_put(I80LCD_RS_PIN, 1);
    gpio_put(I80LCD_RD_PIN, 1);
    for(int32_t i = 0; i < 240 * 320; i++) {
        gpio_put_masked(0xFFFF, 0x001F);
        gpio_put(I80LCD_WR_PIN, 0);
        gpio_put(I80LCD_WR_PIN, 1);
    }
    gpio_put(I80LCD_CS_PIN, 1);
}

void Disp0_DrawBitmap(int16_t x,
                      int16_t y,
                      int16_t width,
                      int16_t height,
                      const uint8_t *bitmap)
{
    const uint16_t *phwSrc = (const uint16_t *)bitmap;
    i80lcd_set_windows(x, y, x + width, y + height);
    gpio_clr_mask(1<<I80LCD_CS_PIN);
    gpio_set_mask(1<<I80LCD_RS_PIN);
    for(int32_t i = 0; i < width * height; i++) {
        /* 16bit MPU */
        gpio_put_masked(0xFFFF, *phwSrc);
        gpio_clr_mask(1<<I80LCD_WR_PIN);
        gpio_set_mask(1<<I80LCD_WR_PIN);
        
        phwSrc++;
    }
    gpio_set_mask(1<<I80LCD_CS_PIN);
}
