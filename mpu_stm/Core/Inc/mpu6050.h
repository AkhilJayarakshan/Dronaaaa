/*
 * mpu6050.h
 *
 *  Created on: 21-Apr-2026
 *      Author: akhil
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "stdint.h"
#include "cmsis_os.h"

typedef struct
{
    uint16_t accel_x;
    uint16_t accel_y;
    uint16_t accel_z;

    uint16_t gyro_x;
    uint16_t gyro_y;
    uint16_t gyro_z;

} IMU_Data_t;

void MPU6050_Init(void);
IMU_Data_t MPU6050_Read_All(void);

#endif /* INC_MPU6050_H_ */
