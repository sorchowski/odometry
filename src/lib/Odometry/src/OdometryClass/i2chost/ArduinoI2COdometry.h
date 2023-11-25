#ifndef ARDUINO_I2C_ODOMETRY_H
#define ARDUINO_I2C_ODOMETRY_H

#include <Arduino.h>
#include <Wire.h>
#include <Odometry.h>
#include <OdometryClass/OdometryPayload.h>

class ArduinoI2COdometry: public Odometry {

    public:

        struct Velocities {
            float velocity1;
            float velocity2;
            byte status1;
            byte status2;
        };

        ArduinoI2COdometry(
            float wheel_diameter, // in meters
            unsigned int ticks_per_revolution,
            bool use_microseconds,
            Stream* serial_port,
            unsigned short i2c_address);

        void begin(TwoWire *wirePort = &Wire);

        Odometry::Velocity_t getVelocity(unsigned long timePeriod);

        Odometry::Velocity_t radiansPerSecond(unsigned long timePeriod);

        Velocities getVelocities(unsigned long timePeriod);

        bool direction();

    protected:

        void getCounts();

    private:

        unsigned short m_i2c_address;

        Stream *m_serial_port;
        TwoWire *m_wire_port;

        int m_odometry_payload_size;

        byte m_status1;
        byte m_status2;

        long m_current_count1;
        long m_current_count2;

        long m_last_count1;
        long m_last_count2;

};

#endif // ARDUINO_I2C_ODOMETRY_H