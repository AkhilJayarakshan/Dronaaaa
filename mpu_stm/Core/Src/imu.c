/*
 * imu.c
 *
 *  Created on: 21-Apr-2026
 *      Author: akhil
 */
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"
#include "tasks.h"
#include "main.h"
#include "mpu6050.h"

IMU_Data_t imu_data;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_0)   // EXTI0 (your INT pin)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        vTaskNotifyGiveFromISR(imuTaskHandle, &xHigherPriorityTaskWoken);

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
void IMU_Task(void *pvParameters)
{
    for (;;)
    {
        // Wait for interrupt notification
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Read MPU6050 data
        imu_data=(IMU_Data_t)MPU6050_Read_All();

    }
}
