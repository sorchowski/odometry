#include <AbstractVelocity.h>

AbstractVelocity::AbstractVelocity(
    float wheel_diameter,
    unsigned int ticks_per_revolution,
    bool use_microseconds) :
        m_last_count(0),
        m_current_count(0),
        m_use_microseconds(use_microseconds)
{

    // If use_microseconds is set, set the scaling factor for that; otherwise, we assume milliseconds
    // 1000*1000 = 1000000 us/second
    unsigned long scalingFactor = (use_microseconds) ? (1000*1000) : 1000;

    this->m_velocity_constant = (this->m_wheel_diameter * PI * scalingFactor) / this->m_ticks_per_revolution;
}