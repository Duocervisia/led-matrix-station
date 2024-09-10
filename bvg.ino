#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

void getBvgData() {
  Serial.println("Lade BVG...");
  Serial.print("connecting to "); 

  String answer = httpGETRequest(bvgRequest);
  Serial.println("Daten geladen");

    //set DynamicJsonDocument size a bit smaller than ESP8266 MaxFreeBlockSize
  DynamicJsonDocument root(ESP.getMaxFreeBlockSize()-1000);
  
  //seserialize Data
  DeserializationError error = deserializeJson(root, answer);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  //set global departure variable to new String
  departures = "";
  for (byte i = 0; i < sizeof(root["departures"]) - 1; i++) {
    //for each train, decide if it's the right direction and show time or canceled
    if(root[i].as<String>() != NULL){
      String platform = root["departures"][i]["plannedPlatform"].as<String>();
      if(platform == stationPlattform){
        String departure = root["departures"][i]["when"].as<String>();
        if(departure == NULL){
          departure = root["departures"][i]["plannedWhen"].as<String>();
          departures += departure.substring(11,16) + " / ";
        }else{
          departures += departure.substring(11,16) + " / ";
        }
      }
    }
    Serial.println(departures);
  }
  if(departures.length() > 0){
    departures = departures.substring(0, (departures.length()-3));
  }else{
    departures = "No trains running";
  }
  
  departures = String(stationName) + ": " + departures;
}

String httpGETRequest(const char* serverName) {
  WiFiClientSecure client;
  HTTPClient http;

  // Debugging output for WiFi connection
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected!");
    return "{}";
  }
  
  client.setInsecure(); // Use this only for debugging purposes, not recommended for production
  Serial.println("Starting connection to server...");
  
  // Your IP address with path or Domain name with URL path 
  if (!http.begin(client, serverName)) {
    Serial.println("Failed to begin HTTP connection");
    return "{}";
  }
  
  // Send HTTP GET request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    // Read payload in chunks
    WiFiClient *stream = http.getStreamPtr();
    payload = "";
    while (stream->available()) {
      char buffer[128];
      int len = stream->readBytes(buffer, sizeof(buffer) - 1);
      buffer[len] = '\0';
      payload.concat(buffer); // Use concat to handle UTF-8 strings correctly
    }
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  
  // Free resources
  http.end();

  return payload;
}