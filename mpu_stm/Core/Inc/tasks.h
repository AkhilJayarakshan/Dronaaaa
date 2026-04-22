/*
 * tasks.h
 *
 *  Created on: 21-Apr-2026
 *      Author: akhil
 */

#ifndef INC_TASKS_H_
#define INC_TASKS_H_

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"

extern TaskHandle_t imuTaskHandle;

void createTask();

#endif /* INC_TASKS_H_ */
