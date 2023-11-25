#include <Odometry.h>

Odometry::Odometry(
    float wheel_diameter,
    unsigned int ticks_per_revolution,
    bool use_microseconds) :
        m_wheel_diameter(wheel_diameter),
        m_ticks_per_revolution(ticks_per_revolution),
        m_last_count(0),
        m_current_count(0),
        m_use_microseconds(use_microseconds)
{

    // If use_microseconds is set, set the scaling factor for that; otherwise, we assume milliseconds
    // 1000*1000 = 1000000 us/second
    const float scalingFactor = (use_microseconds) ? (1000.0*1000.0) : 1000.0;
    const float PI = 3.14159;

    this->m_velocity_constant = (this->m_wheel_diameter * PI * scalingFactor) / ((float)(this->m_ticks_per_revolution));

    this->m_radians_constant = (2.0 * PI * scalingFactor) / ((float)(this->m_ticks_per_revolution));
}