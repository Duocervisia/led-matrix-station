#ifndef ENV_H
#define ENV_H

/*************************************Wifi Setup**************************************/

const char* ssid = "SSID"; // the name of the WiFi network
const char* password = "Password"; // the password of the WiFi network

/*************************************Weather API Setup**************************************/

const char* weatherKey = "OpenWeatherMap API key"; // the OpenWeatherMap API key
const char* weatherHost = "api.openweathermap.org"; // the OpenWeatherMap API endpoint
const char* longitute = "13.40"; // the longitude your location
const char* latitude = "52.52"; // the latitude your location

/*************************************NTP Setup**************************************/

static const char ntpServerName[] = "de.pool.ntp.org";    //Change this to a NTP server from your region https://www.ntppool.org/en/
// Central European Time (Frankfurt, Paris)
// Adapt to your timezone by changing the following values
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Time (Frankfurt, Paris)
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       //Central European Time (Frankfurt, Paris)

/*************************************BVG API Setup**************************************/

const char *bvgRequest = "https://v6.vbb.transport.rest/stops/900063101/departures?subway=false&tram=false&bus=false&pretty=false&duration=30"; // the BVG API endpoint
const char *stationPlattform = "2"; // the platform from which the to be displayed trains should depart

/*************************************Sensor Setup**************************************/

const int triggerSensorDistance = 100; // when the sensor detects an object closer than x cm, scroll the data

#endif // ENV_H