#include "Arduino.h"
#include "SR04.h"
#include <ESP8266WiFi.h>                    
#include <ArduinoJson.h>                    // über Biblothek installieren
#include <Timezone.h>                       //https://github.com/JChristensen/Timezone
#include <TimeLib.h>                        //https://github.com/PaulStoffregen/Time
#include <WiFiUdp.h>

#include "env.h"

#define TRIG_PIN 2
#define ECHO_PIN 5
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long distance;

// NTP Serverpool für die Schweiz:
const int timeZone = 0;                     // 0 wenn mit <Timezone.h> gearbeitet wird sonst stimmt es nachher nicht
WiFiUDP Udp;
unsigned int localPort = 8888;              // lokaler Port zum Abhören von UDP-Paketen
time_t getNtpTime();
void digitalClockDisplay();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);


Timezone CE(CEST, CET);

WiFiClient client;

String weatherString, departures;

#define NUM_MAX 4

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

  if (distance < triggerSensorDistance) { //distance smaler than given centimeters
    String combinedString = weatherString + " " + departures+ "                ";
    printStringWithShift(combinedString.c_str(), 30);
  }

  if (millis() - dotTime > 500) {
    dotTime = millis();
    dots = !dots;
  }
  getTimeLocal();
  showAnimClock();
}
