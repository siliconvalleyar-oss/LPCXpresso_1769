#include <stdint.h>
#include <stdbool.h>

// LPC11Uxx register definitions (minimal)
#define LPC_GPIO_BASE      0x50000000UL
#define LPC_IOCON_BASE     0x40044000UL
#define LPC_SYSCON_BASE    0x40048000UL

// GPIO registers
#define GPIO_DIR0          (*(volatile uint32_t *)(LPC_GPIO_BASE + 0x0000))
#define GPIO_SET0          (*(volatile uint32_t *)(LPC_GPIO_BASE + 0x001C))
#define GPIO_CLR0          (*(volatile uint32_t *)(LPC_GPIO_BASE + 0x0020))

#define GPIO_DIR1          (*(volatile uint32_t *)(LPC_GPIO_BASE + 0x2000))
#define GPIO_SET1          (*(volatile uint32_t *)(LPC_GPIO_BASE + 0x201C))
#define GPIO_CLR1          (*(volatile uint32_t *)(LPC_GPIO_BASE + 0x2020))

// IOCON registers
#define IOCON_PIO0_7       (*(volatile uint32_t *)(LPC_IOCON_BASE + 0x01C))
#define IOCON_PIO1_9       (*(volatile uint32_t *)(LPC_IOCON_BASE + 0x0E4))
#define IOCON_PIO1_10      (*(volatile uint32_t *)(LPC_IOCON_BASE + 0x0E8))

// SYSCON
#define SYSAHBCLKCTRL      (*(volatile uint32_t *)(LPC_SYSCON_BASE + 0x080))

// RGB LED pins for OM13085 (ánodo común)
#define RED_PIN            7
#define GREEN_PIN          9
#define BLUE_PIN           10

static void delay(volatile uint32_t count)
{
    while (count--) {
        __asm volatile ("nop");
    }
}

static void rgb_init(void)
{
    // Enable GPIO clock
    SYSAHBCLKCTRL |= (1 << 6);
    
    // Configure IOCON - FUNC0, digital, no pull
    IOCON_PIO0_7  = 0x80;
    IOCON_PIO1_9  = 0x80;
    IOCON_PIO1_10 = 0x80;
    
    // Set as outputs
    GPIO_DIR0 |= (1 << RED_PIN);
    GPIO_DIR1 |= (1 << GREEN_PIN);
    GPIO_DIR1 |= (1 << BLUE_PIN);
    
    // Turn off all (ánodo común - HIGH = off)
    GPIO_SET0 |= (1 << RED_PIN);
    GPIO_SET1 |= (1 << GREEN_PIN);
    GPIO_SET1 |= (1 << BLUE_PIN);
}

static void rgb_set(bool red, bool green, bool blue)
{
    // ánodo común: LOW = on
    if (red) GPIO_CLR0 |= (1 << RED_PIN);
    else GPIO_SET0 |= (1 << RED_PIN);
    
    if (green) GPIO_CLR1 |= (1 << GREEN_PIN);
    else GPIO_SET1 |= (1 << GREEN_PIN);
    
    if (blue) GPIO_CLR1 |= (1 << BLUE_PIN);
    else GPIO_SET1 |= (1 << BLUE_PIN);
}

int main(void)
{
    rgb_init();
    
    while (1) {
        rgb_set(true, false, false);
        delay(300000);
        rgb_set(false, true, false);
        delay(300000);
        rgb_set(false, false, true);
        delay(300000);
        rgb_set(true, true, false);
        delay(300000);
        rgb_set(false, true, true);
        delay(300000);
        rgb_set(true, false, true);
        delay(300000);
        rgb_set(true, true, true);
        delay(300000);
        rgb_set(false, false, false);
        delay(300000);
    }
    
    return 0;
}
