#include "MyGPIO.h"
/*
=============================================================================
*													TEST FUNCTIONS
=============================================================================
*/
int main(void)
{
	GPIO_Init(PORTD);
	GPIO_SetMode(PORTD, PIN0, MODE1, OUTPUT, IRQ_DIS);	
	SysTick_Init(); /* For delay function */

	/* Blink LED testing */
	while(1)
	{
		GPIO_WriteChannel(PORTD, PIN0, HIGH);
		SysTickDelay(500);
		GPIO_WriteChannel(PORTD, PIN0, LOW);
		SysTickDelay(500);
	}
}
