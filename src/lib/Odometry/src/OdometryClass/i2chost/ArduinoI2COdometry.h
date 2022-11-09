#ifndef ARDUINO_I2C_ODOMETRY_H
#define ARDUINO_I2C_ODOMETRY_H

#include <Arduino.h>
#include <Wire.h>
#include <Odometry.h>

class ArduinoI2COdometry: public Odometry {

    public:

        ArduinoI2COdometry(
            float wheel_diameter, // in meters
            unsigned int ticks_per_revolution,
            bool use_microseconds,
            Stream* serial_port,
            unsigned short i2c_address);

        void begin(TwoWire *wirePort = &Wire);

        Odometry::Velocity_t getVelocity(unsigned long timePeriod);

        bool direction();

    protected:

        short count1() {return receive_count1;}
        short count2() {return receive_count2;}

        void getCounts();

    private:

        short receive_count1;
        short receive_count2;

        unsigned int m_receive_count_size; // in bytes

        long m_current_count1;
        long m_current_count2;

        long m_last_count1;
        long m_last_count2;

        unsigned short m_i2c_address;

        Stream *m_serial_port;
        TwoWire *m_wire_port;

        byte m_count_buffer[2+2]; // two shorts
};

#endif // ARDUINO_I2C_ODOMETRY_H