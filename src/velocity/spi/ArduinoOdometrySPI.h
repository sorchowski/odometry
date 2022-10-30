#ifndef ODOMETRY_H
#define ODOMETRY_H

#include <Arduino.h>
#include <LS7366R.h>
#include <AbstractOdometry.h>

/*
    An implementation of AbstractOdometry that connects a LS7366 Quadrature encoder
    to an Arduino via the SPI interface.

    Example hosting program:

    #include <LS7366R.h>
    #include <ArduinoOdometrySPI.h>
    #include <SPI.h>

    using namespace LS7366R;

    byte MDR0_CONFIG = MDR0_X4QUAD | MDR0_FREE_CNT | MDR0_IDX_DISABLE | MDR0_FILTR_CLK_DIV1;
    byte MDR1_CONFIG = MDR1_CNT_2BYTE | MDR1_FLAG_NONE;

    QuadratureEncoder * m_encoder = new QuadratureEncoder(m_encoder1_select, MDR0_CONFIG, MDR1_CONFIG);

    float wheel_diameter = 0.1;  // in meters

    // Dependent on quadrature encoder implementation
    unsigned long ticks_per_revolution = <expected ticks per revolution>;

    bool use_microseconds = true;

    ArduinoOdometrySPI odometry(wheel_diameter, ticks_per_revolution, use_microseconds, NULL, m_encoder);

    AbstractOdometry::Velocity_t velocity = odometry.getVelocity(<time period>);


*/

class ArduinoOdometrySPI: public AbstractOdometry {

    public:

        ArduinoOdometrySPI(
            float wheel_diameter, // in meters
            unsigned int ticks_per_revolution,
            bool use_microseconds,
            Stream* serial_port,
            LS7366R::QuadratureEncoder* encoder);

        void begin();

        AbstractOdometry::Velocity_t getVelocity(unsigned long timePeriod);

        bool direction();

    protected:
    
        void getCounts();

        Stream* m_serial_port;

        LS7366R::QuadratureEncoder* m_encoder;

        // TODO: add methods for converting status byte from ls7366
};

#endif // ODOMETRY_H