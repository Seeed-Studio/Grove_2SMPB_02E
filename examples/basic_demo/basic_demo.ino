
#include "SMPB.h"


    #ifdef SEEED_XIAO_M0
        #define SERIAL Serial
    #elif defined(ARDUINO_SAMD_VARIANT_COMPLIANCE)
        #define SERIAL SerialUSB
    #else
        #define SERIAL Serial
    #endif
SMPB sensor(SMPB_ADDR);


void setup() {
    SERIAL.begin(9600);
    Wire.begin();
    SERIAL.println("Test Begin!");

    //set t_stanby (sleep time),1/5/50/250/500/1000/2000/4000ms
    sensor.init(T_STANBY_50ms);

    //Calculate the average number of temperatures and pressure,and set mode
    //temp_aver:TEMP_AVERAGE_1/2/4/8..
    //press_aver:PRESS_AVERAGE_1/2/4/8..
    //power_mode:SLEEP_MODE/FORCED_MODE/FORCED_MODE
    sensor.set_average_power(TEMP_AVERAGE_1, PRESS_AVERAGE_4, NORMAL_MODE);
    delay(300);
}


void loop() {
    u8 a;

    double Temp, Press;

    while (!sensor.read_measure_stat()) {
        SERIAL.println("loading...........");
        SERIAL.println();
        delay(5);
    }
    sensor.read_uncom_tempValue();
    sensor.read_uncom_pressValue();

    Temp = sensor.get_tempValue() / 256.0;
    SERIAL.print("Temperature : ");
    SERIAL.print(Temp);
    SERIAL.println(" C");

    Press = sensor.get_pressValue();
    SERIAL.print("Pressure : ");
    SERIAL.print(Press);
    SERIAL.println(" Pa");

    SERIAL.println();
    SERIAL.println();
    delay(800);

}