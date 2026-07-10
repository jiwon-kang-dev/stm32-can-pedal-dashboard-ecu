#include <stdint.h>

/* RCC */
#define RCC_AHB1ENR   (*(volatile uint32_t*)0x40023830)
#define RCC_APB1ENR   (*(volatile uint32_t*)0x40023840)

/* GPIOA */
#define GPIOA_MODER   (*(volatile uint32_t*)0x40020000)
#define GPIOA_AFRL    (*(volatile uint32_t*)0x40020020)
#define GPIOA_ODR     (*(volatile uint32_t*)0x40020014)

/* USART2 */
#define USART2_SR     (*(volatile uint32_t*)0x40004400)
#define USART2_DR     (*(volatile uint32_t*)0x40004404)
#define USART2_BRR    (*(volatile uint32_t*)0x40004408)
#define USART2_CR1    (*(volatile uint32_t*)0x4000440C)

void delay(void)
{
    for (volatile uint32_t i = 0; i < 500000; i++);
}

void USART2_Init(void)
{
    /* GPIOA clock enable */
    RCC_AHB1ENR |= (1 << 0);

    /* USART2 clock enable */
    RCC_APB1ENR |= (1 << 17);

    /*
     * PA2 = USART2_TX
     * PA3 = USART2_RX
     * PA5 = LD2 LED
     */

    /* PA2, PA3 alternate function mode */
    GPIOA_MODER &= ~((3 << (2 * 2)) | (3 << (3 * 2)));
    GPIOA_MODER |=  ((2 << (2 * 2)) | (2 << (3 * 2)));

    /* PA5 output mode */
    GPIOA_MODER &= ~(3 << (5 * 2));
    GPIOA_MODER |=  (1 << (5 * 2));

    /* PA2, PA3 AF7 = USART2 */
    GPIOA_AFRL &= ~((0xF << (2 * 4)) | (0xF << (3 * 4)));
    GPIOA_AFRL |=  ((7 << (2 * 4)) | (7 << (3 * 4)));

    /*
     * Baudrate 115200
     * Default clock HSI 16MHz 기준
     * BRR = 16000000 / 115200 ≈ 139 = 0x8B
     */
    USART2_BRR = 0x8B;

    /* USART enable, transmitter enable, receiver enable */
    USART2_CR1 = (1 << 13) | (1 << 3) | (1 << 2);
}

void USART2_SendChar(char c)
{
    /* TXE bit 기다리기 */
    while (!(USART2_SR & (1 << 7)));

    USART2_DR = c;
}

void USART2_SendString(char *str)
{
    while (*str)
    {
        USART2_SendChar(*str++);
    }
}

int main(void)
{
    USART2_Init();

    while (1)
    {
        USART2_SendString("Hello STM32\r\n");

        /* LD2 toggle: PA5 */
        GPIOA_ODR ^= (1 << 5);

        delay();
    }
}
