# Odometry

A collection of classes, Arduino sketches, and hardware schematics to form a system for reading wheel rotational velocity.

# Table of Contents
1. [Components](#components)
   - [LS7366](#ls7366)
      - [How to Use](#how-to-use)
      - [Default Configuration](#default-configuration)
      - [Example Arduino Sketch](#example-arduino-sketch)
2. [Interfaces](#interfaces)
   - [SPI](#spi)
   - [I2C](#i2c)
3. [Hardware](#hardware)
   - [5 Volt Shield](#5-volt-shield)
      - [5v Arduino Pin Mappings](#5v-arduino-pin-mappings)
   - [3 Volt Shield](#3-volt-shield)
      - [3v Arduino Pin Mappings](#3v-arduino-pin-mappings)
   - [I2C Standalone](#i2c-standalone)
4. [Testing](#testing)
5. [Future Features](#future-features)

# Components

## LS7366

The [LS7366R](https://lsicsi.com/datasheets/LS7366R.pdf) is a 32-bit Quadrature Encoder chip interfaced via SPI. [This class](src/lib/Odometry/src/OdometryClass/ls7366) abstracts the various capabilities of the chip to a set of easily-invoked methods.

### How to Use

The QuadratureEncoder class resides within the LS7366R namespace.

1. Define the desired LS7366R flags.
2. Define a SPI chip select pin.
3. Instantiate the QuadratureEncoder class.
4. Call "init()" on this object.
5. Invoke "count()" and "status()" as desired.

### Default Configuration

Default values, MDR0_DEFAULT_CONFIG and MDR1_DEFAULT_CONFIG, are available for the MDR0 and MDR1 configuration registers.

See the [LS7366R.h header file](src/lib/Odometry/src/OdometryClass/ls7366/LS7366R.h) for a complete list of availavble options. These are 'OR-ed' together to obtain final register values.

### Example Arduino sketch

```
#include <LS7366R.h>
#include <SPI.h>

#define ENCODER_SS 10

using namespace LS7366R;

byte MDR0_CONFIG = MDR0_X1QUAD|MDR0_FREE_CNT|MDR0_IDX_DISABLE|MDR0_FILTR_CLK_DIV1;
byte MDR1_CONFIG = MDR1_CNT_4BYTE|MDR1_FLAG_NONE;

QuadratureEncoder encoder(ENCODER_SS, MDR0_CONFIG, MDR1_CONFIG);

void setup(){
  pinMode(ENCODER_SS, OUTPUT);
  SPI.begin();
  encoder.init();
}

void loop() {
  long count = encoder.count();
  byte status = encoder.status();
}
```

The full sketch is available under [LS7366-basic](src/lib/Odometry/examples/ArduinoSPI/LS7366-basic).

A sketch which manually executes raw SPI commands directly to chip to demonstrate the quadrature capabilities is also available under [LS7366-raw](src/lib/Odometry/examples/ArduinoSPI/LS7366-raw).

# Interfaces

[Odometry.h](src/lib/Odometry/src/Odometry.h) declares basic velocity and directional methods common to either SPI or I2C implementations.

TODO: describe defining wheel measurements and action of class to obtain velocity in meters/s

## SPI

[ArduinoOdometrySPI](src/lib/Odometry/src/OdometryClass/spi) - Provides an implementation of AbstractOdometry for connecting an Arduino to an Ls7366R quadrature encoder via the SPI interface.

## I2C

[ArduinoI2COdometry](src/lib/Odometry/src/OdometryClass/i2chost) - Provides an implementation of AnstractOdometry for connecting an Arduino to a peripheral over the I2C interface. The peripheral implements the interface to the LS7366 chip. See the [ArduinoI2CPeripheral.ino](src/lib/Odometry/examples/ArduinoI2C/ArduinoI2CPeripheral/ArduinoI2CPeripheral.ino) sketch for the example implementation.

[OdometryPayload.h](src/lib/Odometry/src/OdometryClass/OdometryPayload.h) provides a struct definition for packaging quadrature encoder count and status data to be transmitted over the I2C line.

# Hardware

Several Arduino shields are available. Each shield contains two LS7366 Quadrature Encoder chips for tracking encoder counts. Dual JST connectors for 3.3v and 5.0v power are available along with a pair of [I2C qwiic connectors](https://www.sparkfun.com/qwiic).

The LS7366 chip operates at 5 volts and the accompanying 40 Mhz oscillator also requires 5 volts as well.

Each shield is created using a free version of Autodesk EAGLE cad and fabricated using [Oshpark](https://oshpark.com/).

## 5 Volt Shield

The [5 volt shield](schematics/DualEncoderShield5v) is meant for use with an Arduino that operates I/O logic levels between 0 and 5 volts; such as an Uno.

The 5 volt shield exposes the DFLAG and LFLAG outputs of the LS7366 chip.

Onboard jumpers enable users to map the features below to any pin. But the pins listed below are the default when utilizing a simple two-pin jumper.

### 5v Arduino Pin Mappings

Encoder One (near the Dx pins)

Feature | Pin
--- | ---
Chip Select | 10
L1 | 7
D1 | 5

Encoder Two (near the Ax and power pins)

Feature | Pin
--- | ---
Chip Select | 9
L2 | 6
D2 | 4

## 3 Volt Shield

The [3.3 volt shield](schematics/DualEncoderShield3v) is meant for use with an Arduino that operates with I/O logic levels between 0 and 3.3 volts; such as a Due. A TXU0304 chip on board provides level-shifting for SPI and one chip select signal. A single TXU0101 translates the other chip select signal for the second encoder.

The 3.3 volt shield does not expose the DFLAG nor LFLAG outputs of the LS7366 chip.

Onboard jumpers enable users to map the features below to any pin. But the pins listed below are the default when utilizing a simple two-pin jumper.

### 3v Arduino Pin Mappings

Encoder One (near the Dx pins)
Feature | Pin
--- | ---
Chip Select | 9

Encoder Two (near the Ax and power pins)
Feature | Pin
--- | ---
Chip Select | 10

## I2C Standalone

A standalone peripheral board is available for use with an I2C interface. This board contains its own Atmega328 mcu and exposes both a 5v and 3.3v I2C interface to any host controller.

A PCA9306D chip provides logic level translation between 5v and 3.3v for the I2C interface.

Of course, either the 3.3v or 5v shields can also be used as a I2C peripheral when attached to an Arduino with the appropriate peripheral sketch uploaded.

Note, the latest revision of the I2C standalone board is not tested nor confirmed to work properly.

# Testing

## Velocity

To test the velocity measurements, a small, simple, two-wheeled cart ran in a straight line over measured intervals each of one meter. Note, the cart starting point was slightly before the first one meter marker. This is to allow the cart to reach a full and consistent speed before timing is begun.

See [image of the test setup](docs/testing/testing-vehicle-odometry.jpg)

Video of the test is [here](https://youtube.com/shorts/Ld17I0CanR0)

### Radians per second

In testing radiansPerSecond(), I observed 10 revolutions over 15.88 seconds. The measured value returned by this method was roughly 3.98 radians per second.

Using a visual method of measuring the rotation of the wheel with a tape mark and a timer; we can count 10 revolutions of the tape marker while timing how long these revolutions took.

There are 2*pi radians in a revolution = 6.283. 10 revolutions times 6.283 radians per revolution = 62.83 radians over 15.88 seconds. 62.83 radians / 15.88 seconds = 3.956 radians per second; which is close to the value output by the odometry logic.

Note, the visual method of measuring wheel rotations and timing each one is subject to human inaccuracies :)

Video of the test is [here](https://youtube.com/shorts/RxdmSXZlh9c)

# Future Features

* Implement the DFLAG and LFLAG signals for the 3.3v shield via use of four additional TXU0101 chips.
* Create an abstract SPI class and then Arduino-specific SPI class implementation. Modify the LS7366 class to accept an abstract SPI "interface" class to make the LS7366 class independent of an specific host and generic to any SPI interface.
* Create an abstract Quadrature class and pass an instance of this to the Odometry class. This makes the Odometry class independent of how quadrature counts are obtained. Convert the existing QuadratureEncoder class to a LS7366-specific implementation class.
* Rename "getVelocity()" to just "velocity()".
* See note in ArduinoSPIOdometry.h. Both getVelocity() and radiansPerSecond() invoke getCounts(). Therefore, callers of these methods have to be careful when passing the time delta value to either if both are present in a program. We should extract the call to getCounts() to a public method named "update()" which syncs the current count values. Subsequent calls to either getVelocity() or radiansPerSecond() are passed the time delta of the last two calls of "update()" and simply calculate the odometry measurements based on the last updated count values.
* Update the 3.3 volt shield to expose the DFLAG and LFLAG outputs of the 5v LS7366 chip.