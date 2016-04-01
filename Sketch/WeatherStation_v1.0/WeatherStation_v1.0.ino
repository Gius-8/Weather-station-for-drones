/*  WEATHER STATION FOR DRONES V1.0
    POWERED by GIUSEPPE CACCAVALE
    www.giuseppecaccavale.it

    Compatible with Arduino/Genuino MKR1000
    or Arduino Zero + Arduino Wifi Shield 101

    GitHub repository: https://github.com/Gius-8/Weather-station-for-drones

*/

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Scheduler.h>
#include <SFE_BMP180.h>
#include <WiFi101.h>
#include <TinyGPS++.h>
#include "DHT.h"

// PIN used by MKR1000
#define PIN_SD 7
#define DHTPIN 2
const byte BUTTON_PIN = 1; //Start button
const byte SWITCH_WIFI = 4; //Switch wifi
const byte ledStart = 5;
const byte ledError = 3;
const byte ledGPS = 6;
/////////////////////////////////////////////

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

SFE_BMP180 pressure;

File myFile; //File .KML

static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps; // The TinyGPS++ object

// Variables for debounce function
byte StateSwitch = 0;
int ButtPush;
int state = LOW;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = HIGH;   // the previous reading from the input pin
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

// Variables for Wifi function
char ssidAP[] = "WeatherStation"; // created AP name
int status = WL_IDLE_STATUS;
WiFiServer server(80);

// Variables for KML file
boolean WriteEncoding = false;
boolean WriteFolder = false;
boolean CreateFile = false;
int num = 0;
String file = "file";
String Combine = "";

// Variables in general
double lastLAT , lastLNG;
char Status;
double T, P, p0;
float t, h;
double lat, lng, speed, ALTITUDE;
int year;
byte month, day, hour, minute, second;
String DateString = "";
int i = 0;
boolean changeLocation = false;
boolean GPSstatus = false;

void setup() {

  file.reserve(10); //Reserve 10 byte for file
  Combine.reserve(20); //Reserve 20 byte for Combine
  DateString.reserve(30); //Reserve 60 byte for DateString

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(SWITCH_WIFI, INPUT_PULLUP);
  pinMode(ledStart, OUTPUT);
  pinMode(ledGPS, OUTPUT);
  pinMode(ledError, OUTPUT);
  digitalWrite(ledError, LOW);

  Scheduler.startLoop(loop1); // Initializes loop1

  Serial.begin(9600);
  Serial1.begin(GPSBaud);

  dht.begin(); // Initializes dht sensor
  pressure.begin(); // Initializes BMP180 sensor

  init_WifiAP(); // Initializes Wifi MKR1000

  if (!SD.begin(PIN_SD)) { // Initializes SD
    Serial.println("SD: initialization failed :(");
    digitalWrite(ledError, HIGH);
    while (true);
  }
  Serial.println("SD: initialization done :)");

}

void loop() {

  StateSwitch = digitalRead(SWITCH_WIFI);

  if (StateSwitch == LOW) {
    getBMP180data();
    getDHTdata();

    t = ((float)T + t) / 2;

    if (GPSstatus) {

      switch (ButtPush) {
        case LOW:
          writeKmlFile();
          break;
        case HIGH:
          if (CreateFile && WriteEncoding && WriteFolder) {

            close_myFile();
            reset_Variables();
            digitalWrite(ledStart, LOW); // led Status
            Serial.println("CLOSE SD.");
            Serial.println("STOP WRITING");
          }
          else {
            Serial.println("Press the button to start writing.");
          }
          break;
      }
    }
    else {
      Serial.println("GPS Signal Wait");
    }
    delay(5000);
  }
  else if (StateSwitch == HIGH) {
    getBMP180data();
    getDHTdata();

    t = ((float)T + t) / 2;

    if (CreateFile && WriteEncoding && WriteFolder) {
      close_myFile();
      reset_Variables();
      digitalWrite(ledStart, LOW); // led Status
      Serial.println("CLOSE SD.");
      Serial.println("STOP WRITING");
    }
    wifiLoop();
  }

}

void loop1() {
  if(StateSwitch == LOW) ButtPush = debounce(BUTTON_PIN);

 /* if (ButtPush == HIGH && GPSstatus && StateSwitch == LOW)
    digitalWrite(ledStart, LOW);
  else if (ButtPush == LOW && GPSstatus && StateSwitch == LOW)
    digitalWrite(ledStart, HIGH);*/

  while (Serial1.available() > 0)
    if (gps.encode(Serial1.read()))
      getGPSdata();

  if (GPSstatus)
    digitalWrite(ledGPS, HIGH);
  else
    digitalWrite(ledGPS, LOW);

  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
    digitalWrite(ledError, HIGH);
    while (true);
  }

  yield();
}
