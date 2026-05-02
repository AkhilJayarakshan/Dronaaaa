/*
 * imu.c
 *
 *  Created on: 21-Apr-2026
 *      Author: akhil
 */
#include <my_tasks.h>
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"
#include "main.h"
#include "mpu6050.h"

IMU_Data_t imu_data = {0};
// ===== GLOBAL / STATIC =====
static float roll = 0, pitch = 0, yaw = 0;

static float gyro_bias_x = 0;
static float gyro_bias_y = 0;
static float gyro_bias_z = 0;

static uint8_t calibrated = 0;

// Optional smoothing
static float roll_f = 0, pitch_f = 0;

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
	MPU6050_Init();
    // Wait a bit for sensor to stabilize
    vTaskDelay(pdMS_TO_TICKS(500));

//    HAL_NVIC_DisableIRQ(EXTI0_IRQn);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
    for (;;)
    {
        // Wait for interrupt notification
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Read MPU6050 data
        imu_data=(IMU_Data_t)MPU6050_Read_All();
        // Convert raw
        // Convert raw → physical
        float Ax = imu_data.accel_x / 16384.0f;
        float Ay = imu_data.accel_y / 16384.0f;
        float Az = imu_data.accel_z / 16384.0f;

        // Apply gyro bias
        float Gx = (imu_data.gyro_x - gyro_bias_x) / 131.0f;
        float Gy = (imu_data.gyro_y - gyro_bias_y) / 131.0f;
        float Gz = (imu_data.gyro_z - gyro_bias_z) / 131.0f;

        // ===== TIME =====
        static uint32_t prevTick = 0;
        uint32_t now = xTaskGetTickCount();

        float dt = 0.0f;
        if (prevTick != 0)
        {
            dt = (now - prevTick) * portTICK_PERIOD_MS / 1000.0f;
        }
        prevTick = now;

        // ===== ACC ANGLES =====
        float rollAcc  = atan2f(Ay, Az) * 180.0f / 3.14159f;
        float pitchAcc = atan2f(-Ax, sqrtf(Ay*Ay + Az*Az)) * 180.0f / 3.14159f;

        // ===== COMPLEMENTARY FILTER =====
        float alpha = 0.99f;  // stronger filtering

        roll  = alpha * (roll  + Gx * dt) + (1.0f - alpha) * rollAcc;
        pitch = alpha * (pitch + Gy * dt) + (1.0f - alpha) * pitchAcc;

        // ===== YAW (gyro only) =====
        yaw += Gz * dt;

        // ===== OPTIONAL SMOOTHING (extra stability) =====
        float smooth = 0.9f;
        roll_f  = smooth * roll_f  + (1 - smooth) * roll;
        pitch_f = smooth * pitch_f + (1 - smooth) * pitch;


    }
}
