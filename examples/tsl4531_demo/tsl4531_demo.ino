#include<Arduino.h>
#include<Wire.h>
#include<SPI.h>
#include<TSL4531.h>

TSL4531 tsl = TSL4531();

void setup() {
  while(!Serial) {
    delay(1000);
  }
  Serial.begin(9600);
  while (!tsl.begin()) {
    Serial.println("Can't find sensor!");
    delay(1000);
  }
}

void loop() {
  float reading = tsl.getLux();
  Serial.println(reading, 0);
  delay(1000);
}
