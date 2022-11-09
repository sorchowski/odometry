#include <Odometry.h>
#include <OdometryClass/ls7366/LS7366R.h>
#include <OdometryClass/spi/ArduinoSPIOdometry.h>

#include <SPI.h>

#define ENCODER_SELECT 10

using namespace LS7366R;

byte MDR0_CONFIG = MDR0_X4QUAD|MDR0_FREE_CNT|MDR0_IDX_DISABLE|MDR0_FILTR_CLK_DIV1;
byte MDR1_CONFIG = MDR1_CNT_4BYTE|MDR1_FLAG_NONE;

// in meters (4 inches)
float wheel_diameter = 0.1016;

// https://www.pololu.com/product/28233
// 30:1 metal gear motor with encoder at 64 counts per revolution. 1920=30*64
unsigned int ticks_per_revolution = 1920;

QuadratureEncoder encoder(ENCODER_SELECT, MDR0_CONFIG, MDR1_CONFIG);
ArduinoSPIOdometry odometry(wheel_diameter, ticks_per_revolution, true, &Serial, &encoder);

unsigned long currentMicros = 0;
unsigned long lastMicros = 0;

void setup(){
  Serial.begin(115200);
  pinMode(ENCODER_SELECT, OUTPUT);
  SPI.begin();
  odometry.begin();
}

void loop() {
  delay(1000);

  currentMicros = micros();
  unsigned long timePeriod = currentMicros - lastMicros;
  Odometry::Velocity_t velocity = odometry.getVelocity(timePeriod);
  Serial.println("Velocity: "+String(velocity, 4));
  //Serial.println("timePeriod: "+String(timePeriod));
  bool direction = odometry.direction();
  Serial.println("direction: "+String(direction));
  //long count = encoder.count();
  //byte status = encoder.status();
  //Serial.print("count: ");
  //Serial.print(count);
  //Serial.print(", status: ");
  //Serial.println(status);
  lastMicros = currentMicros;
}
