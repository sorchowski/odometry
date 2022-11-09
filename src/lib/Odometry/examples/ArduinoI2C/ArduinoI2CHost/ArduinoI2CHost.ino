#include <Wire.h>
#include <OdometryI2C.h>
#include "Balance.h"

#define ENABLE_ODOM true
#define ENABLE_OUTPUT_VALUES true

// Per second
#define SAMPLE_RATE_ODOM 20
#define SERIAL_DEBUG true

OdometryI2C odometry(ENCODER_I2C_ADDRESS);

void setup() {
  //while (!SerialUSB) {
  //  ; // wait for serial port to connect
  //}

  if (SERIAL_DEBUG) {
    Serial.begin(115200);
  }

  // Delay to allow our encoder handler to come online.
  delay(100);

  Wire.begin();
  Wire.setTimeout(10);
  Wire1.begin(); // The odometer is on the alternate I2C bus.
  Wire1.setTimeout(10);

  if (ENABLE_ODOM) {
    if (SERIAL_DEBUG) {
      odometry.begin(&Wire1, &Serial);
    } else {
      odometry.begin(&Wire1);
    }
  }
}

unsigned long lastMilliOdom= 0;

unsigned long numberOfOdometryReadings = 0;

void loop() {
  unsigned long currentMilli = millis();

  unsigned long odomTimePeriod = currentMilli-lastMilliOdom;
  if (ENABLE_ODOM && (odomTimePeriod > (1000UL/SAMPLE_RATE_ODOM))) {

    odometry.getCounts();

    if (SERIAL_DEBUG) {
      short count1 = odometry.count1();
      short count2 = odometry.count2();
      Serial.println("Counts: "+String(count1)+", "+String(count2));
    }

    numberOfOdometryReadings += 1;
    
    lastMilliOdom = millis();
  }
  
}