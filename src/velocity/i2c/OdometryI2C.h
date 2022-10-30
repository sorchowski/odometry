#ifndef ODOMETRY_I2C_H
#define ODOMETRY_I2C_H

#include <Arduino.h>
#include <Wire.h>

class OdometryI2C {

    public:

        OdometryI2C(unsigned short i2c_address);
        
        // Pass in "Serial"
        void begin(TwoWire *wirePort = &Wire, Stream *serialPort = NULL);

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

#endif // ODOMETRY_I2C_H