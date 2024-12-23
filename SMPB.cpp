
#include "SMPB.h"
#include <math.h>


#ifdef SEEED_XIAO_M0
    #define SERIAL Serial
#elif defined(ARDUINO_SAMD_VARIANT_COMPLIANCE)
    #define SERIAL SerialUSB
#else
    #define SERIAL Serial
#endif

SMPB::SMPB(u8 ADDR) {
    _IIC_ADDR = ADDR;
}

/******************************************************************************************
    function:init()
    parameter:t_stanby  // T_STANBY_1ms/5ms..
    describe: Initialize the sensor and set the sleep time
*******************************************************************************************/
void SMPB::init(s32 t_stanby) {
    u8 a;

    //set t_stanby time
    sensor.reset_control();
    delay(500);
    sensor.download_otp_value();

    set_t_stanby(t_stanby);
    iic_read_byte(0xD1, &a);
}

/****************************************************************************************
    function:set_IIR_filter()
    parameter:set_byte  // IIR_N_2/4/6...
    describe: Set filtering parameters according to scene
*****************************************************************************************/
void SMPB::set_IIR_filter(u8 set_byte) {
    iic_write_byte(IIR_CNT_ADD, set_byte);
}

/****************************************************************************************
    function:set_t_stanby()
    parameter:t_stanby  // T_STANBY_1ms/5ms...
    describe: Set the sleep time
*****************************************************************************************/
void SMPB::set_t_stanby(u8 t_stanby) {
    iic_write_byte(IO_SETUP_ADD, t_stanby);
}

/****************************************************************************************
    function:read_measure_stat()
    parameter:
    describe: Obtain detection status
*****************************************************************************************/
u8 SMPB::read_measure_stat() {
    u8 measure_stat;
    delay(50);
    iic_read_byte(DEVICE_STAT_ADD, &measure_stat);
    measure_stat &= 0x08;
    return measure_stat;
}

/****************************************************************************************
    function:read_otp_stat()
    parameter:
    describe: Obtain otp status
*****************************************************************************************/
u8 SMPB::read_otp_stat() {
    u8 otp_stat;
    iic_read_byte(DEVICE_STAT_ADD, &otp_stat);
    otp_stat &= 0x01;

    return otp_stat;
}

/****************************************************************************************
    function:reset_control()
    parameter:
    describe: Restart from software
*****************************************************************************************/
void SMPB::reset_control() {
    iic_write_byte(RESET_ADD, RESET_COMMAND);
}

/****************************************************************************************
    function:get_otp()
    parameter:add0,add1  //Register address
    describe: Calculation of otp data
*****************************************************************************************/
s16 SMPB::get_otp(u8 add0, u8 add1) {
    u8  data0 = 0;
    u8  data1 = 0;
    u16 data = 0;
    s16 data_convert = 0;
    iic_read_byte(add0, &data0);
    iic_read_byte(add1, &data1);
    data = ((u16)data1 << 8 | data0);
    data_convert = data;
    return data_convert;
}

/****************************************************************************************
    function:get_a0_otp()
    parameter:addex,add0,add1  //Register address
    describe: Calculation of a0 otp data
*****************************************************************************************/
s32 SMPB::get_a0_otp(u8 addex, u8 add0, u8 add1) {
    u8  data_ex = 0;
    u8  data0 = 0;
    u8  data1 = 0;
    u32 data = 0;
    s16 data_convert = 0;
    iic_read_byte(addex, &data_ex);
    iic_read_byte(add0, &data0);
    iic_read_byte(add1, &data1);
    data = ((u32)data1 << 12 | (u32)data0 << 4 | (data_ex & 0x0F));
    data_convert = (s16)data;
    return data_convert;
}

/****************************************************************************************
    function:get_b00_otp()
    parameter:addex,add0,add1  //Register address
    describe: Calculation of b00 otp data
*****************************************************************************************/
u32 SMPB::get_b00_otp(u8 addex, u8 add0, u8 add1) {
    u8  data_ex = 0;
    u8  data0 = 0;
    u8  data1 = 0;
    u32 data = 0;
    s32 data_convert = 0;
    iic_read_byte(addex, &data_ex);
    iic_read_byte(add0, &data0);
    iic_read_byte(add1, &data1);
    data = ((u32)data1 << 12 | (u32)data0 << 4 | (u32)data_ex >> 4);
    return data;
}

/****************************************************************************************
    function:download_otp_value()
    parameter:
    describe: Get the otp value
*****************************************************************************************/
void SMPB::download_otp_value() {
    double otp;
    //a0
    otp = get_a0_otp(COE_b00_a0_ex, COE_a0_0, COE_a0_1);
    a0 = otp / 16.0;
    //a1
    otp = get_otp(COE_a1_0, COE_a1_1);
    a1 = (a1_A) + (a1_S) * otp / 32767;
    //a2
    otp = get_otp(COE_a2_0, COE_a2_1);
    a2 = (a2_A) + (a2_S) * otp / 32767;
    //bt1
    otp = get_otp(COE_bt1_0, COE_bt1_1);
    bt1 = (bt1_A) + (bt1_S) * otp / 32767;
    //bt2
    otp = get_otp(COE_bt2_0, COE_bt2_1);
    bt2 = (bt2_A) + (bt2_S) * otp / 32767;
    //bp1
    otp = get_otp(COE_bp1_0, COE_bp1_1);
    bp1 = (bp1_A) + (bp1_S) * otp / 32767;
    //bp2
    otp = get_otp(COE_bp2_0, COE_bp2_1);
    bp2 = (bp2_A) + (bp2_S) * otp / 32767;
    //bp3
    otp = get_otp(COE_bp3_0, COE_bp3_1);
    bp3 = (bp3_A) + (bp3_S) * otp / 32767;
    //b11
    otp = get_otp(COE_b11_0, COE_b11_1);
    b11 = (b11_A) + (b11_S) * otp / 32767;
    //b12
    otp = get_otp(COE_b12_0, COE_b12_1);
    b12 = (b12_A) + (b12_S) * otp / 32767;
    //b21
    otp = get_otp(COE_b21_0, COE_b21_1);
    b21 = (b21_A) + (b21_S) * otp / 32767;
    //b00
    otp = get_b00_otp(COE_b00_a0_ex, COE_b00_0, COE_b00_1);
    b00 = otp / 16.0;
}

/****************************************************************************************
    function:set_average_power()
    parameter:temp_aver:TEMP_AVERAGE_1/2/4/8..
          press_aver:PRESS_AVERAGE_1/2/4/8..
          power_mode:SLEEP_MODE/FORCED_MODE/FORCED_MODE
    describe: Calculate the average number of temperatures and pressure,and set mode
*****************************************************************************************/
void SMPB::set_average_power(u8 temp_aver, u8 press_aver, u8 power_mode) {
    u8 data = 0;
    data = (temp_aver | press_aver | power_mode);
    iic_write_byte(CTRL_MEAS_ADD, data);
}

/****************************************************************************************
    function:read_uncom_tempValue()
    parameter:
    describe: Get the uncompensated temperature value
*****************************************************************************************/
void SMPB::read_uncom_tempValue() {
    u8  data0, data1, data2;
    u32 data = 0;
    iic_read_byte(TEMP_TXD0_ADD, &data0);
    iic_read_byte(TEMP_TXD1_ADD, &data1);
    iic_read_byte(TEMP_TXD2_ADD, &data2);
    data = ((u32)data2 << 16 | (u32)data1 << 8 | data0);
    Dt = data - pow(2.0, 23.0);
}

/****************************************************************************************
    function:read_uncom_pressValue()
    parameter:
    describe: Get the uncompensated pressure value
*****************************************************************************************/
void SMPB::read_uncom_pressValue() {
    u8  data0, data1, data2;
    u32 data = 0;
    iic_read_byte(PRESS_TXD0_ADD, &data0);
    iic_read_byte(PRESS_TXD1_ADD, &data1);
    iic_read_byte(PRESS_TXD2_ADD, &data2);
    data = ((u32)data2 << 16 | (u32)data1 << 8 | data0);
    Dp = data - pow(2.0, 23.0);
}

/****************************************************************************************
    function:get_tempValue()
    parameter:
    describe: Get the temperature value
*****************************************************************************************/
double SMPB::get_tempValue() {
    Tr = a0 + (a1 * Dt) + (a2 * Dt * Dt);
    return Tr;
}

/****************************************************************************************
    function:get_pressValue()
    parameter:
    describe: Get the pressure value
*****************************************************************************************/
double SMPB::get_pressValue() {
    Pr = b00 + (bt1 * Tr) + (bp1 * Dp) + (b11 * Dp * Tr) + (bt2 * Tr * Tr)
         + (bp2 * Dp * Dp) + (b12 * Dp * Tr * Tr)
         + (b21 * Dp * Dp * Tr) + (bp3 * Dp * Dp * Dp);
    return Pr;
}

/**********************************************************************************************
 **************IIC Opration******************************************************************
 *********************************************************************************************/


void IIC_OPRTS::iic_read_byte(u8 reg_addr, u8* byte) {
    u32 time_out_count = 0;
    Wire.beginTransmission(_IIC_ADDR);
    Wire.write(reg_addr);
    Wire.endTransmission(false);

    Wire.requestFrom(_IIC_ADDR, (u8)1);
    while (1 != Wire.available()) {
        time_out_count++;
        if (time_out_count > 10) {
            break;
        }
        delay(1);
    }
    *byte = Wire.read();

}

s32 IIC_OPRTS::iic_write_byte(u8 set_addr, u8 set_byte) {
    Wire.beginTransmission(_IIC_ADDR);
    Wire.write(set_addr);
    Wire.write(set_byte);
    return Wire.endTransmission();
}

