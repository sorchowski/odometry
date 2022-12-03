#include <Wire.h>
#include <SPI.h>

#ifdef ARDUINO_AVR
// For ATOMIC_BLOCK
#include <util/atomic.h>
#endif // ARDUINO_AVR

#include <Odometry.h>
#include <OdometryClass/ls7366/LS7366R.h>
#include <OdometryClass/OdometryPayload.h>

#define I2C_ADDRESS 8
#define ENCODER_SELECT1 10
#define ENCODER_SELECT2 9
#define ODOMETRY_READINGS_PER_SECOND 50

using namespace LS7366R;

byte MDR0_CONFIG = MDR0_X4QUAD|MDR0_FREE_CNT|MDR0_IDX_DISABLE|MDR0_FILTR_CLK_DIV1;
byte MDR1_CONFIG = MDR1_CNT_4BYTE|MDR1_FLAG_NONE;

QuadratureEncoder encoder1(ENCODER_SELECT1, MDR0_CONFIG, MDR1_CONFIG);
//QuadratureEncoder encoder2(ENCODER_SELECT2, MDR0_CONFIG, MDR1_CONFIG);

volatile OdometryPayload odometryPayload;
// number of milliseconds between each odometry countr reading
long measurePeriod = 1000/ODOMETRY_READINGS_PER_SECOND;
long lastMillis = 0;

void setup() {

    pinMode(ENCODER_SELECT1, OUTPUT);
    pinMode(ENCODER_SELECT2, OUTPUT);

    SPI.begin();
    encoder1.init();
    //encoder2.init();

    Wire.begin(I2C_ADDRESS);
    Wire.onRequest(requestEvent);

    Serial.begin(9600);
}

//long tempCount1 = 0;
//long tempCount2 = 0;

void loop() {

    delay(2);

    long currentMillis = millis();
    if ((currentMillis - lastMillis) > measurePeriod) {
        
        //Serial.println("period(ms): "+String(measurePeriod));

        //tempCount1 += 1000;
        //tempCount2 += 2000;
        long tempCount1 = encoder1.count();
        odometryPayload.status1 = encoder1.status();

        //long tempCount2 = encoder2.count();
        //odometryPayload.status2 = encoder2.status();
        //Serial.println("count 1: "+String(tempCount1));
#ifdef ARDUINO_AVR

        // avr processors cannot atomically set 32-bit values with one instruction
        // Disable interrupts and set the values for the Odometry Payload
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            odometryPayload.count1 = tempCount1;
            odometryPayload.count2 = 12345678;
            //odometryPayload.count2 = tempCount2;
        }

#else// not ARDUINO_AVR

        // ARDUINO_SAM and ARDUINO_SAMD can set 32-bit values atomically with one instruction
        odometryPayload.count1 = tempCount1;
        odometryPayload.count2 = 12345678;
        //odometryPayload.count2 = tempCount2;

#endif

        lastMillis = currentMillis;
    }
}

void requestEvent() {

    Wire.write((byte*)&odometryPayload, sizeof odometryPayload);
}