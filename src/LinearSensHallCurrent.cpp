#include "LinearSensHallCurrent.h"

#include <Arduino.h>


float LinearSensHallCurrent::getMaxCurrent(LinearSensHallCurrentType model) {
  switch (model) {
  case LinearSensHallCurrentType::WCS1500:
    return 200.0;
  case LinearSensHallCurrentType::WCS1600:
    return 100.0;
  case LinearSensHallCurrentType::WCS1700:
  case LinearSensHallCurrentType::WCS1800:
    return 70.0;
  case LinearSensHallCurrentType::WCS2800:
    return 30.0;
  case LinearSensHallCurrentType::WCS38A25:
    return 0.25;
  case LinearSensHallCurrentType::WCS6800:
    return 35.0;
  default:
    return 0;
  }
}

float LinearSensHallCurrent::getMinCurrent(LinearSensHallCurrentType model) {
  switch (model) {
  case LinearSensHallCurrentType::WCS1500:
    return -200.0;
    break;
  case LinearSensHallCurrentType::WCS1600:
    return -100.0;
    break;
  case LinearSensHallCurrentType::WCS1700:
  case LinearSensHallCurrentType::WCS1800:
    return -70.0;
  case LinearSensHallCurrentType::WCS2800:
    return -30.0;
  case LinearSensHallCurrentType::WCS38A25:
    return -0.25;
  case LinearSensHallCurrentType::WCS6800:
    return -35.0;
  default:
    return 0;
  }
}

float LinearSensHallCurrent::getZeroVuot(LinearSensHallCurrentType model, float vcc) {
  switch (model) {
  case LinearSensHallCurrentType::WCS1500:
  case LinearSensHallCurrentType::WCS1600:
  case LinearSensHallCurrentType::WCS1700:
  case LinearSensHallCurrentType::WCS1800:
  case LinearSensHallCurrentType::WCS2800:
  case LinearSensHallCurrentType::WCS6800:
  case LinearSensHallCurrentType::WCS38A25:
    return vcc / 2.0; // 2.5v at 5V
  default:
    return 0;
  }
}

float LinearSensHallCurrent::getVASensitivity(LinearSensHallCurrentType model, float vcc) {
  switch (model) {
  case LinearSensHallCurrentType::WCS1500:
    return vcc * 0.011 / 5.0; // 0.011V/A at 5V
  case LinearSensHallCurrentType::WCS1600:
    return vcc * 0.022 / 5.0; // 0.022V/A at 5V
  case LinearSensHallCurrentType::WCS1700:
    return vcc * 0.033 / 5.0; // 0.033V/A at 5V
  case LinearSensHallCurrentType::WCS1800:
    return vcc * 0.066 / 5.0; // 0.066V/A at 5V
  case LinearSensHallCurrentType::WCS2800:
    return vcc * 0.070 / 5.0; // 0.070V/A at 5V
  case LinearSensHallCurrentType::WCS38A25:
    return vcc * 7 / 5.0; // 7V/A at 5V
  case LinearSensHallCurrentType::WCS6800:
    return vcc * 0.065 / 5.0; // 0.065V/A at 5V
  default:
    return 0;
  }
}

LinearSensHallCurrent::LinearSensHallCurrent(LinearSensHallCurrentType model, uint8_t analogPinNum, float vcc, uint8_t adcBitResolution) {
  this->analogPinNum = analogPinNum;
  this->model = model;
  this->vcc = vcc;
  this->adcMaxVal = pow(2, adcBitResolution) - 1;
  this->lastRaw = 0;
  this->sampleNum = 1;
  this->sampleDelay = 0;
  
  this->extRawReadFn = 0;

  this -> initParameter();
}

 void LinearSensHallCurrent::setRawReadFn( int (*extRawReadFn)()){
	 this->extRawReadFn = extRawReadFn;
	 
 }

int LinearSensHallCurrent::readRaw(bool now) {
  if (now) {
    uint32_t acc = 0;
    for (uint8_t i = 0; i < this -> sampleNum; i++) {
      if(this->extRawReadFn == 0){
		acc += analogRead(this -> analogPinNum);
	  }
	  else{
		acc += this->extRawReadFn();
	  }
      if (this -> sampleDelay > 0) {
        delay(this -> sampleDelay);
      }
    }

    this -> lastRaw = acc / sampleNum;
  }
  return this -> lastRaw;
}

float LinearSensHallCurrent::getVcc() {
  return this -> vcc;
}

float LinearSensHallCurrent::getAdcMaxVal() {
  return this -> adcMaxVal;
}

void LinearSensHallCurrent::setSampleDelay(uint16_t ms) {
  this -> sampleDelay = ms;
}

void LinearSensHallCurrent::setSampleNumber(uint8_t num) {
  this -> sampleNum = num;
}

float LinearSensHallCurrent::readVoltage(bool now) {
  return this -> vcc / this -> adcMaxVal * this -> readRaw(now);
}

float LinearSensHallCurrent::readCurrent(bool now) {
  return (this -> vcc / this -> adcMaxVal * this -> readRaw(now) - this -> zeroVout) / this -> vASensitivity;
}

void LinearSensHallCurrent::setZeroVout(float v) {
  this -> zeroVout = v;
}

void LinearSensHallCurrent::setVASensitivity(float v) {
  this -> vASensitivity = v;
}

String LinearSensHallCurrent::readLog(bool now) {
  char buffer[60];
  sprintf(buffer, "Analog value: %d\nVoltage: %.4fV\nCurrent: %.4fA", this -> readRaw(now), this -> readVoltage(false), this -> readCurrent(false));
  return String(buffer);
}

void LinearSensHallCurrent::initParameter() {
  this -> zeroVout = LinearSensHallCurrent::getZeroVuot(this -> model, this -> vcc);
  this -> vASensitivity = LinearSensHallCurrent::getVASensitivity(this -> model, this -> vcc);
}