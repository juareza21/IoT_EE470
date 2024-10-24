#ifndef SENDREQUEST_H
#define SENDREQUEST_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

extern DHT dht;

void transmitData(float temperature, float humidity, const String& timestamp);

float readtemp();
float readhum();
String gettime(const String& timeZone);
void connectionDetails();
#endif
