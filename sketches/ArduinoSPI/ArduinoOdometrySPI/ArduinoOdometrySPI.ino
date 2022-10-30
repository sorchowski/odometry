#include <LS7366R.h>
#include <SPI.h>
#include <ArduinoSPIVelocity.h>
#include <AbstractOdometry.h>

#define ENCODER_SELECT 10

using namespace LS7366R;

byte MDR0_CONFIG = MDR0_X4QUAD|MDR0_FREE_CNT|MDR0_IDX_DISABLE|MDR0_FILTR_CLK_DIV1;
byte MDR1_CONFIG = MDR1_CNT_4BYTE|MDR1_FLAG_NONE;

float wheel_diameter = 0.1016; // in meters (4 inches)
unsigned int ticks_per_revolution = 1965;

QuadratureEncoder encoder(ENCODER_SELECT, MDR0_CONFIG, MDR1_CONFIG);
ArduinoOdometrySPI odometry(wheel_diameter, ticks_per_revolution, true, &Serial, &encoder);

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
  AbstractOdometry::Velocity_t velocity = odometry.getVelocity(timePeriod);
  Serial.println("Velocity: "+String(velocity));
  Serial.println("timePeriod: "+String(timePeriod));
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
