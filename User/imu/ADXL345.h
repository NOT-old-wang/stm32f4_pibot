// I2Cdev library collection - ADXL345 I2C device class header file
// Based on Analog Devices ADXL345 datasheet rev. C, 5/2011
// 7/31/2011 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//     2011-07-31 - initial release

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2011 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#ifndef _ADXL345_H_
#define _ADXL345_H_


#define ADXL345_ADDRESS_ALT_LOW     0x53 // alt address pin low (GND)
#define ADXL345_ADDRESS_ALT_HIGH    0x1D // alt address pin high (VCC)
#define ADXL345_DEFAULT_ADDRESS     ADXL345_ADDRESS_ALT_LOW

#define ADXL345_RA_DEVID            0x00
#define ADXL345_RA_RESERVED1        0x01
#define ADXL345_RA_THRESH_TAP       0x1D
#define ADXL345_RA_OFSX             0x1E
#define ADXL345_RA_OFSY             0x1F
#define ADXL345_RA_OFSZ             0x20
#define ADXL345_RA_DUR              0x21
#define ADXL345_RA_LATENT           0x22
#define ADXL345_RA_WINDOW           0x23
#define ADXL345_RA_THRESH_ACT       0x24
#define ADXL345_RA_THRESH_INACT     0x25
#define ADXL345_RA_TIME_INACT       0x26
#define ADXL345_RA_ACT_INACT_CTL    0x27
#define ADXL345_RA_THRESH_FF        0x28
#define ADXL345_RA_TIME_FF          0x29
#define ADXL345_RA_TAP_AXES         0x2A
#define ADXL345_RA_ACT_TAP_STATUS   0x2B
#define ADXL345_RA_BW_RATE          0x2C
#define ADXL345_RA_POWER_CTL        0x2D
#define ADXL345_RA_INT_ENABLE       0x2E
#define ADXL345_RA_INT_MAP          0x2F
#define ADXL345_RA_INT_SOURCE       0x30
#define ADXL345_RA_DATA_FORMAT      0x31
#define ADXL345_RA_DATAX0           0x32
#define ADXL345_RA_DATAX1           0x33
#define ADXL345_RA_DATAY0           0x34
#define ADXL345_RA_DATAY1           0x35
#define ADXL345_RA_DATAZ0           0x36
#define ADXL345_RA_DATAZ1           0x37
#define ADXL345_RA_FIFO_CTL         0x38
#define ADXL345_RA_FIFO_STATUS      0x39

#define ADXL345_AIC_ACT_AC_BIT      7
#define ADXL345_AIC_ACT_X_BIT       6
#define ADXL345_AIC_ACT_Y_BIT       5
#define ADXL345_AIC_ACT_Z_BIT       4
#define ADXL345_AIC_INACT_AC_BIT    3
#define ADXL345_AIC_INACT_X_BIT     2
#define ADXL345_AIC_INACT_Y_BIT     1
#define ADXL345_AIC_INACT_Z_BIT     0

#define ADXL345_TAPAXIS_SUP_BIT     3
#define ADXL345_TAPAXIS_X_BIT       2
#define ADXL345_TAPAXIS_Y_BIT       1
#define ADXL345_TAPAXIS_Z_BIT       0

#define ADXL345_TAPSTAT_ACTX_BIT    6
#define ADXL345_TAPSTAT_ACTY_BIT    5
#define ADXL345_TAPSTAT_ACTZ_BIT    4
#define ADXL345_TAPSTAT_ASLEEP_BIT  3
#define ADXL345_TAPSTAT_TAPX_BIT    2
#define ADXL345_TAPSTAT_TAPY_BIT    1
#define ADXL345_TAPSTAT_TAPZ_BIT    0

#define ADXL345_BW_LOWPOWER_BIT     4
#define ADXL345_BW_RATE_BIT         3
#define ADXL345_BW_RATE_LENGTH      4

#define ADXL345_RATE_3200           15
#define ADXL345_RATE_1600           14
#define ADXL345_RATE_800            13
#define ADXL345_RATE_400            12
#define ADXL345_RATE_200            11
#define ADXL345_RATE_100            10
#define ADXL345_RATE_50             9
#define ADXL345_RATE_25             8
#define ADXL345_RATE_12P5           7
#define ADXL345_RATE_6P25           6
#define ADXL345_RATE_3P13           5
#define ADXL345_RATE_1P56           4
#define ADXL345_RATE_0P78           3
#define ADXL345_RATE_0P39           2
#define ADXL345_RATE_0P20           1
#define ADXL345_RATE_0P10           0

#define ADXL345_PCTL_LINK_BIT       5
#define ADXL345_PCTL_AUTOSLEEP_BIT  4
#define ADXL345_PCTL_MEASURE_BIT    3
#define ADXL345_PCTL_SLEEP_BIT      2
#define ADXL345_PCTL_WAKEUP_BIT     1
#define ADXL345_PCTL_WAKEUP_LENGTH  2

#define ADXL345_WAKEUP_8HZ          0b00
#define ADXL345_WAKEUP_4HZ          0b01
#define ADXL345_WAKEUP_2HZ          0b10
#define ADXL345_WAKEUP_1HZ          0b11

#define ADXL345_INT_DATA_READY_BIT  7
#define ADXL345_INT_SINGLE_TAP_BIT  6
#define ADXL345_INT_DOUBLE_TAP_BIT  5
#define ADXL345_INT_ACTIVITY_BIT    4
#define ADXL345_INT_INACTIVITY_BIT  3
#define ADXL345_INT_FREE_FALL_BIT   2
#define ADXL345_INT_WATERMARK_BIT   1
#define ADXL345_INT_OVERRUN_BIT     0

#define ADXL345_FORMAT_SELFTEST_BIT 7
#define ADXL345_FORMAT_SPIMODE_BIT  6
#define ADXL345_FORMAT_INTMODE_BIT  5
#define ADXL345_FORMAT_FULL_RES_BIT 3
#define ADXL345_FORMAT_JUSTIFY_BIT  2
#define ADXL345_FORMAT_RANGE_BIT    1
#define ADXL345_FORMAT_RANGE_LENGTH 2

#define ADXL345_RANGE_2G            0
#define ADXL345_RANGE_4G            1
#define ADXL345_RANGE_8G            2
#define ADXL345_RANGE_16G           3

#define ADXL345_FIFO_MODE_BIT       7
#define ADXL345_FIFO_MODE_LENGTH    2
#define ADXL345_FIFO_TRIGGER_BIT    5
#define ADXL345_FIFO_SAMPLES_BIT    4
#define ADXL345_FIFO_SAMPLES_LENGTH 5

#define ADXL345_FIFO_MODE_BYPASS    0
#define ADXL345_FIFO_MODE_FIFO      1
#define ADXL345_FIFO_MODE_STREAM    2
#define ADXL345_FIFO_MODE_TRIGGER   3

#define ADXL345_FIFOSTAT_TRIGGER_BIT        7
#define ADXL345_FIFOSTAT_LENGTH_BIT         5
#define ADXL345_FIFOSTAT_LENGTH_LENGTH      6

class ADXL345 {
    public:
        ADXL345();
        ADXL345(unsigned char address);

        void initialize();
        bool testConnection();

        // DEVID register
        unsigned char getDeviceID();
        
        // THRESH_TAP register
        unsigned char getTapThreshold();
        void setTapThreshold(unsigned char threshold);

        // OFS* registers
        void getOffset(char* x, char* y, char* z);
        void setOffset(char x, char y, char z);
        char getOffsetX();
        void setOffsetX(char x);
        char getOffsetY();
        void setOffsetY(char y);
        char getOffsetZ();
        void setOffsetZ(char z);
        
        // DUR register
        unsigned char getTapDuration();
        void setTapDuration(unsigned char duration);
        
        // LATENT register
        unsigned char getDoubleTapLatency();
        void setDoubleTapLatency(unsigned char latency);
        
        // WINDOW register
        unsigned char getDoubleTapWindow();
        void setDoubleTapWindow(unsigned char window);
        
        // THRESH_ACT register
        unsigned char getActivityThreshold();
        void setActivityThreshold(unsigned char threshold);
        
        // THRESH_INACT register
        unsigned char getInactivityThreshold();
        void setInactivityThreshold(unsigned char threshold);

        // TIME_INACT register
        unsigned char getInactivityTime();
        void setInactivityTime(unsigned char time);
        
        // ACT_INACT_CTL register
        bool getActivityAC();
        void setActivityAC(bool enabled);
        bool getActivityXEnabled();
        void setActivityXEnabled(bool enabled);
        bool getActivityYEnabled();
        void setActivityYEnabled(bool enabled);
        bool getActivityZEnabled();
        void setActivityZEnabled(bool enabled);
        bool getInactivityAC();
        void setInactivityAC(bool enabled);
        bool getInactivityXEnabled();
        void setInactivityXEnabled(bool enabled);
        bool getInactivityYEnabled();
        void setInactivityYEnabled(bool enabled);
        bool getInactivityZEnabled();
        void setInactivityZEnabled(bool enabled);
        
        // THRESH_FF register
        unsigned char getFreefallThreshold();
        void setFreefallThreshold(unsigned char threshold);
        
        // TIME_FF register
        unsigned char getFreefallTime();
        void setFreefallTime(unsigned char time);
        
        // TAP_AXES register
        bool getTapAxisSuppress();
        void setTapAxisSuppress(bool enabled);
        bool getTapAxisXEnabled();
        void setTapAxisXEnabled(bool enabled);
        bool getTapAxisYEnabled();
        void setTapAxisYEnabled(bool enabled);
        bool getTapAxisZEnabled();
        void setTapAxisZEnabled(bool enabled);
        
        // ACT_TAP_STATUS register
        bool getActivitySourceX();
        bool getActivitySourceY();
        bool getActivitySourceZ();
        bool getAsleep();
        bool getTapSourceX();
        bool getTapSourceY();
        bool getTapSourceZ();
        
        // BW_RATE register
        bool getLowPowerEnabled();
        void setLowPowerEnabled(bool enabled);
        unsigned char getRate();
        void setRate(unsigned char rate);

        // POWER_CTL register
        bool getLinkEnabled();
        void setLinkEnabled(bool enabled);
        bool getAutoSleepEnabled();
        void setAutoSleepEnabled(bool enabled);
        bool getMeasureEnabled();
        void setMeasureEnabled(bool enabled);
        bool getSleepEnabled();
        void setSleepEnabled(bool enabled);
        unsigned char getWakeupFrequency();
        void setWakeupFrequency(unsigned char frequency);
        
        // INT_ENABLE register
        bool getIntDataReadyEnabled();
        void setIntDataReadyEnabled(bool enabled);
        bool getIntSingleTapEnabled();
        void setIntSingleTapEnabled(bool enabled);
        bool getIntDoubleTapEnabled();
        void setIntDoubleTapEnabled(bool enabled);
        bool getIntActivityEnabled();
        void setIntActivityEnabled(bool enabled);
        bool getIntInactivityEnabled();
        void setIntInactivityEnabled(bool enabled);
        bool getIntFreefallEnabled();
        void setIntFreefallEnabled(bool enabled);
        bool getIntWatermarkEnabled();
        void setIntWatermarkEnabled(bool enabled);
        bool getIntOverrunEnabled();
        void setIntOverrunEnabled(bool enabled);
        
        // INT_MAP register
        unsigned char getIntDataReadyPin();
        void setIntDataReadyPin(unsigned char pin);
        unsigned char getIntSingleTapPin();
        void setIntSingleTapPin(unsigned char pin);
        unsigned char getIntDoubleTapPin();
        void setIntDoubleTapPin(unsigned char pin);
        unsigned char getIntActivityPin();
        void setIntActivityPin(unsigned char pin);
        unsigned char getIntInactivityPin();
        void setIntInactivityPin(unsigned char pin);
        unsigned char getIntFreefallPin();
        void setIntFreefallPin(unsigned char pin);
        unsigned char getIntWatermarkPin();
        void setIntWatermarkPin(unsigned char pin);
        unsigned char getIntOverrunPin();
        void setIntOverrunPin(unsigned char pin);

        // INT_SOURCE register
        unsigned char getIntDataReadySource();
        unsigned char getIntSingleTapSource();
        unsigned char getIntDoubleTapSource();
        unsigned char getIntActivitySource();
        unsigned char getIntInactivitySource();
        unsigned char getIntFreefallSource();
        unsigned char getIntWatermarkSource();
        unsigned char getIntOverrunSource();
        
        // DATA_FORMAT register
        unsigned char getSelfTestEnabled();
        void setSelfTestEnabled(unsigned char enabled);
        unsigned char getSPIMode();
        void setSPIMode(unsigned char mode);
        unsigned char getInterruptMode();
        void setInterruptMode(unsigned char mode);
        unsigned char getFullResolution();
        void setFullResolution(unsigned char resolution);
        unsigned char getDataJustification();
        void setDataJustification(unsigned char justification);
        unsigned char getRange();
        void setRange(unsigned char range);

        // DATA* registers
        void getAcceleration(short* x, short* y, short* z);
        short getAccelerationX();
        short getAccelerationY();
        short getAccelerationZ();

        // FIFO_CTL register
        unsigned char getFIFOMode();
        void setFIFOMode(unsigned char mode);
        unsigned char getFIFOTriggerInterruptPin();
        void setFIFOTriggerInterruptPin(unsigned char interrupt);
        unsigned char getFIFOSamples();
        void setFIFOSamples(unsigned char size);
        
        // FIFO_STATUS register
        bool getFIFOTriggerOccurred();
        unsigned char getFIFOLength();

    private:
        unsigned char devAddr;
        unsigned char buffer[6];
};

#endif /* _ADXL345_H_ */
