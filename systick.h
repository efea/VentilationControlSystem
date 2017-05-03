/*
 * systick.h
 *
 *  Created on: 31.1.2016
 *      Author: krl
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

bool GetComFlag();
void SetComFlag(bool flag);
bool GetAutoFlag();
void SetAutoFlag(bool flag);
/**
 * @brief   Read current systick count
 * @return  Systick count
 */
unsigned int GetSysticks();

/**
 * @brief   Wait for at least ms milliseconds
 * @return  Nothing
 */
void Sleep(unsigned int ms);

#endif /* SYSTICK_H_ */
