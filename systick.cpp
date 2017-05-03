/*
 * systick.cpp
 *
 *  Created on: 31.1.2016
 *      Author: krl
 */
#include "systick.h"
#include "chip.h"

static volatile unsigned int counter;
static volatile unsigned int systicks;
static volatile bool comFlag = false;
static volatile bool autoFlag = false;

/*
 * Note: Systick handler must be declared with C-linkage otherwise the linker
 * can't set the interrupt vectors correctly
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
void SysTick_Handler(void)
{
	systicks++;
	if ((systicks % 2000) == 0){
		comFlag = true;
	}
	if ((systicks % 10000) == 0){
			autoFlag = true;
		}
	if(counter > 0) counter--;
}
#ifdef __cplusplus
}
#endif

unsigned int GetSysticks()
{
	return systicks;
}

bool GetComFlag(){
	return comFlag;
}

void SetComFlag(bool flag){
	comFlag = flag;
}

bool GetAutoFlag(){
	return autoFlag;
}

void SetAutoFlag(bool flag){
	autoFlag = flag;
}

void Sleep(unsigned int ms)
{
	counter = ms + 1; // add one to guarantee specified wait time
	while(counter > 0) {
		__WFI();
	}
}
