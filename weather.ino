String weatherMain = "";
String weatherDescription = "";
String weatherLocation = "";
String country;
int humidity;
int pressure;
float temp;
float tempMin, tempMax;
int clouds;
float windSpeed;
float rain;
float snow;
int windDeg;
String date;

String WTemp, Datum;

String weatherLang = "&lang=en";

void getWeatherData()
{
  Serial.print("connecting to "); Serial.println(weatherHost);
  if (client.connect(weatherHost, 80)) {
    client.println(String("GET /data/2.5/onecall?lat="+String(latitude) +"&lon="+String(longitute) +"&units=metric&exclude=hourly,minutely&appid=") + weatherKey + weatherLang + "\r\n" +
      "Host: " + weatherHost + "\r\nUser-Agent: ArduinoWiFi/1.1\r\n" +
      "Connection: close\r\n\r\n");
  } else {
    Serial.println("connection failed");
    return;
  }

  int repeatCounter = 0;
  while (!client.available() && repeatCounter < 10) {
    delay(500);
    Serial.println("w.");
    repeatCounter++;
  }
  Serial.println(client.connected());
  Serial.println(client.available());

  DynamicJsonDocument root(6144);

  DeserializationError error = deserializeJson(root, client);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  client.stop();

  //weatherMain = root["weather"]["main"].as<String>();
  weatherDescription = root["current"]["weather"][0]["description"].as<String>();
  weatherDescription.toLowerCase();
  //  weatherLocation = root["name"].as<String>();
  //  country = root["sys"]["country"].as<String>();
  temp = root["current"]["temp"];
  tempMax = root["daily"][0]["temp"]["max"];

  humidity = root["current"]["humidity"];
  pressure = root["current"]["pressure"];
  //  tempMin = root["main"]["temp_min"];
  //  tempMax = root["main"]["temp_max"];
  windSpeed = root["current"]["wind_speed"];
  windSpeed *= 3.6;
  windDeg = root["current"]["wind_deg"];
  clouds = root["current"]["clouds"];
  rain = root["current"]["rain"];
  snow = root["current"]["snow"];

  getTimeLocal();

  String windDirection = "N NOO SOS SWW NW";  // Windrichtungen N NO O SO S SW W NW immer 2 char lang
  int wr = (windDeg+22)%360/45;
  //  Serial.println(wr);

  String dayName[] = {"Err", "Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};
  
  String deg = String(char('~' + 25));
  String arrowUp = String(char('~' + 23));
  
  //weatherString = "       " + dayName[w]  + "  " + String(d) + "." + String(mo) + "." + String(ye)+ "      ";
  weatherString = "       " +  String(temp, 1) + deg + "C ";
  weatherString += "  "+ arrowUp + ": "+  String(tempMax, 1) + deg + "C ";
  // weatherString += "  Luftf.: " + String(humidity) + "%  ";  // Luftfeuchtigkeit
  // weatherString += "  Luftd: " + String(pressure) + "hPa  "; // Luftdruck
  // weatherString += "  Weather: " + String(weatherDescription); // Regenrisiko
  // if(snow != 0.00){
  //   weatherString += "  Snow: " + String(snow, 1) + " mm"; // Regenrisiko
  // }else if(rain != 0.00){
  //   weatherString += "  Rain: " + String(rain, 1) + " mm"; // Regenrisiko
  // }else{
  //   weatherString += "  Clouds: " + String(clouds) + " %"; // Regenrisiko
  // }
  //  weatherString += "  Wind: " + String(windSpeed, 1) + "m/s                "; // Windgeschwindigkeit
  // weatherString += "  Wind: " + String(windSpeed, 1) + " km/h" + "                "; // Windgeschwindigkeit
  //weatherString += String(windDeg) + deg + "  ";
  //weatherString +=  arrowUp + "  " + windDirection.substring(2*wr,2*wr+2) + "                "; // Windgeschwindigkeit und Richtung
  Serial.println(weatherString);  //Leh
}