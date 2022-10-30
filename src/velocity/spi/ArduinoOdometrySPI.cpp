#include <ArduinoOdometrySPI.h>
#include <Arduino.h>

ArduinoOdometrySPI::ArduinoOdometrySPI(
    float wheel_diameter,
    unsigned int ticks_per_revolution,
    bool use_microseconds,
    Stream* serial_port,
    LS7366R::QuadratureEncoder* quadrature_encoder
    ) :
        AbstractOdometry(wheel_diameter, ticks_per_revolution, use_microseconds),
        m_serial_port(serial_port),
        m_encoder(quadrature_encoder)
{}

void ArduinoOdometrySPI::begin()
{
    this->m_encoder->init();
}

AbstractOdometry::Velocity_t ArduinoOdometrySPI::getVelocity(unsigned long timePeriod)
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

void ArduinoOdometrySPI::getCounts()
{

    this->m_current_count = this->m_encoder->count();
}

bool ArduinoOdometrySPI::direction()
{

    byte status = m_encoder->status();
    return m_encoder->statusDirection(status);
}