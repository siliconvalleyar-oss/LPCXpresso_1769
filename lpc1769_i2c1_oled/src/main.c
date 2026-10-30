#include <stdint.h>

// LPC1769
#define LPC_GPIO_BASE   0x2009C000UL
#define LPC_PINCON_BASE 0x4002C000UL
#define LPC_SYSCON_BASE 0x400FC000UL
#define LPC_I2C1_BASE   0x4005C000UL

// PINCON
#define PINSEL0 (*(volatile uint32_t *)(LPC_PINCON_BASE + 0x00))
#define PINMODE0 (*(volatile uint32_t *)(LPC_PINCON_BASE + 0x40))

// I2C1
#define I2C1CONSET (*(volatile uint32_t *)(LPC_I2C1_BASE + 0x00))
#define I2C1STAT   (*(volatile uint32_t *)(LPC_I2C1_BASE + 0x04))
#define I2C1DAT    (*(volatile uint32_t *)(LPC_I2C1_BASE + 0x08))
#define I2C1SCLH   (*(volatile uint32_t *)(LPC_I2C1_BASE + 0x10))
#define I2C1SCLL   (*(volatile uint32_t *)(LPC_I2C1_BASE + 0x14))
#define I2C1CONCLR (*(volatile uint32_t *)(LPC_I2C1_BASE + 0x18))

// SYSCON
#define PCONP      (*(volatile uint32_t *)(LPC_SYSCON_BASE + 0x0C4))
#define SCS        (*(volatile uint32_t *)(LPC_SYSCON_BASE + 0x1A0))
#define CLKSRCSEL  (*(volatile uint32_t *)(LPC_SYSCON_BASE + 0x10C))
#define PLL0CON    (*(volatile uint32_t *)(LPC_SYSCON_BASE + 0x080))
#define PLL0CFG    (*(volatile uint32_t *)(LPC_SYSCON_BASE + 0x084))
#define PLL0STAT   (*(volatile uint32_t *)(LPC_SYSCON_BASE + 0x088))
#define PLL0FEED   (*(volatile uint32_t *)(LPC_SYSCON_BASE + 0x08C))
#define CCLKCFG    (*(volatile uint32_t *)(LPC_SYSCON_BASE + 0x104))
#define PCLKSEL0   (*(volatile uint32_t *)(LPC_SYSCON_BASE + 0x1A8))
#define PCLKSEL1   (*(volatile uint32_t *)(LPC_SYSCON_BASE + 0x1AC))

#define SSD1306_ADDR 0x78

static void delay_ms(volatile uint32_t ms)
{
    volatile uint32_t i;
    for (ms = ms * 10000; ms; ms--) {
        for (i = 0; i < 10; i++) __asm("nop");
    }
}

static void pll_init(void)
{
    SCS |= (1 << 5);
    while (!(SCS & (1 << 6)));

    CLKSRCSEL = 0x01;

    PLL0CFG = (49) | (0 << 16);
    PLL0FEED = 0xAA;
    PLL0FEED = 0x55;

    PLL0CON = 0x01;
    PLL0FEED = 0xAA;
    PLL0FEED = 0x55;
    while (!(PLL0STAT & (1 << 26)));

    CCLKCFG = 0x03;

    PLL0CON = 0x03;
    PLL0FEED = 0xAA;
    PLL0FEED = 0x55;

    PCLKSEL0 = 0xAAAAAAAA;
    PCLKSEL1 = 0xAAAAAAAA;
}

static void i2c1_init(void)
{
    PCONP |= (1 << 19); // I2C1
    
    // P0.0 SDA1, P0.1 SCL1 - function 11
    PINSEL0 &= ~((3 << 0) | (3 << 2));
    PINSEL0 |= ((3 << 0) | (3 << 2));
    
    // No pull up/down
    PINMODE0 &= ~((3 << 0) | (3 << 2));
    PINMODE0 |= ((2 << 0) | (2 << 2));
    
    I2C1CONCLR = 0x6C;
    I2C1SCLH = 50;
    I2C1SCLL = 50;
    I2C1CONSET = (1 << 6);
}

static void i2c1_start(void)
{
    I2C1CONSET = (1 << 5);
    I2C1CONCLR = (1 << 3);
    while (!(I2C1CONSET & (1 << 3)));
    I2C1CONCLR = (1 << 5);
}

static void i2c1_stop(void)
{
    I2C1CONSET = (1 << 4);
    I2C1CONCLR = (1 << 3);
}

static void i2c1_write(uint8_t data)
{
    I2C1DAT = data;
    I2C1CONCLR = (1 << 3);
    while (!(I2C1CONSET & (1 << 3)));
}

static void ssd1306_cmd(uint8_t cmd)
{
    i2c1_start();
    i2c1_write(SSD1306_ADDR);
    i2c1_write(0x00);
    i2c1_write(cmd);
    i2c1_stop();
}

static void ssd1306_data(uint8_t data)
{
    i2c1_start();
    i2c1_write(SSD1306_ADDR);
    i2c1_write(0x40);
    i2c1_write(data);
    i2c1_stop();
}

static void ssd1306_init(void)
{
    delay_ms(100);
    ssd1306_cmd(0xAE);
    ssd1306_cmd(0xD5); ssd1306_cmd(0x80);
    ssd1306_cmd(0xA8); ssd1306_cmd(0x3F);
    ssd1306_cmd(0xD3); ssd1306_cmd(0x00);
    ssd1306_cmd(0x40);
    ssd1306_cmd(0x8D); ssd1306_cmd(0x14);
    ssd1306_cmd(0x20); ssd1306_cmd(0x00);
    ssd1306_cmd(0xA1);
    ssd1306_cmd(0xC8);
    ssd1306_cmd(0xDA); ssd1306_cmd(0x12);
    ssd1306_cmd(0x81); ssd1306_cmd(0xCF);
    ssd1306_cmd(0xD9); ssd1306_cmd(0xF1);
    ssd1306_cmd(0xDB); ssd1306_cmd(0x40);
    ssd1306_cmd(0xA4);
    ssd1306_cmd(0xA6);
    ssd1306_cmd(0xAF);
}

static void ssd1306_clear(void)
{
    uint16_t i;
    for (i = 0; i < 1024; i++) {
        ssd1306_data(0x00);
    }
}

int main(void)
{
    pll_init();
    i2c1_init();
    ssd1306_init();
    ssd1306_clear();
    
    while (1) {
        delay_ms(1000);
    }
    
    return 0;
}
