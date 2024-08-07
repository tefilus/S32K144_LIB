#include "MyGPIO.h"
/*
=============================================================================
*												GLOBAL VARIABLE
=============================================================================
*/
static uint32_t g_Count; /* Time counter variable */
/*
=============================================================================
*												FUNCTIONS CODE
=============================================================================
*/
void BitMaskRevision(uint32_t *address, uint8_t position, uint8_t action)
{
	unsigned int iBit;
	iBit = (1 << position);

	/* Set case */
	if (action == SET_BIT)
	{
		*address |= iBit;
		return;
	}

	/* Clear case */
	iBit = ~iBit;
	*address &= iBit;
}
/*
=============================================================================
*/
void MultiBitMaskRevision(uint32_t *address, uint8_t position, uint8_t count, uint8_t action)
{
	/* Create mask */
	unsigned int iBit;
	iBit = ~(unsigned int)0;	/* Set all bit as 1 */
	iBit <<= count;				/* Set many first bits as 0 */
	iBit = (~iBit) << position; /* Reverse bit and move to specific position */

	/* Set case */
	if (action == SET_BIT)
	{
		*address |= iBit;
		return;
	}

	/* Clear case */
	iBit = ~iBit;
	*address &= iBit;
}
/*
=============================================================================
*/
/* INITIALIZE THE PERIPHERAL CLOCK */
void GPIO_Init(uint32_t port)
{
	uint32_t *pPort = (uint32_t *)(PCC_BASE_ADDRESS + 0x124 + 0x04 * port);
	BitMaskRevision(pPort, 30, SET_BIT);
}
/*
=============================================================================
*/
/* GPIO SET MODE FUNCTION */
void GPIO_SetMode(uint32_t port, uint8_t pin, uint8_t mode, uint8_t direct, uint8_t IRQ)
{
	/* Declare register address */ 
	uint32_t *PCRx;
	uint32_t GPIOx;
	uint32_t *PDDR;
	
	/* Pin MODE by MUX 
	Alternative (0..7)
	000 Pin disabled (Alternative 0) (analog)
	001 Alternative 1 (GPIO)
	010 Alternative 2 (chip-specific)
	011 Alternative 3 (chip-specific)
	100 Alternative 4 (chip-specific)
	101 Alternative 5 (chip-specific)
	110 Alternative 6 (chip-specific)
	111 Alternative 7 (chip-specific)
	*/
	PCRx = (uint32_t *)(PORTA_BASE_ADDRESS + 0x1000 * port + 4 * pin);
	*PCRx |= (uint32_t)(mode << 8); /* Selecting mode by MUX */
	
	/* GPIOx address [A..E] */ 
	GPIOx = GPIO_BASE_ADDRESS + 0x40 * port;
	
	/* GPIO_PDDR */
	PDDR = (uint32_t *)(GPIOx + PDDR_OFFSET_ADDRESS);
	BitMaskRevision(PDDR, pin, direct);
	
	/* Check interrupt request (0..15)
	0000 Interrupt Status Flag (ISF) is disabled
	0001 ISF flag and DMA request on rising edge
	0010 ISF flag and DMA request on falling edge
	0011 ISF flag and DMA request on either edge
	0100 Reserved
	0101 Reserved
	0110 Reserved
	0111 Reserved
	1000 ISF flag and Interrupt when logic 0
	1001 ISF flag and Interrupt on rising-edge
	1010 ISF flag and Interrupt on falling-edge
	1011 ISF flag and Interrupt on either edge
	1100 ISF flag and Interrupt when logic 1
	1101 Reserved
	1110 Reserved
	1111 Reserved
	*/
	*PCRx |= (uint32_t)(IRQ << 16);
}
/*
=============================================================================
*/
void GPIO_WriteChannel(uint32_t port, uint8_t pin, uint8_t level)
{
	/* Declare register address */ 
	uint32_t GPIOx;
	uint32_t *PDDR;
	uint32_t *PDOR;
	
	/* GPIOx address [A..E] */ 
	GPIOx = GPIO_BASE_ADDRESS + 0x40 * port;
	
	/* GPIO_PDDR */
	PDDR = (uint32_t *)(GPIOx + PDDR_OFFSET_ADDRESS);
	
	/* Validate pin configured as output */
	if ((*PDDR & (0x01 << pin)) >> pin)
	{
		PDOR = (uint32_t *)GPIOx + 0x00;
		BitMaskRevision(PDOR, pin, level);
	}
}
/*
=============================================================================
*/
uint8_t GPIO_ReadChannel(uint32_t port, uint8_t pin)
{
	/* Declare register address */ 
	uint32_t GPIOx;
	uint32_t *PDDR;
	uint32_t *PDIR;
	
	/* GPIOx address [A..E] */ 
	GPIOx = GPIO_BASE_ADDRESS + 0x40 * port;
	
	/* GPIO_PDDR */
	PDDR = (uint32_t *)(GPIOx + PDDR_OFFSET_ADDRESS);
	
	/* Validate pin configured as input */
	if ((*PDDR & (0x01 << pin)) >> pin == 0)
	{
		PDIR = (uint32_t *)GPIOx + 0x10;
		return (uint8_t)((*PDIR & (0x01 << pin)) >> pin);
	}
	
	else
	{
		return 255;
	}
}
/*
=============================================================================
*/
void SysTick_Init(void)
{
	BitMaskRevision(SysTick_CTRL, 0, CLEAR_BIT); /* Disable SysTick timer */
	BitMaskRevision(SysTick_CTRL, 1, SET_BIT); /* Enable SysTick Interrupt timer */
	BitMaskRevision(SysTick_CTRL, 2, SET_BIT); /* Processor clock source at 48 MHz */ 

	*SysTick_LOAD |= 48000000/1000 - 1; /* Count 47999...0 ~ 1 ms */
	*SysTick_VAL &= 0x00; /* Clear for initial value */
	
	BitMaskRevision(SysTick_CTRL, 0, SET_BIT); /* Enable SysTick timer */
}
/*
=============================================================================
*/
void SysTick_Handler(void) 
{
  if(g_Count != 0)
		g_Count--;
}
/*
=============================================================================
*/
void SysTickDelay(uint32_t Milisecond)
{
	g_Count = Milisecond;
	
	while(1)
	{
		if(g_Count == 0)
			return;	
	}
}
