#include "LS7366R.h"
#include <Arduino.h>
#include <SPI.h>

using namespace LS7366R;

#define SPI_SETTINGS_MAX_SPEED 500000

QuadratureEncoder::QuadratureEncoder(int enc_select, byte mdr0_config, byte mdr1_config) : m_enc_select(enc_select),
                                                                                           m_mdr0_config(mdr0_config),
                                                                                           m_mdr1_config(mdr1_config)
{
    byte counter_size = 0b00000011 & (~mdr1_config);
    m_counter_size = counter_size + 1;
}

void QuadratureEncoder::init()
{
    pinMode(m_enc_select, OUTPUT);
    digitalWrite(m_enc_select, HIGH);

    SPI.beginTransaction(SPISettings(SPI_SETTINGS_MAX_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(m_enc_select, LOW);
    SPI.transfer(WR | MDR0);
    SPI.transfer(m_mdr0_config);
    digitalWrite(m_enc_select, HIGH);
    SPI.endTransaction();

    SPI.beginTransaction(SPISettings(SPI_SETTINGS_MAX_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(m_enc_select, LOW);
    SPI.transfer(WR | MDR1);
    SPI.transfer(m_mdr1_config | MDR1_CNT_ENABLE);
    digitalWrite(m_enc_select, HIGH);
    SPI.endTransaction();

    SPI.beginTransaction(SPISettings(SPI_SETTINGS_MAX_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(m_enc_select, LOW);
    SPI.transfer(CLR | CNTR);
    digitalWrite(m_enc_select, HIGH);
    SPI.endTransaction();

    SPI.beginTransaction(SPISettings(SPI_SETTINGS_MAX_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(m_enc_select, LOW);
    SPI.transfer(CLR | STR);
    digitalWrite(m_enc_select, HIGH);
    SPI.endTransaction();
}

byte QuadratureEncoder::status()
{
    SPI.beginTransaction(SPISettings(SPI_SETTINGS_MAX_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(m_enc_select, LOW);
    SPI.transfer(RD | STR);
    byte enc_status = SPI.transfer(0x00);
    digitalWrite(m_enc_select, HIGH);
    SPI.endTransaction();
    return enc_status;
}

void QuadratureEncoder::clearCount()
{
    SPI.beginTransaction(SPISettings(SPI_SETTINGS_MAX_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(m_enc_select, LOW);
    byte b = SPI.transfer((byte)CLR | CNTR);
    digitalWrite(m_enc_select, HIGH);
    SPI.endTransaction();
}

long QuadratureEncoder::count()
{
    long count = 0;

    SPI.beginTransaction(SPISettings(SPI_SETTINGS_MAX_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(m_enc_select, LOW);
    byte b = SPI.transfer((byte)RD | CNTR);
    // highest byte comes first
    for (unsigned int i = 0; i < m_counter_size; i++)
    {
        byte count_byte = SPI.transfer(0x00);
        count <<= 8;
        count |= count_byte;
    }
    digitalWrite(m_enc_select, HIGH);
    SPI.endTransaction();

    return count;
}

bool QuadratureEncoder::statusCounterOverflow(byte status) {
    return STR_CY & status;
}

bool QuadratureEncoder::statusCounterUnderflow(byte status) {
    return STR_BW & status;
}

bool QuadratureEncoder::statusCounterEqualDtr(byte status) {
    return STR_CMP & status;
}

bool QuadratureEncoder::statusIndex(byte status) {
    return STR_IDX & status;
}

bool QuadratureEncoder::statusCountEnabled(byte status) {
    return STR_CEN & status;
}

bool QuadratureEncoder::statusPowerLoss(byte status) {
    return STR_PLS & status;
}

bool QuadratureEncoder::statusDirection(byte status) {
    // 0: count down, 1: count up
     return STR_UD & status;
} 

bool QuadratureEncoder::directionUp(byte status) {
    // 0: count down, 1: count up
    return (STR_UD & status) == true;
}

bool QuadratureEncoder::directionDown(byte status) {
    // 0: count down, 1: count up
    return (STR_UD & status) == false;
}

bool QuadratureEncoder::statusSign(byte status) {
    // 1: negative, 0: positive
     return STR_SIGN & status;
} 