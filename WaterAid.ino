// Universum | Universum Projects > WaterAid

// Andrei Florian 30/MAY/2019

#define ARDUINO_MKR

#include <ArduinoLowPower.h>
#include <Universum_Logo.h>

#include "ring.h"
#include "sensors.h"
#include "time.h"
#include "gprs.h"

// Editable
volatile int mode = 1;          // mode ~ 1 = collect with button, 2 = collect at intervals
int deviceID = 1;               // custom ID for backend
String deviceName = "device1";  // custom name of device
int sleepTime = 450000;           // time to sleep between reads (applicable to [mode 1])

bool proDebug = false;           // debugging?

// Other
volatile int wokeUp = 0;
String waterState;

float waterTurbidity;
float waterPh;
float waterTemperature;

float atmoTemperature;
float atmoHumidity;

int currentEpoch;
float latitude;
float longitude;

const byte buttonPin = 4;

void prepareSample()
{
  wokeUp = 1;
}

void takeSample()
{
  Serial.println("Preparing Sample");
  Serial.println("________________________________________");
  
  Serial.println("Wake Up");
  Serial.println("  OK - Checking if just woke up");
  if(wokeUp == 1)
  {
    Serial.println("  OK - State is [Woke Up]");
  }
  else
  {
    Serial.println("  OK - State is [First Run]");
  }

  Serial.println("");
  Serial.println("Preparing Sample");

  Serial.println("  OK - Ready to Take Sample");
  Serial.println("  Warning - Place Probes in Water");
  
  // feedback
  colourLED(50);
  delay(1000);
  errorLED();
  delay(3000);
  colourLED(50);

  Serial.println("________________________________________");
  Serial.println("");

  Serial.println("Taking Sample");
  Serial.println("________________________________________");
  Serial.println("Taking Sample");
  
  Serial.println("  OK - Warming Up");
  delay(6000); // delay for sensor calibration
  colourLED(50);
  
  Serial.println("  OK - Taking Sample");
  Serial.print("  ");
  for(int i = 0; i < 16; i++)
  {
    if(mode == 1)
    {
      strip.setPixelColor(i, strip.Color(0, 255, 0));
      strip.show();
    }
    else
    {
      strip.setPixelColor(i, strip.Color(0, 0, 255));
      strip.show();
    }

    // going to take multiple water samples - sensors not that precise
    waterTurbidity += getWaterTurbidity();
    waterPh += getWaterPh();

    if(i > 14)
    {
      // take a single sample for high precision sensors
      waterTemperature = getWaterTemp();
      atmoTemperature = getAtmoTemp();
      atmoHumidity = getAtmoHumidity();
    }
    
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("  Success - Samples Taken");

  for(int i = 0; i <= 16; i++)
  {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
    strip.show();
    delay(30);
  }

  Serial.println("________________________________________");
  Serial.println("");
  delay(500);

  Serial.println("Processing Data");
  Serial.println("________________________________________");
  
  Serial.println("Processing Data");
  processData(); // add the data collected to global variables

  Serial.println("");
  Serial.println("Getting Base Credentials");
  getCredentials(); // get time, date, ID

  Serial.println("");
  Serial.println("Appending all Data to Dataline");
  String dataToSend = makeLine(); // compile the data into a line

  Serial.println("________________________________________");
  Serial.println("");

  Serial.println("Parsing Data");
  Serial.println("________________________________________");
  
  Serial.println("Parsing Data");
  parseData(dataToSend); // parse the data
  
  Serial.println("________________________________________");
  Serial.println("");
  delay(500);
}

String makeLine()
{
  Serial.println("  OK - Making String");
  String dataReturned; dataReturned += "{"; dataReturned += " \n";
  
  dataReturned += "\"Latitude\":"+ String(latitude, 7); dataReturned += ", \n";
  dataReturned += "\"Longitude\":"+ String(longitude, 7); dataReturned += ", \n";

  dataReturned += "\"waterpH\":"+ String(waterPh); dataReturned += ", \n";
  dataReturned += "\"waterTurbidity\":"+ String(waterTurbidity); dataReturned += ", \n";
  dataReturned += "\"waterTemp\":"+ String(waterTemperature); dataReturned += ", \n";

  dataReturned += "\"atmoTemp\":"+ String(atmoTemperature); dataReturned += ", \n";
  dataReturned += "\"atmoHum\":"+ String(atmoHumidity); dataReturned += ", \n";

  dataReturned += "\"deviceID\":"+ String(deviceID); dataReturned += ", \n";
  dataReturned += "\"deviceName\":"; dataReturned += String("\""); dataReturned += String(deviceName); dataReturned += String("\""); dataReturned += ", \n";
  dataReturned += "\"epoch\":"+ String(currentEpoch); dataReturned += ", \n";
  dataReturned += "\"mode\":"+ String(mode); dataReturned += " \n";

  dataReturned += "}";
  
  Serial.println("  OK - Data is below");
  Serial.println(""); 
  Serial.println(dataReturned);
  Serial.println(""); 
  
  Serial.println("  Success - String is Ready");
  colourLED(50);      
  return dataReturned; 
}

String getStatus()
{
  if(waterPh > 5.5 && waterPh < 8.5)
  {
    if(waterTurbidity > 85)
    {
      return "green";
    }
    else
    {
      return "red";
    }
  }
  else
  {
    return "red";
  }
}

void getCredentials()
{
  Serial.println("  [1/2] Time");
  Serial.println("  OK - Getting Time from RTC");
  currentEpoch = processTime();
  colourLED(50);
  
  Serial.println("  [2/2] Geolocation");
  Serial.println("  OK - Getting Geolocation from GPRS");
  while(!getLocation());
  Serial.print("  Success - Geolocation is "); Serial.print(latitude, 7); Serial.print(", "); Serial.println(longitude, 7);
  colourLED(50);
}

bool getLocation()
{
  if(location.available())
  {
    latitude = location.latitude();
    longitude = location.longitude();

    delay(500);
    return true;
  }
  else
  {
    delay(500);
    return false;
  }
}

void processData()
{
  Serial.println("  OK - Getting Mean of Water pH and Turbidity");
  waterPh = (waterPh / 16);
  waterTurbidity = (waterTurbidity / 16);

  Serial.println("  OK - Dumping Data to Serial");
  Serial.println("");

  Serial.print("  [Water] pH          "); Serial.println(waterPh);
  Serial.print("  [Water] Turbidity   "); Serial.println(waterTurbidity);
  Serial.print("  [Water] Temperature "); Serial.println(waterTemperature);
  
  Serial.print("  [Atmo]  Temperature "); Serial.println(atmoTemperature);
  Serial.print("  [Atmo]  Humidity    "); Serial.println(atmoHumidity);

  Serial.println("");
  Serial.println("  Success - Data Processed");
  colourLED(50); 
}

bool checkLocation(float latitude, float longitude)
{
  int rawLat = latitude;
  int rawLng = longitude;

  if(location.available())
  {
    if(rawLat == latitude && rawLng == longitude) // location is not precise
    {
      Serial.print(".");
      return false;
    }
    else
    {
      Serial.print(".");
      return true;
    }
  }
  else
  {
    return false;
  }
}

void setup()
{
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);

  if(proDebug)
  {
    while(!Serial) {};
    logoStart("WaterAid");
  }

  Serial.println("Initialising LED Ring");
  startLEDRing();
  Serial.println("  Success - LED Ring Online");

  Serial.println("Initialising Sensor");
  gy21.begin();
  Serial.println("  Success - Sensor Online");

  Serial.println("Initialising Temperature Module");
  soilSens.begin();
  Serial.println("  Success - Temperature Sensor Online");

  Serial.println("Initialising SIM Card");
  bool connected = false;
  
  while(!connected)
  {
    if((gsm.begin() == GSM_READY))
    {
      connected = true;
    }
    else
    {
      Serial.println("  Error - SIM Card Initialisation Failed");
      Serial.println("  OK - Trying again in 1 Second");
      errorLED();
      delay(1000);
    }
  }

  Serial.println("Attaching GPRS Module");
  while(gprs.attachGPRS(apn, login, password) == GPRS_READY) 
  {
    Serial.println("  Error - GPRS Module Responded with Error");
    delay(500);
  }
  Serial.println("  Success - Module Attached");
  
  /*
  Serial.println("Getting IMEI of Sim");
  Serial.println("  OK - Beginning Module");
  if(modem.begin()) 
  {
    Serial.println("  OK - Extracting IMEI");
    Serial.print("  OK - Got "); Serial.println(modem.getIMEI());
  }
  Serial.println("  Success - Extratced IMEI");
  */
  
  Serial.println("Attaching Lcation Services");
  location.begin();
  Serial.println("  OK - Services Online");

  Serial.println("  OK - Getting Location Fix");
  Serial.println("  Warning - This may take a couple of minutes");

  Serial.print("  ");
  while(!checkLocation(location.latitude(), location.longitude())) // wait until a lock is achieved
  {
    delay(500);
  }

  Serial.println("");
  Serial.println("  Success - Location is Fixed");
  
  colourLED(50);
  Serial.println("  OK - Enabling Location");
  location.begin();
  
  Serial.println("  Success - SIM Card Initialised");
  Serial.println("");
  
  Serial.println("Attaching Interrupts");
  if(mode == 1)
  {
    Serial.println("  OK - Device in [Mode 1]");
    colourLED(50);
  }
  else if(mode == 2)
  {
    Serial.println("  OK - Device in [Mode 2]");
    LowPower.attachInterruptWakeup(RTC_ALARM_WAKEUP, prepareSample, CHANGE);
    colourLED(50);
  }
  else
  {
    Serial.println("  Error - Inputted Ddevice Mode is not Valid");
    Serial.println("  Warning - Ensure that the 'mode' variable is set to '1' or '2'");
    Serial.println("  OK - Reset Device when Ready");
    while(1) {};
  }
  
  Serial.println("  Success - Interrupts Attached");
  colourLED(50);

  Serial.println("");
  Serial.println("");
  synchRTC();

  if(mode == 1)
  {
    while(1) 
    {
      if(digitalRead(buttonPin) == HIGH)
      {
        break;
      }
    }
  }
}

void loop()
{ 
  takeSample(); // take a sample
  Serial.println("");
  delay(1000);

  if(mode == 1)
  {
    while(1) 
    {
      if(digitalRead(buttonPin) == HIGH)
      {
        break;
      }
    }
  }
  else
  {
    LowPower.sleep(sleepTime);
  }
}
