#include <Odometry.h>
#include <OdometryClass/ls7366/LS7366R.h>
#include <OdometryClass/spi/ArduinoSPIOdometry.h>

#include <SPI.h>

// For 3.3v shield:
// Encoder near the Dx pins
#define ENCODER1_SELECT 9
// Encoder near the Ax and power pins
#define ENCODER2_SELECT 10

// For 5v shield:
// Encoder near the Dx pins
//#define ENCODER1_SELECT 10
// Encoder near the Ax and power pins
//#define ENCODER2_SELECT 9

using namespace LS7366R;

byte MDR0_CONFIG = MDR0_X4QUAD|MDR0_FREE_CNT|MDR0_IDX_DISABLE|MDR0_FILTR_CLK_DIV1;
byte MDR1_CONFIG = MDR1_CNT_4BYTE|MDR1_FLAG_NONE;

// in meters (4 inches)
float wheel_diameter = 0.1016;

// https://www.pololu.com/product/28233
// 30:1 metal gear motor with encoder at 64 counts per revolution. 1920=30*64
unsigned int ticks_per_revolution = 1920;

QuadratureEncoder encoder1(ENCODER1_SELECT, MDR0_CONFIG, MDR1_CONFIG);
ArduinoSPIOdometry odometry1(wheel_diameter, ticks_per_revolution, true, &Serial, &encoder1);

QuadratureEncoder encoder2(ENCODER2_SELECT, MDR0_CONFIG, MDR1_CONFIG);
ArduinoSPIOdometry odometry2(wheel_diameter, ticks_per_revolution, true, &Serial, &encoder2);

unsigned long currentMicros = 0;
unsigned long lastMicros = 0;

void setup(){
  Serial.begin(115200);
  pinMode(ENCODER1_SELECT, OUTPUT);
  pinMode(ENCODER2_SELECT, OUTPUT);
  SPI.begin();
  odometry1.begin();
  odometry2.begin();
}

void loop() {
  delay(1000);

  currentMicros = micros();
  unsigned long timePeriod = currentMicros - lastMicros;

  Odometry::Velocity_t velocity1 = odometry1.getVelocity(timePeriod);
  Serial.println("Velocity1: "+String(velocity1, 4));
  bool direction1 = odometry1.direction();
  Serial.println("direction1: "+String(direction1));

  Odometry::Velocity_t velocity2 = odometry2.getVelocity(timePeriod);
  Serial.println("Velocity2: "+String(velocity2, 4));
  bool direction2 = odometry2.direction();
  Serial.println("direction2: "+String(direction2));

  //long count = encoder1.count();
  //byte status = encoder1.status();
  //Serial.print("count1: ");
  //Serial.print(count1);
  //Serial.print(", status1: ");
  //Serial.println(status1);

  lastMicros = currentMicros;

}
