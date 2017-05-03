/*
 * lcd_port.cpp
 *
 *  Created on: 26.1.2016
 *      Author: krl
 */

/* compatibility layer */
#include "board.h"
#include "lcd_port.h"
#include "systick.h"

static volatile bool RIT_flag;

extern "C" {
void RIT_IRQHandler(void) {
	// todo: implement RIT ISR that signals main task that timer has expired

	// Acknowledge interrupt by clearing RIT interrupt flag
	Chip_RIT_ClearIntStatus(LPC_RITIMER);
	// disable RIT – we want a single interrupt after the wait is over
	Chip_RIT_Disable(LPC_RITIMER);
	// set a flag to notify main program
	RIT_flag = true;

}
}

static const int dPort[] = { 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0 };
static const int dPin[] = { 10, 9, 29, 9, 10, 16, 3, 0, 24, 0, 27, 28, 12, 14 };

void digitalWrite(uint8_t pin, uint8_t val) {
	if (pin > 13)
		return;
	Chip_GPIO_SetPinState(LPC_GPIO, dPort[pin], dPin[pin], val);
}

void pinMode(uint8_t pin, uint8_t mode) {
	if (mode == OUTPUT) {
		Chip_IOCON_PinMuxSet(LPC_IOCON, dPort[pin], dPin[pin],
				(IOCON_DIGMODE_EN));
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, dPort[pin], dPin[pin]);
	} else {
		Chip_IOCON_PinMuxSet(LPC_IOCON, dPort[pin], dPin[pin],
				(IOCON_MODE_PULLUP | IOCON_DIGMODE_EN));
		Chip_GPIO_SetPinDIRInput(LPC_GPIO, dPort[pin], dPin[pin]);
	}
}

void delayMicroseconds(int us) {
	// todo: implement accurate waiting using RIT-timer

	// calculate compare value
	uint64_t cmp_value = (uint64_t) Chip_Clock_GetSystemClockRate();
	cmp_value = cmp_value * (uint64_t) us / 1000000;
	// disable RIT – compare value may only be changed when RIT is disabled
	Chip_RIT_Disable(LPC_RITIMER);
	//clear wait flag
	RIT_flag = false;
	//set compare value
	Chip_RIT_SetCompareValue(LPC_RITIMER, cmp_value);
	//clear RIT counter
	Chip_RIT_SetCounter(LPC_RITIMER, 0);
	//enable RIT
	Chip_RIT_Enable(LPC_RITIMER);
	//wait flag
	while (!RIT_flag) {
		__WFI();
	}

	/*
	 // simple version that rounds up to nearest ms
	 int ms = us / 1000;
	 if(us % 1000 != 0) ms++;
	 Sleep(ms); // use systick timer at 1000 Hz for this
	 // you need to implement Sleep() - hint: adapt it from the earli
	 */
}
