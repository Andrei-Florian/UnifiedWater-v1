// Universum | Universum Projects > WaterAid | time

// Andrei Florian 30/MAY/2019

#include <RTCZero.h>

RTCZero rtc;
GSM gsm;
GPRS gprs;
GSMLocation location;
GSMClient client;
GSMModem modem;

int processTime() // a public loop to convert time to string
{
  String time;
  String date;
  String finalVal;
  
  if(rtc.getHours() < 10) time += "0";
  time += rtc.getHours();
  time += ":";
  if(rtc.getMinutes() < 10) time += "0";
  time += rtc.getMinutes();
  time += ":";
  if(rtc.getSeconds() < 10) time += "0";
  time += rtc.getSeconds();

  date += rtc.getDay();
  date += "/";
  date += rtc.getMonth();
  date += "/";
  date += rtc.getYear();

  rtc.getY2kEpoch();
  
  finalVal += date; finalVal += " "; finalVal += time;

  Serial.print("  OK - Current Time is "); Serial.println(finalVal);
  Serial.println("  Success - Got Epoch Time");
  int epoch = rtc.getEpoch();
  return epoch;
}

void synchRTC()
{
  Serial.println("Synching RTC");
  Serial.println("________________________________________");
  Serial.println("Getting GNSS Time");

  Serial.println("  OK - Getting Time from GNSS");
  double epoch = gsm.getTime();;
  colourLED(50);
  
  Serial.print("  OK - Epoch is         "); Serial.println(epoch);
  Serial.println("  Success - Time Set Complete");
  
  Serial.println("Initialising RTC with Time");
  Serial.println("  OK - Initialising RTC");
  rtc.begin();

  Serial.println("  OK - Setting Current Time");
  rtc.setEpoch(epoch);
  
  Serial.println("  Success - RTC Setup Complete");
  Serial.println("________________________________________");
  Serial.println("");
}
