#include <Arduino.h>
#include <Wire.h>

#include <Odometry.h>
#include "ArduinoI2COdometry.h"

ArduinoI2COdometry::ArduinoI2COdometry(
    float wheel_diameter,
    unsigned int ticks_per_revolution,
    bool use_microseconds,
    Stream *serial_port,
    unsigned short i2c_address) :
        Odometry(wheel_diameter, ticks_per_revolution, use_microseconds),
        m_serial_port(serial_port),
        m_i2c_address(i2c_address),
        m_last_count1(0),
        m_last_count2(0),
        m_current_count1(0),
        m_current_count2(0)
{
    OdometryPayload payload;
    m_odometry_payload_size = sizeof payload;
}

void ArduinoI2COdometry::begin(TwoWire *wirePort /*=&Wire*/)
{
    m_wire_port = wirePort;
}

void ArduinoI2COdometry::getCounts()
{

    OdometryPayload payload;

    Wire.requestFrom(m_i2c_address, m_odometry_payload_size);

    int bytesAvailable = Wire.available();
    if (bytesAvailable < m_odometry_payload_size) {
        if (NULL != m_serial_port)
        {
            m_serial_port->println("Error. Did not receive counts: " + String(bytesAvailable));
        }
        // TODO: is this necessary?
        while (Wire.available()) {
            byte b = Wire.read();  // eat the truncated response to clear the buffer for the next request
        }
    }
    else {
  
        for (int i=0;i<m_odometry_payload_size;i++) {
            ((byte*)&payload)[i] = Wire.read();; 
        }

        m_current_count1 = payload.count1;
        m_current_count2 = payload.count2;
        m_status1 = payload.status1;
        m_status2 = payload.status2;
    }
}

Odometry::Velocity_t ArduinoI2COdometry::getVelocity(unsigned long timePeriod)
{
    ArduinoI2COdometry::Velocities velocities = getVelocities(timePeriod);
    return velocities.velocity1;
}

Odometry::Velocity_t ArduinoI2COdometry::radiansPerSecond(unsigned long timePeriod) {
    // TODO
}

ArduinoI2COdometry::Velocities ArduinoI2COdometry::getVelocities(unsigned long timePeriod) {

    getCounts();

    long countDiff1 = this->m_current_count1 - this->m_last_count1;
    long countDiff2 = this->m_current_count2 - this->m_last_count2;
    // velocity (meters/second)
    // velocity = distance/time
    // distance = (circumference / 1 revolution) * (measured num ticks / num ticks per revolution)
    // time = period passed / 1000000
    // constant = (circumference * 1000 * 1000) / num ticks per revolution
    // velocity = (constant * measured num ticks) / period passed in us

    float velocity1 = (this->m_velocity_constant * countDiff1) / timePeriod;
    float velocity2 = (this->m_velocity_constant * countDiff2) / timePeriod;

    // TODO: implement a low-pass (or high-pass?) filter
    //  https://en.wikipedia.org/wiki/Low-pass_filter#Simple_infinite_impulse_response_filter

    this->m_last_count1 = this->m_current_count1;
    this->m_last_count2 = this->m_current_count2;

    if (NULL != m_serial_port)
    {
        String countStr = "Count1: " + String(this->m_current_count1);
        m_serial_port->println(countStr);
    }

    Velocities velocities;
    velocities.velocity1 = velocity1;
    velocities.status1 = m_status1;
    velocities.velocity2 = velocity2;
    velocities.status2 = m_status2;

    return velocities;
}

bool ArduinoI2COdometry::direction()
{
    // TODO
    return false;
}