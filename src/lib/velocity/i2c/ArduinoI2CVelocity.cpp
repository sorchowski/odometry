#include <ArduinoI2CVelocity.h>

#include <Arduino.h>
#include <Wire.h>

ArduinoI2CVelocity::ArduinoI2CVelocity(
    float wheel_diameter,
    unsigned int ticks_per_revolution,
    bool use_microseconds,
    Stream *serial_port,
    unsigned short i2c_address) :
        AbstractVelocity(wheel_diameter, ticks_per_revolution, use_microseconds),
        m_serial_port(serial_port),
        m_i2c_address(i2c_address),
        m_last_count1(0),
        m_last_count2(0),
        m_current_count1(0),
        m_current_count2(0),
        m_receive_count_size(sizeof(receive_count1)+sizeof(receive_count2))
{}

void ArduinoI2CVelocity::begin(TwoWire *wirePort /*=&Wire*/)
{
    m_wire_port = wirePort;
}

void ArduinoI2CVelocity::getCounts()
{

    m_current_count1 = 0;
    m_current_count2 = 0;

    int bytesReturned = m_wire_port->requestFrom(m_i2c_address, m_receive_count_size, true);

    int i = 0;
    while (m_wire_port->available())
    {
        m_count_buffer[i++] = m_wire_port->read();
    }

    if (bytesReturned != m_receive_count_size)
    {
        if (NULL != m_serial_port)
        {
            m_serial_port->println("Error. Did not receive counts: " + String(bytesReturned));
        }
    }
    else
    {
        short count1 = *(short *)(m_count_buffer);
        short count2 = *(short *)(m_count_buffer + sizeof(receive_count1));

        m_current_count1 = (long)count1;
        m_current_count2 = (long)count2;
    }
}

AbstractVelocity::Velocity_t getVelocity(unsigned long timePeriod)
{
    // TODO
    return 0.0;
}

bool ArduinoI2CVelocity::direction()
{
    // TODO
    return false;
}