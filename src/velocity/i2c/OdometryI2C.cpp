#include "OdometryI2C.h"

// TODO: define a OdometryArduino class derived from Odometry
//  Consider converting "getCounts" to a pure virtual function
//  and then implement as necessary in the child classes. Move
//  this Arduino-specific code to that class

#include <Arduino.h>
#include <Wire.h>

OdometryI2C::OdometryI2C(
  unsigned short i2c_address) {
    this->m_i2c_address = i2c_address;
    this->m_last_count1 = 0;
    this->m_last_count2 = 0;
    this->m_current_count1 = 0;
    this->m_current_count2 = 0;
    this->m_receive_count_size = sizeof(receive_count1)+sizeof(receive_count2);
}

void OdometryI2C::begin(TwoWire *wirePort /*=&Wire*/, Stream *serialPort /*=NULL*/) {
  m_serial_port = serialPort;
  m_wire_port = wirePort;
}

void OdometryI2C::getCounts() {

  m_current_count1=0;
  m_current_count2=0;

  int bytesReturned = m_wire_port->requestFrom(m_i2c_address, m_receive_count_size, true);

  int i=0;
  while(m_wire_port->available()) {
    m_count_buffer[i++] = m_wire_port->read();
  }

  if (bytesReturned != m_receive_count_size) {
    if (NULL != m_serial_port) {
      m_serial_port->println("Error. Did not receive counts: "+String(bytesReturned));
    }
  }
  else {
    short count1 = *(short*)(m_count_buffer);
    short count2 = *(short*)(m_count_buffer+sizeof(receive_count1));

    m_current_count1 = (long)count1;
    m_current_count2 = (long)count2;
  }
}