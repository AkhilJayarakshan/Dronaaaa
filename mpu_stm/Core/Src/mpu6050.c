/*
 * mpu6050.c
 *
 *  Created on: 21-Apr-2026
 *      Author: akhil
 */


#include "mpu6050.h"
#include "i2c.h"

#define MPU6050_ADDR (0x68 << 1)

extern I2C_HandleTypeDef hi2c1;

// ---------------- INIT ----------------
void MPU6050_Init(void)
{
    uint8_t data;

    // Wake up device
    data = 0x00;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x6B, 1, &data, 1, 100);

    // Set gyro config (±250 deg/s)
    data = 0x00;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x1B, 1, &data, 1, 100);

    // Set accel config (±2g)
    data = 0x00;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x1C, 1, &data, 1, 100);

    // Enable data ready interrupt
    data = 0x01;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x38, 1, &data, 1, 100);
}

IMU_Data_t MPU6050_Read_All(void)
{
    IMU_Data_t data;
    uint8_t buffer[14];

    HAL_I2C_Mem_Read(&hi2c1, (0x68 << 1), 0x3B, 1, buffer, 14, 100);

    data.accel_x = (uint16_t)(buffer[0] << 8 | buffer[1]);
    data.accel_y = (uint16_t)(buffer[2] << 8 | buffer[3]);
    data.accel_z = (uint16_t)(buffer[4] << 8 | buffer[5]);

    data.gyro_x  = (uint16_t)(buffer[8] << 8 | buffer[9]);
    data.gyro_y  = (uint16_t)(buffer[10] << 8 | buffer[11]);
    data.gyro_z  = (uint16_t)(buffer[12] << 8 | buffer[13]);

    return data;
}
