#include "ArduinoSPIOdometry.h"
#include <Arduino.h>

ArduinoSPIOdometry::ArduinoSPIOdometry(
    float wheel_diameter,
    unsigned int ticks_per_revolution,
    bool use_microseconds,
    Stream* serial_port,
    LS7366R::QuadratureEncoder* quadrature_encoder
    ) :
        Odometry(wheel_diameter, ticks_per_revolution, use_microseconds),
        m_serial_port(serial_port),
        m_encoder(quadrature_encoder)
{}

void ArduinoSPIOdometry::begin()
{
    this->m_encoder->init();
}

Odometry::Velocity_t ArduinoSPIOdometry::getVelocity(unsigned long timePeriod)
{

    getCounts();

    long countDiff = this->m_current_count - this->m_last_count;

    // velocity (meters/second)
    // velocity = distance/time
    // distance = (circumference / 1 revolution) * (measured num ticks / num ticks per revolution)
    // time = period passed / 1000000
    // constant = (circumference * 1000 * 1000) / num ticks per revolution
    // velocity = (constant * measured num ticks) / period passed in us

    float velocity = (this->m_velocity_constant * countDiff) / timePeriod;

    // TODO: implement a low-pass (or high-pass?) filter
    //  https://en.wikipedia.org/wiki/Low-pass_filter#Simple_infinite_impulse_response_filter

    this->m_last_count = this->m_current_count;

    if (NULL != m_serial_port)
    {
        String countStr = "Count: " + String(this->m_current_count);
        m_serial_port->println(countStr);
    }

    return velocity;
}

Odometry::Velocity_t ArduinoSPIOdometry::radiansPerSecond(unsigned long timePeriod) {

    getCounts();

    long countDiff = this->m_current_count - this->m_last_count;

    // Radians per revolution = 2 * PI
    // wheel rotated factor = num ticks (tick diff) / num ticks per revolution
    float radiansPerSecond = (this->m_radians_constant * countDiff) / timePeriod;

    this->m_last_count = this->m_current_count;

    if (NULL != m_serial_port)
    {
        String countStr = "Count: " + String(this->m_current_count);
        m_serial_port->println(countStr);
    }

    return radiansPerSecond;
}

void ArduinoSPIOdometry::getCounts()
{

    this->m_current_count = this->m_encoder->count();
}

bool ArduinoSPIOdometry::direction()
{

    byte status = m_encoder->status();
    return m_encoder->statusDirection(status);
}