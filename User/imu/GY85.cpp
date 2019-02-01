#include "GY85.h"
#include "board.h"
#include <stdio.h>
#include "delay.h"
#define ADXL345_SCALE 25.60000
#define ITG3205_SCALE 0.00121414209  //rad/s
#define HMC5883L_SCALE 0.92  // mG/LSb

#define WRITE_INTERVAL 50

bool IMU::init(){
    Board::get()->i2c_init(IIC_GY85);


    printf("testConnection\r\n");

    if (!accel.testConnection()){
        printf("ADXL345 NOT FOUND!\r\n");
        return false;
    }

    printf("initialize\r\n");
    accel.initialize();
    
    delay_ms(WRITE_INTERVAL);
    accel.setAutoSleepEnabled(false);
    delay_ms(WRITE_INTERVAL);
    accel.setFullResolution(1);
    delay_ms(WRITE_INTERVAL);
    accel.setRange(ADXL345_RANGE_4G);
    delay_ms(WRITE_INTERVAL);
    accel.setRate(ADXL345_RATE_50);
    delay_ms(WRITE_INTERVAL);
    
#if IMU_DEBUG_ENABLE
    uint8_t buffer=0;
    Board::get()->i2c_read_byte(IIC_GY85, ADXL345_DEFAULT_ADDRESS<<1, ADXL345_RA_POWER_CTL, &buffer);
    printf("ADXL345_RA_POWER_CTL=%d\r\n", buffer);
    Board::get()->i2c_read_byte(IIC_GY85, ADXL345_DEFAULT_ADDRESS<<1, ADXL345_RA_DATA_FORMAT, &buffer);
    printf("ADXL345_RA_DATA_FORMAT=%d\r\n", buffer);
    Board::get()->i2c_read_byte(IIC_GY85, ADXL345_DEFAULT_ADDRESS<<1, ADXL345_RA_BW_RATE, &buffer);
    printf("ADXL345_RA_BW_RATE=%d\r\n", buffer);
#endif

    printf("ADXL345 completed!\r\n");

    if (!gyro.testConnection()){
        printf("ITG3205 NOT FOUND!\r\n");
        return false;
    }

    gyro.initialize();
    gyro.reset();
    delay_ms(WRITE_INTERVAL);
    gyro.setFullScaleRange(ITG3200_FULLSCALE_2000);
    delay_ms(WRITE_INTERVAL);
    gyro.setDLPFBandwidth(ITG3200_DLPF_BW_42);
    delay_ms(WRITE_INTERVAL);
    gyro.setRate(0x13);     //1khz/(1+0x13)  50hz
    delay_ms(WRITE_INTERVAL);
    gyro.setClockSource(ITG3200_CLOCK_PLL_ZGYRO);
    delay_ms(WRITE_INTERVAL);
    
#if IMU_DEBUG_ENABLE
    Board::get()->i2c_read_byte(IIC_GY85, ITG3200_DEFAULT_ADDRESS<<1, ITG3200_RA_PWR_MGM, &buffer);
    printf("ITG3200_RA_PWR_MGM=%d\r\n", buffer);
    Board::get()->i2c_read_byte(IIC_GY85, ITG3200_DEFAULT_ADDRESS<<1, ITG3200_RA_DLPF_FS, &buffer);
    printf("ITG3200_RA_DLPF_FS=%d\r\n", buffer);
    Board::get()->i2c_read_byte(IIC_GY85, ITG3200_DEFAULT_ADDRESS<<1, ITG3200_RA_SMPLRT_DIV, &buffer);
    printf("ITG3200_RA_SMPLRT_DIV=%d\r\n", buffer);
#endif

    if (!mag.testConnection()){
        printf("HMC5883L NOT FOUND!\r\n");
        return false;
    }

    mag.initialize();
    delay_ms(WRITE_INTERVAL);
    mag.setGain(HMC5883L_GAIN_1090);
    delay_ms(WRITE_INTERVAL);
    mag.setDataRate(HMC5883L_RATE_75);
    delay_ms(WRITE_INTERVAL);
    mag.setSampleAveraging(HMC5883L_AVERAGING_1);
    delay_ms(WRITE_INTERVAL);
    mag.setMode(HMC5883L_MODE_SINGLE);
    delay_ms(WRITE_INTERVAL);

#if IMU_DEBUG_ENABLE
    Board::get()->i2c_read_byte(IIC_GY85, HMC5883L_DEFAULT_ADDRESS<<1, HMC5883L_RA_CONFIG_B, &buffer);
    printf("HMC5883L_RA_CONFIG_B=%d\r\n", buffer);
    Board::get()->i2c_read_byte(IIC_GY85, HMC5883L_DEFAULT_ADDRESS<<1, HMC5883L_RA_CONFIG_A, &buffer);
    printf("HMC5883L_RA_CONFIG_A=%d\r\n", buffer);
    Board::get()->i2c_read_byte(IIC_GY85, HMC5883L_DEFAULT_ADDRESS<<1, HMC5883L_RA_MODE, &buffer);
    printf("HMC5883L_RA_MODE=%d\r\n", buffer);
#endif

    printf("IMU INIT SUCCESS!\r\n");

    return true;
}

void IMU::get_data(float imu_data[9]){
    accel.getAcceleration(&ax, &ay, &az);
    gyro.getRotation(&gx, &gy, &gz);
    mag.getHeading(&mx, &my, &mz);
    mag.setMode(HMC5883L_MODE_SINGLE);

#if IMU_DEBUG_ENABLE
    printf("[%d %d %d] [%d %d %d] [%d %d %d]\r\n", ax, ay, az, gx, gy, gz, mx, my, mz);
#endif

    imu_data[0] = ax/ADXL345_SCALE;
    imu_data[1] = ay/ADXL345_SCALE;
    imu_data[2] = az/ADXL345_SCALE;
    imu_data[3] = gx*ITG3205_SCALE;
    imu_data[4] = gy*ITG3205_SCALE;
    imu_data[5] = gz*ITG3205_SCALE;
    imu_data[6] = mx*HMC5883L_SCALE;
    imu_data[7] = my*HMC5883L_SCALE;
    imu_data[8] = mz*HMC5883L_SCALE;
}
