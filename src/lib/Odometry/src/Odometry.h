#ifndef ODOMETRY_H
#define ODOMETRY_H

/*
    A generic abstract class for representing velocity readings as a measurement of wheel ticks.
    Ticks are typically measured via a Quadrature Encoder.

    One revolution of a wheel will represent a set number of ticks depending on implementation.
    This number plus the wheel diameter will define a velocity when measured over a time period.

    The time period is based on one second, but the user must define whether to measure it based
    on microseconds or milliseconds. Set 'use_microseconds' to true for microseconds or false for
    for milliseconds.
*/

class Odometry {

    public:

        Odometry(
            float wheel_diameter,
            unsigned int ticks_per_revolution,
            bool use_microseconds);

        typedef float Velocity_t;

        /**
         * Calculate the linear velocity of a wheel
        */
        virtual Velocity_t getVelocity(unsigned long timePeriod) = 0;

        /**
         * Calculate the rotational velocity of a wheel in radians per second
        */
        virtual Velocity_t radiansPerSecond(unsigned long timePeriod) = 0;

        /**
         * Return the rotational direction of the wheel.
         *
         * The value implies only that the wheel is rotating in one of two possible directions. Users
         * must interpret forward/reverse, left/right, etc based on the implementation and placement
         * of the wheel.
         */
        virtual bool direction() = 0;
    
    protected:
        float m_wheel_diameter;
        unsigned int m_ticks_per_revolution;

        // The following value allows us to reduce calculations:
        
        // when determining wheel velocity
        // constant = (circumference * full time period in ms or us) / ticks per wheel revolution
        float m_velocity_constant;

        // when determining wheel radians per second
        float m_radians_constant;

        long m_current_count;

        long m_last_count;
    
        bool m_use_microseconds;

        // Obtain the number of ticks occurring since the previous measurement point
        virtual void getCounts() = 0;
};

#endif // ODOMETRY_H