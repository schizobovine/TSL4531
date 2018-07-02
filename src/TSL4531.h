/*
 * TSL45315.h
 *
 * Library for interfacing with the TSL4531x ambient light sensor from ams.
 *
 * Author: Sean Caulfield <sean@yak.net>
 * License: GPL v3.0
 *
 */

#ifndef _TSL45315_H
#define _TSL45315_H

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif
#include <Wire.h>

#define TSL4531_I2C_ADDR  0x29

class TSL45315
{
  public:
    TSL45315(void);
    bool begin(void);

  protected:
    void write8(uint8_t reg);
    void write8(uint8_t reg, uint8_t val);
    uint16_t read16(uint8_t reg);
    uint8_t read8(uint8_t reg);
};

#endif
