/*
 * TSL4531.h
 *
 * Library for interfacing with the TSL4531x ambient light sensor from ams.
 *
 * Author: Sean Caulfield <sean@yak.net>
 * License: GPL v3.0
 *
 */

#ifndef _TSL4531_H
#define _TSL4531_H

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif
#include <Wire.h>

// Can also be 0x39 if it's a '7 or '3 version. Not sure offhand the best way
// to handle that bit.
#define TSL45315_I2C_ADDR  0x29
#define TSL45317_I2C_ADDR  0x39

// Register addresses
// NB these all have been or'ed with 0x80 because for whatever reason, the
// datasheet specifies them as 0x00..0x0A, but seems you have to OR with 0x80
// or it doesn't work.
#define TSL4531_REG_CONTROL 0x80
#define TSL4531_REG_CONFIG  0x81
#define TSL4531_REG_DATA_L  0x84
#define TSL4531_REG_DATA_H  0x85
#define TSL4531_REG_DEVID   0x8A

// Control register (0x00)
#define TSL4531_CTRL_MODE_OFF       0x00
#define TSL4531_CTRL_MODE_ONESHOT   0x02
#define TSL4531_CTRL_MODE_NORMAL    0x03

// Config register (0x01)
#define TSL4531_CFG_PSAVESKIP       0x08
#define TSL4531_CFG_TCNTRL_400MS    0x00
#define TSL4531_CFG_TCNTRL_200MS    0x01
#define TSL4531_CFG_TCNTRL_100MS    0x02

// ID register (0x0A)
#define TSL4531_DEVID_TSL45317      0x80
#define TSL4531_DEVID_TSL45313      0x90
#define TSL4531_DEVID_TSL45315      0xA0
#define TSL4531_DEVID_TSL45311      0xB0

typedef enum {
  TSL4531_INT_TIME_400MS  = 0x00,
  TSL4531_INT_TIME_200MS  = 0x01,
  TSL4531_INT_TIME_100MS  = 0x02
} tsl4531_int_time_t;

typedef enum {
  TSL4531_MODE_OFF      = 0x00,
  TSL4531_MODE_ONESHOT  = 0x02,
  TSL4531_MODE_NORMAL   = 0x03
} tsl4531_mode_t;

class TSL4531
{
  public:
    TSL4531();
    TSL4531(uint8_t i2c_addr);
    bool begin(void);
    void setIntegrationTime(tsl4531_int_time_t newTime);
    void setMode(tsl4531_mode_t newMode);
    tsl4531_mode_t getMode(void);
    tsl4531_int_time_t getIntegrationTime(void);
    void setSaveSkip(bool s);
    bool getSaveSkip(void);
    uint16_t getReading(void);
    float getLux(void);
    uint8_t getDeviceId(void);

  protected:
    void write8(uint8_t reg);
    void write8(uint8_t reg, uint8_t val);
    uint16_t read16(uint8_t reg);
    uint8_t read8(uint8_t reg);

    uint8_t _i2c_addr;
    tsl4531_int_time_t _int_time;
    tsl4531_mode_t _mode;
    bool _saveSkip;
};

#endif
