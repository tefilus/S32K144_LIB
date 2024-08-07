#ifndef _MY_GPIO_H
#define _MY_GPIO_H
#include <stdint.h>
/*
=============================================================================
*												ADDRESS DEFINITIONS
=============================================================================
*/
/* PERIPHERALS CLOCK CONTROL ADDRESS */
#define PCC_BASE_ADDRESS (0x40065000U)

/* PORTA BASE ADDRESS */
#define PORTA_BASE_ADDRESS (0x40049000U)

/* GENERAL-PURPOSE INPUT/OUTPUT ADDRESS */
#define GPIO_BASE_ADDRESS (0x400FF000U)
#define PDDR_OFFSET_ADDRESS (0x14U)
/*
=============================================================================
*										BIT MASK REVISION FUNCTIONS
=============================================================================
*/
#define SET_BIT 1
#define CLEAR_BIT 0

void BitMaskRevision(uint32_t *address, uint8_t bit, uint8_t action);
void MultiBitMaskRevision(uint32_t *address, uint8_t bit, uint8_t count, uint8_t action);
/*
=============================================================================
*													GPIO FUNCTIONS
=============================================================================
*/
#define PORTA 0
#define PORTB 1
#define PORTC 2
#define PORTD 3
#define PORTE 4

#define PIN0 0
#define PIN1 1
#define PIN2 2
#define PIN3 3
#define PIN4 4
#define PIN5 5
#define PIN6 6
#define PIN7 7
#define PIN8 8
#define PIN9 9
#define PIN10 10
#define PIN11 11
#define PIN12 12
#define PIN13 13
#define PIN14 14
#define PIN15 15
#define PIN16 16

#define MODE0 0
#define MODE1 1
#define MODE2 2
#define MODE3 3
#define MODE4 4
#define MODE5 5
#define MODE6 6
#define MODE7 7

#define INPUT 0
#define OUTPUT 1
#define LOW 0
#define HIGH 1

#define IRQ_ENB 1
#define IRQ_DIS 0

void GPIO_Init(uint32_t port);
void GPIO_SetMode(uint32_t port, uint8_t pin, uint8_t mode, uint8_t direct, uint8_t IRQ);
void GPIO_WriteChannel(uint32_t port, uint8_t pin, uint8_t level);
uint8_t GPIO_ReadChannel(uint32_t port, uint8_t pin);
/*
=============================================================================
*													DELAY FUNCTIONS
=============================================================================
*/
/* SysTick ADDRESS Definition */
#define SysTick_CTRL 	(uint32_t *)(0xE000E010U)
#define SysTick_LOAD 	(uint32_t *)(0xE000E014U)
#define SysTick_VAL		(uint32_t *)(0xE000E018U)
#define SysTick_CALIB	(uint32_t *)(0xE000E01CU)

void SysTick_Init(void);
void SysTick_Handler(void);
void SysTickDelay(uint32_t Milisecond);

#endif /* _MY_GPIO_H */
