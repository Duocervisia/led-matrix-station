1.	const char *bvgHost = "v5.bvg.transport.rest";
2.	 
3.	void getBvgData()
4.	{
5.	  Serial.print("connecting to "); Serial.println(bvgHost);
6.	  clientSecure.setInsecure();
7.	 
8.	  //connect to bvg api
9.	  if (clientSecure.connect(bvgHost, 443)) {
10.	    clientSecure.println(String("GET /stops/900063101/departures?subway=false&tram=false&bus=false&pretty=false&duration=30 HTTP/1.1\r\n") +
11.	                   "Host: " + bvgHost + "\r\nUser-Agent: ArduinoWiFi/1.1\r\n" +
12.	                   "Connection: close\r\n\r\n");
13.	  } else {
14.	    Serial.println("connection failed");
15.	    return;
16.	  }
17.	 
18.	  //wait till bvg is ready
19.	  int repeatCounter = 0;
20.	  while (!clientSecure.available() && repeatCounter < 50) {
21.	    delay(100);
22.	    Serial.println("w.");
23.	    repeatCounter++;
24.	  }
25.	  
26.	  //response contains headerinfo before showing json response, skip it
27.	  while (clientSecure.connected()) {
28.	    String line = clientSecure.readStringUntil('\n');
29.	    if (line == "\r") {
30.	      Serial.println("headers received");
31.	      for (byte i = 0; i < 6; i++) {
32.	        Serial.print((char)clientSecure.read());
33.	      }
34.	      break;
35.	    }
36.	  }
37.	 
38.	  //set DynamicJsonDocument size a bit smaller than ESP8266 MaxFreeBlockSize
39.	  DynamicJsonDocument root(ESP.getMaxFreeBlockSize()-1000);
40.	 
41.	  //seserialize Data
42.	  DeserializationError error = deserializeJson(root, clientSecure);
43.	  if (error) {
44.	    Serial.print(F("deserializeJson() failed: "));
45.	    Serial.println(error.f_str());
46.	    return;
47.	  }
48.	  clientSecure.stop();
49.	 
50.	  //set global departure variable to new String
51.	  departures = "S1: ";
52.	  for (byte i = 0; i < sizeof(root) - 1; i++) {
53.	 
54.	    //for each train, decide if it's the right direction and show time or canceled
55.	    if(root[i].as<String>() != NULL){
56.	      String platform = root[i]["platform"].as<String>();
57.	      if(platform == "2"){
58.	        String departure = root[i]["when"].as<String>();
59.	        if(departure == NULL){
60.	          departures += "canceled / ";
61.	        }else{
62.	          departures += departure.substring(11,16) + " / ";
63.	        }
64.	      }else if(platform == NULL){
65.	        departures += "canceled / ";
66.	      }
67.	    }
68.	  }
69.	  
70.	  departures = departures.substring(0, (departures.length()-3));
71.	  departures += "                ";
72.	}
