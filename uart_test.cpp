/*
 ===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
 ===============================================================================
 */

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

// TODO: insert other include files here
#include "systick.h"
#include "lcd_port.h"
#include "LiquidCrystal.h"
#include "BarGraph.h"
#include "SimpleMenu.h"
#include "MenuItem.h"
#include "IntegerEdit.h"
#include "DigitalIoPin.h"
#include "DebouncedInput.h"
#include "PinEvent.h"
#include "Timer.h"
#include "I2C.h"
#include "ModbusMaster.h"
#include "SensorEdit.h"
#include "AutoEdit.h"
// TODO: insert other definitions and declarations here

#include <cstring>
#include <cstdio>
#include <cmath>
/*static volatile unsigned int counter;
 static volatile unsigned int systicks;
 static volatile bool heartbeatFlag = false;*/

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/*
 #ifdef __cplusplus
 extern "C" {
 #endif

 void SysTick_Handler(void) {
 systicks++;
 if (counter > 0)
 counter--;
 if ((systicks % 1000) == 0){
 heartbeatFlag = true;
 }
 }
 #ifdef __cplusplus
 }
 #endif

 void Sleep(int ms) {
 counter = ms;
 while (counter > 0) {
 __WFI();
 }
 }
 */

/* this function is required by the modbus library */
uint32_t millis() {
	return GetSysticks();
//	return systicks;
}

void printRegister(ModbusMaster& node, uint16_t reg) {
	uint8_t result;
	// slave: read 16-bit registers starting at reg to RX buffer
	result = node.readHoldingRegisters(reg, 1);

	// do something with data if read is successful
	if (result == node.ku8MBSuccess) {
		printf("R%d=%04X\n", reg, node.getResponseBuffer(0));
	} else {
		printf("R%d=???\n", reg);
	}
}

bool setFrequency(ModbusMaster& node, uint16_t freq) {
	uint8_t result;
	int ctr;
	bool atSetpoint;
	const int delay = 500;

	node.writeSingleRegister(1, freq); // set motor frequency

	printf("Set freq = %d\n", freq / 40); // for debugging

	// wait until we reach set point or timeout occurs
	ctr = 0;
	atSetpoint = false;
	do {
		Sleep(delay);
		// read status word
		result = node.readHoldingRegisters(3, 1);
		// check if we are at setpoint
		if (result == node.ku8MBSuccess) {
			if (node.getResponseBuffer(0) & 0x0100)
				atSetpoint = true;
		}
		ctr++;
	} while (ctr < 20 && !atSetpoint);

	printf("Elapsed: %d\n", ctr * delay); // for debugging

	return atSetpoint;
}

int16_t getHumidity(I2C i2c) {
	uint8_t sensorData[4];
	uint8_t readCmd = 0xF1;
	int16_t humidityData = 0;
	int16_t statusbits = 0;
	int16_t humidity = 0;

	if (i2c.transaction(0x27, &readCmd, 1, sensorData, 4)) {
		/* Output temperature. */
		humidityData = (sensorData[0] << 8) | sensorData[1];
		statusbits = humidityData >> 14;
		humidityData = humidityData - (statusbits << 14);

		humidity = (humidityData / (pow(2.0, 14.0) - 2) * 100);
		return humidity;
	}
}

int16_t getTemparature(I2C i2c) {
	uint8_t sensorData[4];
	uint8_t readCmd = 0xF1;
	int16_t temparatureData = 0;
	int16_t temparature = 0;

	if (i2c.transaction(0x27, &readCmd, 1, sensorData, 4)) {
		/* Output temperature. */
		temparatureData = (sensorData[2] << 6) | (sensorData[3] >> 2);

		temparature = (temparatureData / (pow(2.0, 14.0) - 2) * 165 - 40);
		return temparature;
	}
}

/**
 * @brief	Main UART program body
 * @return	Always returns 1
 */
int main(void) {

#if defined (__USE_LPCOPEN)
	// Read clock settings and update SystemCoreClock variable
	SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
	// Set up and initialize all required blocks and
	// functions related to the board hardware
	Board_Init();
	// Set the LED to the state of "On"
	Board_LED_Set(0, true);
#endif
#endif

	int lastF;
	/* Set up SWO to PIO1_2 */
	Chip_SWM_MovablePortPinAssign(SWM_SWO_O, 1, 2); // Needed for SWO printf

	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(Chip_Clock_GetSysTickClockRate() / 1000);

	Chip_RIT_Init(LPC_RITIMER);
	NVIC_EnableIRQ(RITIMER_IRQn);

	Board_LED_Set(0, false);
	Board_LED_Set(1, true);
	printf("Started\n");

	LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

	DebouncedInput ok(4, 20), back(5, 20), up(6, 20), down(7, 20);
	PinEvent okEV(ok), backEV(back), upEV(up), downEV(down);

	lcd.begin(16, 2);
	lcd.setCursor(0, 0);
	BarGraph bg50px(lcd, 50);

	I2C i2c(0, 1000);

	SimpleMenu menu;
	IntegerEdit frequency(lcd, std::string("Frequency"), 0, 500, bg50px);
	SensorEdit humidity(lcd, std::string("Humidity"), 0, 100, bg50px);
	SensorEdit temparature(lcd, std::string("Temperature"), -40, 160, bg50px);
	AutoEdit automation(lcd, std::string("Automatic Humi"), true, false);
	AutoEdit automationTemp(lcd, std::string("Automatic Temp"), false, true);

	menu.addItem(new MenuItem(frequency));
	menu.addItem(new MenuItem(humidity));
	menu.addItem(new MenuItem(temparature));
//	menu.addItem(new MenuItem(frequency));
	menu.addItem(new MenuItem(automation));
	menu.addItem(new MenuItem(automationTemp));

	frequency.setValue(0);
	humidity.setValue(getHumidity(i2c));
	temparature.setValue(getTemparature(i2c));
	automation.setHValue(getHumidity(i2c));
	automation.setFValue(0);
	automationTemp.setHValue(getTemparature(i2c));
		automationTemp.setFValue(0);

	menu.event(MenuItem::show);

	ModbusMaster node(2); // Create modbus object that connects to slave id 2

	node.begin(9600); // set transmission rate - other parameters are set inside the object and can't be changed here

	printRegister(node, 3); // for debugging

	node.writeSingleRegister(0, 0x0406); // prepare for starting

	printRegister(node, 3); // for debugging

	Sleep(1000); // give converter some time to set up
	// note: we should have a startup state machine that check converter status and acts per current status
	//       but we take the easy way out and just wait a while and hope that everything goes well

	printRegister(node, 3); // for debugging

	node.writeSingleRegister(0, 0x047F); // set drive to start mode

	printRegister(node, 3); // for debugging

	Sleep(1000); // give converter some time to set up
	// note: we should have a startup state machine that check converter status and acts per current status
	//       but we take the easy way out and just wait a while and hope that everything goes well
	setFrequency(node, 0);
	printRegister(node, 3); // for debugging

	int i = 0;
	int j = 0;
	const uint16_t fa[20] = { 1000, 2000, 3000, 3500, 4000, 5000, 7000, 8000,
			8300, 10000, 10000, 9000, 8000, 7000, 6000, 5000, 4000, 3000, 2000,
			1000 };

	Timer timer1(0, 3, 0, true);
	while (1) {
//		printf("%d \n", GetSysticks());
//		printf("Start the while loop \n");
		__WFI();
//		if (timer1.Tick()) {
//		if ((GetSysticks() % 2000) == 0) {
		if (GetComFlag()) {
			SetComFlag(false);
			printf("Timer ends \n");
			uint8_t result;

			// slave: read (2) 16-bit registers starting at register 102 to RX buffer
			j = 0;
			do {
				result = node.readHoldingRegisters(102, 2);
				j++;
			} while (j < 3 && result != node.ku8MBSuccess);
			// note: sometimes we don't succeed on first read so we try up to threee times
			// if read is successful print frequency and current (scaled values)
			if (result == node.ku8MBSuccess) {
				printf("F=%4d, I=%4d  (ctr=%d)\n", node.getResponseBuffer(0),
						node.getResponseBuffer(1), j);
			} else {
				printf("ctr=%d\n", j);
			}
		}

		if (menu.isSensor()) {
//			printf("getting new value \n");
//			if ((GetSysticks() % 500) == 0){
//			printf("getting new value \n");
//			setFrequency(node, lastF * 40);
			humidity.setValue(getHumidity(i2c));
			temparature.setValue(getTemparature(i2c));
			menu.event(MenuItem::show);
//			}
			Sleep(100);
		}

		if (menu.isAuto() && menu.GetFocus()) {
			automation.setHValue(getHumidity(i2c));
			//menu.event(MenuItem::show);
			if (GetAutoFlag()) {
				SetAutoFlag(false);
//			automation.setHValue(getHumidity(i2c));
				int diff = getHumidity(i2c) - menu.getSetValue();
				if (diff > 0) {
					automation.setFValue(diff * 2);
					setFrequency(node, (diff * 2) * 40);
					lastF = diff * 2;
				} else {
					automation.setFValue(0);
					setFrequency(node, 0);
				}
				//menu.event(MenuItem::show);
			}
			menu.event(MenuItem::show);
			Sleep(100);
		}

		if (menu.isAutoT() && menu.GetFocus()) {
					automationTemp.setHValue(getTemparature(i2c));
					//menu.event(MenuItem::show);
					if (GetAutoFlag()) {
						SetAutoFlag(false);
		//			automation.setHValue(getHumidity(i2c));
						int diff = getTemparature(i2c) - menu.getSetValue();
						if (diff > 0) {
							automationTemp.setFValue(diff * 2);
							setFrequency(node, (diff * 20) * 40);
							lastF = diff * 2;
						} else {
							automationTemp.setFValue(0);
							setFrequency(node, 0);
						}
						//menu.event(MenuItem::show);
					}
					menu.event(MenuItem::show);
					Sleep(100);
				}

		if (okEV.event()) {
			printf("OK \n");
			menu.event(MenuItem::ok);
			if ((menu.flag) && (!menu.isSensor()) && (!menu.isAuto())) {
//			if (menu.flag){
				setFrequency(node, menu.getValue() * 40);
				lastF = menu.getValue();
			}
		} else if (backEV.event()) {
			printf("BACK \n");
			menu.event(MenuItem::back);
		} else if (up.read()) {
			printf("UP \n");
			menu.event(MenuItem::up);
			Sleep(100);
		} else if (down.read()) {
			printf("DOWN \n");
			menu.event(MenuItem::down);
			Sleep(100);
		}

		// frequency is scaled:
		// 20000 = 50 Hz, 0 = 0 Hz, linear scale 400 units/Hz

	}

	return 1;
}

