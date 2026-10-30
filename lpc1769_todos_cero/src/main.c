#include <stdint.h>

// LPC1769
#define LPC_GPIO_BASE   0x2009C000UL
#define LPC_PINCON_BASE 0x4002C000UL
#define LPC_SYSCON_BASE 0x400FC000UL

// PINCON
#define PINSEL0 (*(volatile uint32_t *)(LPC_PINCON_BASE + 0x00))
#define PINSEL1 (*(volatile uint32_t *)(LPC_PINCON_BASE + 0x04))
#define PINSEL2 (*(volatile uint32_t *)(LPC_PINCON_BASE + 0x08))
#define PINSEL3 (*(volatile uint32_t *)(LPC_PINCON_BASE + 0x0C))
#define PINSEL4 (*(volatile uint32_t *)(LPC_PINCON_BASE + 0x10))

// GPIO
#define FIO0DIR (*(volatile uint32_t *)(LPC_GPIO_BASE + 0x00))
#define FIO0SET (*(volatile uint32_t *)(LPC_GPIO_BASE + 0x18))
#define FIO0CLR (*(volatile uint32_t *)(LPC_GPIO_BASE + 0x1C))

#define FIO1DIR (*(volatile uint32_t *)(LPC_GPIO_BASE + 0x20))
#define FIO1SET (*(volatile uint32_t *)(LPC_GPIO_BASE + 0x38))
#define FIO1CLR (*(volatile uint32_t *)(LPC_GPIO_BASE + 0x3C))

#define FIO2DIR (*(volatile uint32_t *)(LPC_GPIO_BASE + 0x40))
#define FIO2SET (*(volatile uint32_t *)(LPC_GPIO_BASE + 0x58))
#define FIO2CLR (*(volatile uint32_t *)(LPC_GPIO_BASE + 0x5C))

#define FIO3DIR (*(volatile uint32_t *)(LPC_GPIO_BASE + 0x60))
#define FIO3SET (*(volatile uint32_t *)(LPC_GPIO_BASE + 0x78))
#define FIO3CLR (*(volatile uint32_t *)(LPC_GPIO_BASE + 0x7C))

#define FIO4DIR (*(volatile uint32_t *)(LPC_GPIO_BASE + 0x80))
#define FIO4SET (*(volatile uint32_t *)(LPC_GPIO_BASE + 0x98))
#define FIO4CLR (*(volatile uint32_t *)(LPC_GPIO_BASE + 0x9C))

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

#define LED_PIN 22

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

static void gpio_all_low(void)
{
    PCONP |= (1 << 15);

    PINSEL0 = 0x00000000;
    PINSEL1 = 0x00000000;
    PINSEL2 = 0x00000000;
    PINSEL3 = 0x00000000;
    PINSEL4 = 0x00000000;

    FIO0DIR = 0xFFFFFFFF;
    FIO1DIR = 0xFFFFFFFF;
    FIO2DIR = 0xFFFFFFFF;
    FIO3DIR = 0xFFFFFFFF;
    FIO4DIR = 0xFFFFFFFF;

    FIO0CLR = 0xFFFFFFFF;
    FIO1CLR = 0xFFFFFFFF;
    FIO2CLR = 0xFFFFFFFF;
    FIO3CLR = 0xFFFFFFFF;
    FIO4CLR = 0xFFFFFFFF;
}

int main(void)
{
    pll_init();
    gpio_all_low();

    FIO0DIR |= (1 << LED_PIN);
    FIO0CLR |= (1 << LED_PIN);

    while (1)
    {
        FIO0CLR |= (1 << LED_PIN);
        delay_ms(500);
        FIO0SET |= (1 << LED_PIN);
        delay_ms(500);
    }

    return 0;
}
