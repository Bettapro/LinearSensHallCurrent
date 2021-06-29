#ifndef LINEARSENSHALLCURRENT_H
#define LINEARSENSHALLCURRENT_H

#include <Arduino.h>


typedef enum {
  WCS1500,
  WCS1600,
  WCS1700,
  WCS1800,
  WCS2800,
  WCS38A25,
  WCS6800
}
LinearSensHallCurrentType;

class LinearSensHallCurrent {
public:

  LinearSensHallCurrent(LinearSensHallCurrentType model, uint8_t analogPinNum, float vcc, uint8_t adcBitResolution);

  int readRaw(bool now = true);
  float getVcc();
  float getAdcMaxVal();
  void setSampleDelay(uint16_t ms);
  void setSampleNumber(uint8_t num);
  float readVoltage(bool now = true);
  float readCurrent(bool now = true);
  void setZeroVout(float v);
  void setVASensitivity(float v);
  String readLog(bool now = true);
  static float getMaxCurrent(LinearSensHallCurrentType model);
  static float getMinCurrent(LinearSensHallCurrentType model);
  static float getZeroVuot(LinearSensHallCurrentType model, float vcc);
  static float getVASensitivity(LinearSensHallCurrentType model, float vcc);

private:
  void initParameter();
  uint8_t analogPinNum;
  LinearSensHallCurrentType model;
  int lastRaw;
  uint8_t sampleNum;
  uint16_t sampleDelay;
  //
  float vcc;
  float adcMaxVal;
  //
  float zeroVout;
  float vASensitivity;
};

#endif;