#include "Arduino.h"
#include "SR04.h"
#include <ESP8266WiFi.h>                    
#include <ArduinoJson.h>                    // über Biblothek installieren
#include <Timezone.h>                       //https://github.com/JChristensen/Timezone
#include <TimeLib.h>                        //https://github.com/PaulStoffregen/Time
#include <WiFiUdp.h>


#define TRIG_PIN 2
#define ECHO_PIN 5
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long distance;


/************************************(Wifi Einstellungen)**********************************/
const char* ssid     = "WLAN";     // SSID of local network
const char* password = "20012017";   // Password on network

/**************************(openweathermap.org Einstellungen)*******************************/
String weatherKey = "a1a26f8e76a1823c72d29fc9dec54109";

String weatherLang = "&lang=en";
String cityID = "berlin,de"; //Biberist


// NTP Serverpool für die Schweiz:
static const char ntpServerName[] = "ch.pool.ntp.org";    //Finde lokale Server unter http://www.pool.ntp.org/zone/ch
const int timeZone = 0;                     // 0 wenn mit <Timezone.h> gearbeitet wird sonst stimmt es nachher nicht
WiFiUDP Udp;
unsigned int localPort = 8888;              // lokaler Port zum Abhören von UDP-Paketen
time_t getNtpTime();
void digitalClockDisplay();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);

// - Timezone. - //
// Bearbeiten Sie diese Zeilen entsprechend Ihrer Zeitzone und Sommerzeit.
// TimeZone Einstellungen Details https://github.com/JChristensen/Timezone
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Time (Frankfurt, Paris)
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       //Central European Time (Frankfurt, Paris)
Timezone CE(CEST, CET);

WiFiClient client;
/********************( Definieren der Globale Variablen und Konstanten)********************/
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

String WTemp, Datum, Zeit;
String weatherString, departures;

#define NUM_MAX 4

// for ESP-01 module
//#define DIN_PIN 2 // D4
//#define CS_PIN  3 // D9/RX
//#define CLK_PIN 0 // D3

// for NodeMCU 1.0
#define DIN_PIN 15  // D8
#define CS_PIN  13  // D7
#define CLK_PIN 12  // D6

#include "max7219.h"
#include "fonts.h"


bool bNTPStarted = false;
bool gbIsConnecting = false;

#define MAX_DIGITS 16
byte dig[MAX_DIGITS] = {0};
byte digold[MAX_DIGITS] = {0};
byte digtrans[MAX_DIGITS] = {0};
int updCnt = 0;
int dots = 0;
long dotTime = 0;
long clkTime = 0;
long clkBvgTime = 0;
int dx = 0;
int dy = 0;
byte del = 0;
int h, m, s, w, mo, ye, d;
const char* wochenstring;
float utcOffset = 2;
long localEpoc = 0;
long localMillisAtUpdate = 0;
bool bStart = true;


/*****************************************( Setup )****************************************/
void setup()
{
  Serial.begin(9600);
  delay(500);

  initMAX7219();

  sendCmdAll(CMD_SHUTDOWN, 1);
  sendCmdAll(CMD_INTENSITY, 0);
  Serial.print("Verbinde mit WiFi ");
  WiFi.begin(ssid, password);
  printStringWithShift("Verbinde", 15);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Verbunden: "); Serial.println(WiFi.localIP());

  Udp.begin(localPort);
  Serial.print("lokaler Port: ");
  Serial.println(Udp.localPort());
  Serial.println("Warten auf die Synchronisation");
  setSyncProvider(getNtpTime);
  setSyncInterval(86400);                    // Anzahl der Sekunden zwischen dem erneuten Synchronisieren ein. 86400 = 1 Tag
}
/*************************************(Hauptprogramm)**************************************/
void loop()
{
  distance=sr04.Distance();

  if (millis() - clkTime > 900000 || bStart){ //update weather every 15 min
    Serial.println("Verbinde ...");
    getWeatherData();
    getTimeLocal();
    Serial.println("Daten geladen");
    clkTime = millis();
  }
  if (millis() - clkBvgTime > 120000 || bStart){ //update bvg every 2 min
    bStart = false;
    Serial.println("Lade BVG...");
    getBvgData();
    Serial.println("Daten geladen");
    clkBvgTime = millis();
  }

  Serial.println(distance);


  if (distance < 100) { //distance smaler than given centimeters
    String combinedString = weatherString + " " + departures+ "                ";
    printStringWithShift(combinedString.c_str(), 30);
  }

//  if (millis() - clkTime > 15000 && !del && dots) { // clock for 15s, then scrolls for about 30s
//    printStringWithShift(weatherString.c_str(), 30);
//    updCnt--;
//    clkTime = millis();
//  }
  if (millis() - dotTime > 500) {
    dotTime = millis();
    dots = !dots;
  }
  getTimeLocal();
  showAnimClock();
}
