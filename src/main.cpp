//
// ─── IMPORT LIBRARIES ───────────────────────────────────────────────────────────
//
#include <Arduino.h>
#include "DHT.h"
#include <SPI.h>
#include <WiFiNINA.h>
#include <BlynkSimpleWiFiNINA.h>

//
// ─── PARAMETER LED ──────────────────────────────────────────────────────────────
//

const int led_white = 7;

//
// ─── PARAMETER SENSORS ──────────────────────────────────────────────────────────
//
#define DHTPIN 8
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;

//
// ─── WIFI CREDENTIAL ────────────────────────────────────────────────────────────
//
char auth[] = "";
char ssid[] = "";
char pass[] = "";
int keyIndex = 0;

//
// ─── FUNCTION ───────────────────────────────────────────────────────────────────
//

void alertPushNotification(float sensor, int tempMax, String message)
{
  if (sensor > tempMax)
  {
    digitalWrite(led_white, HIGH);
    Blynk.notify(message);
  }
  else
  {
    digitalWrite(led_white, LOW);
  }
}

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);

  //
  // ─── PUSH NOTIFICATION ────────────────────────────────────────────────────────────
  //
  alertPushNotification(t, 28, "Alert - Temperature over 28C!");
  alertPushNotification(h, 50, "Alert - Humidity over 50%!");
}

//
// ─── SETUP ──────────────────────────────────────────────────────────────────────
//

void setup()
{
  Serial.begin(9600);
  pinMode(led_white, OUTPUT);
  Blynk.begin(auth, ssid, pass);

  dht.begin();

  timer.setInterval(1000L, sendSensor);
}

//
// ─── LAUNCH PROGRAMME ───────────────────────────────────────────────────────────
//

void loop()
{
  Blynk.run();
  timer.run();
}