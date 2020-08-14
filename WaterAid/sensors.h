// Universum | Universum Projects > WaterAid | sensors

// Andrei Florian 30/MAY/2019

#include <Wire.h>
#include "SparkFunHTU21D.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// Create Objects
OneWire oneWire(3);
DallasTemperature soilSens(&oneWire);
HTU21D gy21;

float calibration = 2.30; // the turbidity as a voltage in clean water

float getAtmoTemp()
{
  return gy21.readTemperature();
}

float getAtmoHumidity()
{
  return gy21.readHumidity();
}

float getWaterTemp()
{
  soilSens.requestTemperatures();
  return soilSens.getTempCByIndex(0);
}

float getWaterTurbidity()
{
  int raw = analogRead(A1);
  float volt = raw / 1024.0 * 3.3;
  float percentage = (volt / calibration) * 100.00;

  return percentage;
}

float getWaterPh()
{
  int raw = analogRead(A2);
  float volt = raw / 1024.0 * 3.3;
  float ph = 7 + ((2.5 - volt) / 0.18);

  return ph;
}
