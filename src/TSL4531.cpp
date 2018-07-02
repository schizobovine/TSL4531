/*
 * TSL4531.cpp
 *
 * Library for interfacing with the TSL4531x ambient light sensor from ams.
 *
 * Author: Sean Caulfield <sean@yak.net>
 * License: GPL v3.0
 *
 */

#include "TSL4531.h"

//////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////////////////////////////

TSL4531::TSL4531(void) : TSL4531(TSL45315_I2C_ADDR)
{}

TSL4531::TSL4531(uint8_t i2c_addr) :
  _i2c_addr(i2c_addr),
  _int_time(TSL4531_INT_TIME_100MS),
  _mode(TSL4531_MODE_NORMAL),
  _saveSkip(true)
{}

//////////////////////////////////////////////////////////////////////
// PUBLIC
//////////////////////////////////////////////////////////////////////

bool TSL4531::begin(void)
{
  Wire.begin();

  // Check for valid device on the i2c bus
  uint8_t devid = this->getDeviceId();
  switch (devid) {
    case TSL4531_DEVID_TSL45317:
    case TSL4531_DEVID_TSL45313:
    case TSL4531_DEVID_TSL45315:
    case TSL4531_DEVID_TSL45311:
      break;
    default:
      return false;
  }

  // Setup defaults so we can start capturing data
  this->setMode(this->_mode);
  this->setIntegrationTime(this->_int_time);

  return true;
}

void TSL4531::setIntegrationTime(tsl4531_int_time_t newTime)
{
  uint8_t data;

  if (this->_saveSkip) {
    data = TSL4531_CFG_PSAVESKIP;
  } else {
    data = 0;
  }

  switch (newTime) {
    case TSL4531_INT_TIME_400MS:
      data |= TSL4531_CFG_TCNTRL_400MS;
      break;
    case TSL4531_INT_TIME_200MS:
      data |= TSL4531_CFG_TCNTRL_200MS;
      break;
    case TSL4531_INT_TIME_100MS:
      data |= TSL4531_CFG_TCNTRL_100MS;
      break;
    default://wat
      data |= TSL4531_CFG_TCNTRL_400MS;
      break;
  }

  this->write8(TSL4531_REG_CONFIG, data);
  this->_int_time = newTime;
}

void TSL4531::setSaveSkip(bool s)
{
  // Only call intergration time setting routine if we are actually changing
  // state since this is somewhat side-effect-y
  if (this->_saveSkip != s) {
    this->setIntegrationTime(this->getIntegrationTime());
    this->_saveSkip = s;
  }
}

void TSL4531::setMode(tsl4531_mode_t newMode)
{
  uint8_t data = 0;

  switch (newMode) {
    case TSL4531_MODE_OFF:
      data = TSL4531_CTRL_MODE_OFF;
      break;
    case TSL4531_MODE_ONESHOT:
      data = TSL4531_CTRL_MODE_ONESHOT;
      break;
    case TSL4531_MODE_NORMAL:
      data = TSL4531_CTRL_MODE_NORMAL;
      break;
    default:
      break;
  }

  this->write8(TSL4531_REG_CONTROL, data);
  this->_mode = newMode;
}

tsl4531_mode_t TSL4531::getMode(void)
{
  return this->_mode;
}

tsl4531_int_time_t TSL4531::getIntegrationTime(void)
{
  return this->_int_time;
}

bool TSL4531::getSaveSkip(void)
{
  return this->_saveSkip;
}

uint16_t TSL4531::getReading(void)
{
  return this->read16(TSL4531_REG_DATA_L);
}

float TSL4531::getLux(void)
{
  uint16_t reading = this->getReading();
  uint8_t multiplier = 1;

  // Integration time affects the lux calculation
  switch (this->_int_time) {
    case TSL4531_INT_TIME_400MS:
      multiplier = 1;
      break;
    case TSL4531_INT_TIME_200MS:
      multiplier = 2;
      break;
    case TSL4531_INT_TIME_100MS:
      multiplier = 4;
      break;
    default://wat
      multiplier = 1;
      break;
  }

  return ((float)reading) * multiplier;
}

uint8_t TSL4531::getDeviceId(void)
{
  uint8_t devid = read8(TSL4531_REG_DEVID);
  devid &= 0xF0; //have to mask off lower bytes For Reasons (TM)
  return devid;
}

//////////////////////////////////////////////////////////////////////
// PROTECTED
//////////////////////////////////////////////////////////////////////

void TSL4531::write8(uint8_t reg)
{
  Wire.beginTransmission(this->_i2c_addr);
  Wire.write(reg);
  Wire.endTransmission();
}

void TSL4531::write8(uint8_t reg, uint8_t val)
{
  Wire.beginTransmission(this->_i2c_addr);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

uint16_t TSL4531::read16(uint8_t reg)
{
  uint8_t msb;
  uint8_t lsb;
  uint16_t data;

  Wire.beginTransmission(this->_i2c_addr);
  Wire.write(reg);
  Wire.endTransmission();
  
  Wire.requestFrom(this->_i2c_addr, (uint8_t)2);
  lsb = Wire.read();
  msb = Wire.read();

  data = ((uint16_t)msb) << 8;
  data |= lsb;

  return data;
}

uint8_t TSL4531::read8(uint8_t reg)
{
  uint8_t data;

  Wire.beginTransmission(this->_i2c_addr);
  Wire.write(reg);
  Wire.endTransmission();
  
  Wire.requestFrom(this->_i2c_addr, (uint8_t)1);
  data = Wire.read();

  return data;
}
