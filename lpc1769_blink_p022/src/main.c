#include <stdint.h>

// LPC1769
#define LPC_GPIO_BASE   0x2009C000UL
#define LPC_PINCON_BASE 0x4002C000UL
#define LPC_SYSCON_BASE 0x400FC000UL

// FIO0
#define FIO0DIR (*(volatile uint32_t *)(LPC_GPIO_BASE + 0x00))
#define FIO0SET (*(volatile uint32_t *)(LPC_GPIO_BASE + 0x18))
#define FIO0CLR (*(volatile uint32_t *)(LPC_GPIO_BASE + 0x1C))

// PINSEL0
#define PINSEL0 (*(volatile uint32_t *)(LPC_PINCON_BASE + 0x00))

// PCONP
#define PCONP   (*(volatile uint32_t *)(LPC_SYSCON_BASE + 0x0C4))

#define LED_PIN 22

static void delay(volatile uint32_t d)
{
    while(d--) __asm("nop");
}

int main(void)
{
    PCONP |= (1 << 15); // GPIO power
    
    // P0[22] como GPIO
    PINSEL0 &= ~(3 << (LED_PIN * 2));
    
    // P0[22] salida
    FIO0DIR |= (1 << LED_PIN);
    
    while(1)
    {
        // Encender LED
        FIO0CLR |= (1 << LED_PIN);
        delay(500000);
        
        // Apagar LED
        FIO0SET |= (1 << LED_PIN);
        delay(500000);
    }
    
    return 0;
}
