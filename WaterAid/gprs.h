// Universum | Universum Projects > WaterAid | gprs

// Andrei Florian 30/MAY/2019

char apn[20] = "soracom.io";
char login[8] = "sora";
char password[8] = "sora";

char url[] = "harvest.soracom.io";
char path[] = "/";

void parseData(String dataToSend)
{
  Serial.println("  OK - Setting Up Connection");
  if(client.connect(url, 80))
  {
    Serial.println("  OK - Connection Established, Parsing Data");

    client.println("POST / HTTP/1.1");
    client.println("Host: harvest.soracom.io");
    client.println("User-Agent: Arduino/1.0");
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(dataToSend.length());
    client.println("");
    client.println(dataToSend);

    Serial.println("  OK - Data Parsed");
  }

  Serial.println("  OK - Getting Responce");
  Serial.println("");

  while(1)
  {
    if(client.available()) 
    {
      char c = client.read();
      Serial.print(c);
    }

    if(!client.connected()) 
    {
      break;
    }
  }

  Serial.println("  Success - Data is Parsed");
}
