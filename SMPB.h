#ifndef  _SMPB_H
#define _SMPB_H

#include <Wire.h>
#include <Arduino.h>


typedef signed int s32;
typedef unsigned char u8;
typedef signed char   s8;
typedef unsigned short u16;
typedef signed short s16;
typedef long unsigned int u32;

#define SMPB_ADDR        0x56

#define IO_SETUP_ADD     0xF5

#define IIR_CNT_ADD      0xF1
#define IIR_N_2          0x01
#define IIR_N_4          0x02
#define IIR_N_8          0x03
#define IIR_N_16         0x04
#define IIR_N_32         0x05

#define T_STANBY_1ms     0x00
#define T_STANBY_5ms     0x20
#define T_STANBY_50ms    0x40
#define T_STANBY_250ms   0x60
#define T_STANBY_500ms   0x80
#define T_STANBY_1s      0xA0
#define T_STANBY_2s      0xC0
#define T_STANBY_4s      0xE0

#define DEVICE_STAT_ADD  0xF3

#define RESET_ADD        0xE0
#define RESET_COMMAND    0xE6

#define COE_b00_a0_ex    0xB8
#define COE_a2_0         0xB7
#define COE_a2_1         0xB6
#define COE_a1_0         0xB5
#define COE_a1_1         0xB4
#define COE_a0_0         0xB3
#define COE_a0_1         0xB2
#define COE_bp3_0        0xB1
#define COE_bp3_1        0xB0
#define COE_b21_0        0xAF
#define COE_b21_1        0xAE
#define COE_b12_0        0xAD
#define COE_b12_1        0xAC
#define COE_bp2_0        0xAB
#define COE_bp2_1        0xAA
#define COE_b11_0        0xA9
#define COE_b11_1        0xA8
#define COE_bp1_0        0xA7
#define COE_bp1_1        0xA6
#define COE_bt2_0        0xA5
#define COE_bt2_1        0xA4
#define COE_bt1_0        0xA3
#define COE_bt1_1        0xA2
#define COE_b00_0        0xA1
#define COE_b00_1        0xA0

#define a1_A        -6.3E-03
#define a1_S         4.3E-04
#define a2_A        -1.9E-11
#define a2_S         1.2E-10
#define bt1_A        1.0E-01
#define bt1_S        9.1E-02
#define bt2_A        1.2E-08
#define bt2_S        1.2E-06
#define bp1_A        3.3E-02
#define bp1_S        1.9E-02
#define b11_A        2.1E-07
#define b11_S        1.4E-07
#define bp2_A       -6.3E-10
#define bp2_S        3.5E-10
#define b12_A        2.9E-13
#define b12_S        7.6E-13
#define b21_A        2.1E-15
#define b21_S        1.2E-14
#define bp3_A        1.3E-16
#define bp3_S        7.9E-17

/*Averaging times setting for Temperature/Pressure measurement
  Temp/Press_ave=1, 22bits output
  Temp/Press_ave=2, 22bits output
  Temp/Press_ave=4~64, 24bits output
  *************************************
  notice!!!
  This library USES 24-bit output. 
  For more details, please see datasheet.
*/
#define CTRL_MEAS_ADD       0xF4
#define TEMP_AVERAGE_SKIP   0x00
#define TEMP_AVERAGE_1      0x20
#define TEMP_AVERAGE_2      0x40
#define TEMP_AVERAGE_4      0x60
#define TEMP_AVERAGE_8      0x80
#define TEMP_AVERAGE_16     0xA0
#define TEMP_AVERAGE_32     0xC0
#define TEMP_AVERAGE_64     0xE0
#define PRESS_AVERAGE_SKIP  0x00
#define PRESS_AVERAGE_1     0x04
#define PRESS_AVERAGE_2     0x08
#define PRESS_AVERAGE_4     0x0C
#define PRESS_AVERAGE_8     0x10
#define PRESS_AVERAGE_16    0x14
#define PRESS_AVERAGE_32    0x18
#define PRESS_AVERAGE_64    0x1C
#define SLEEP_MODE          0x00
#define FORCED_MODE         0x01
#define FORCED_MODE         0x02
#define NORMAL_MODE         0x03

#define TEMP_TXD0_ADD       0xFC
#define TEMP_TXD1_ADD       0xFB
#define TEMP_TXD2_ADD       0xFA
#define PRESS_TXD0_ADD      0xF9
#define PRESS_TXD1_ADD      0xF8
#define PRESS_TXD2_ADD      0xF7


class IIC_OPRTS
{
    public:
    void iic_read_byte(u8 reg_addr,u8 *byte);
    s32 iic_write_byte(u8 set_addr,u8 set_byte);
    u8 _IIC_ADDR;
};

class SMPB:public IIC_OPRTS
{
    public:
    SMPB(u8 ADDR=SMPB_ADDR);
    ~SMPB(){};
    void init(s32 t_stanby);
    void set_t_stanby(u8 t_stanby);
    void set_IIR_filter(u8 set_byte);
    u8   read_measure_stat();
    u8   read_otp_stat();
    void reset_control();
    s16  get_otp(u8 add0,u8 add1);
    s32  get_a0_otp(u8 addex,u8 add0,u8 add1);
    u32  get_b00_otp(u8 addex,u8 add0,u8 add1);
    void download_otp_value();
    void set_average_power(u8 temp_aver,u8 press_aver,u8 power_mode);
    void read_uncom_tempValue();
    void read_uncom_pressValue();
    double  get_tempValue();
    double  get_pressValue();
    double  a0,a1,a2;
    double  bt1,bt2;
    double  bp1,bp2,bp3;
    double  b11,b12,b21,b00;
    double  Dt,Dp;
    double  Tr,Pr;
    u8 d1,d2,d3;
    u32 d;
};
extern SMPB sensor;

#endif