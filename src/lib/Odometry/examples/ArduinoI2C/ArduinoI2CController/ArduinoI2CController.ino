#include <Wire.h>
#include <Odometry.h>
#include <OdometryClass/OdometryPayload.h>
#include <OdometryClass/i2chost/ArduinoI2COdometry.h>

// DUE: Wire = pins 20 and 21
//      Wire1 = pins 70 and 71
//      SerialUSB = Native port
//      Serial = Programming port

// UNO:
//      Serial

#define I2C_ADDRESS 8

// in meters (4 inches)
float wheel_diameter = 0.1016;

// https://www.pololu.com/product/28233
// 30:1 metal gear motor with encoder at 64 counts per revolution. 1920=30*64
unsigned int ticks_per_revolution = 1920;

ArduinoI2COdometry odometry(wheel_diameter, ticks_per_revolution, true, &Serial, I2C_ADDRESS);

unsigned long currentMicros = 0;
unsigned long lastMicros = 0;

void setup() {

    Wire.begin();
    //SerialUSB.begin(9600);
    Serial.begin(115200);

    odometry.begin(&Wire);
}

void loop() {

    delay(1000);

    currentMicros = micros();
    unsigned long timePeriod = currentMicros - lastMicros;
    Odometry::Velocity_t velocity = odometry.getVelocity(timePeriod);
    Serial.println("Velocity: "+String(velocity, 4));

    lastMicros = currentMicros;
}