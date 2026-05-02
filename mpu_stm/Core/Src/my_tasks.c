/*
 * tasks.c
 *
 *  Created on: 21-Apr-2026
 *      Author: akhil
 */

#include "my_tasks.h"
#include "imu.h"

TaskHandle_t imuTaskHandle = NULL;

void createTask()
{
	xTaskCreate(IMU_Task,"IMU",256,NULL,3,&imuTaskHandle);
}
