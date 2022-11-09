#ifndef ARDUINO_SPI_ODOMETRY_H
#define ARDUINO_SPI_ODOMETRY_H

#include <Arduino.h>
#include <Odometry.h>
#include <OdometryClass/ls7366/LS7366R.h>


/*
    An implementation of Odometry that connects a LS7366 Quadrature encoder to an Arduino via the SPI interface.

    Example hosting program:

    #include <LS7366R.h>
    #include <ArduinoVelocitySPI.h>
    #include <SPI.h>

    using namespace LS7366R;

    byte MDR0_CONFIG = MDR0_X4QUAD | MDR0_FREE_CNT | MDR0_IDX_DISABLE | MDR0_FILTR_CLK_DIV1;
    byte MDR1_CONFIG = MDR1_CNT_2BYTE | MDR1_FLAG_NONE;

    QuadratureEncoder * m_encoder = new QuadratureEncoder(m_encoder1_select, MDR0_CONFIG, MDR1_CONFIG);

    float wheel_diameter = 0.1;  // in meters

    // Dependent on quadrature encoder implementation
    unsigned long ticks_per_revolution = <expected ticks per revolution>;

    bool use_microseconds = true;

    ArduinoSPIOdometry Odometry(wheel_diameter, ticks_per_revolution, use_microseconds, NULL, m_encoder);

    AbstractOdometry::Odometry_t odometry = odometry.getVelocity(<time period>);


*/

class ArduinoSPIOdometry: public Odometry {

    public:

        ArduinoSPIOdometry(
            float wheel_diameter, // in meters
            unsigned int ticks_per_revolution,
            bool use_microseconds,
            Stream* serial_port,
            LS7366R::QuadratureEncoder* encoder);

        void begin();

        Odometry::Velocity_t getVelocity(unsigned long timePeriod);

        bool direction();

    protected:
    
        void getCounts();

        Stream* m_serial_port;

        LS7366R::QuadratureEncoder* m_encoder;

        // TODO: add methods for converting status byte from ls7366
};

#endif // ARDUINO_SPI_ODOMETRY_H