/*
 * ----------------------------------------------
 * Project/Program Name : programming_ESP8266
 * File Name            : main.cpp
 * Author               : Alex Juarez
 * Date                 : 23/10/2024
 * Version              : Version: 1.94.2 (Universal)
 * 
 * Purpose:
 *    The purpose of this program is to take in the temperature and humidity of the sensor's surroundings and record it and add
 *    into the Users database. This program also fetches the time that the reading was captured at.
 * 
 * Inputs:
 *    D1: Switch reading to tell the program when to fetch data
 *    D2: Temperature and humidity value readings
 *    timeZ: The input of the time zone that was selected by the user
 * 
 * Outputs:
 *    Collected data that is sent into the database.
 * 
 * Example Application:
 *    In Iot design this sort of design is used to capture readings of certain surroundings without having to be there in person
 *    such as monitoring the conditions of a park using a temperature sensor and collecting data over time to make a graph of
 *    how temperature changes over time.
 * 
 * Dependencies:
 *    Esternal:
 *     * Arduino.h
 *     * ArduinoJson.h
 *     * ESP8266WiFi.h
 *     * ESP8266HTTPClient.h
 *     * DHT.h
 *     * Adafruit_Sensor.h
 *    Hardware:
 *     * ESP8266
 *     * DHT11
 *     * Switch
 * 
 * Usage Notes:
 *    Make sure to change the "ssid" variable to the WiFi name you are trying to connect, the "password" variable to the
 *    password of that WiFI.
 *    Also change the durl to be able to insert into the desired database, as well as re-ordering the variables of 
 *    "node name", time, temperature and humidty as needed to be added into the database you are trying to add to.
 * 
 * ---------------------------------------------------------------------------
 */


#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <sendRequest.h> // required header file
#include <DHT.h>
#include <Adafruit_Sensor.h>
//#include <WiFiClientSecure.h> // not needed

const char* ssid     = "SEJW"; // must be exactly as it apears - case sensitive
const char* password = "bob12345";
int i;

String url     = "https://juareza.net/insert.php";

const char* timeZone[] = {"America/New_York", "America/Chicago", "America/Danver", "America/Los_Angeles", "America/Anchorage", "Pacific/Honolulu", "Americsa/Puerto_Rico"};

//Default time zone is Los Angeles
String timeZ = "America/Los_Angeles";

void setup () {
  Serial.begin(9600);
  delay(100); // just a short delay
  Serial.println("");
  
  // (A) Start connecting to the WiFI
  Serial.println("Connecting to WiFi"); 
  WiFi.begin(ssid, password); // Connection to WiFi Starts until status()=WL_CONNECTED
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print("."); // waiting to get connected
  }
  // Details of the connection
   connectionDetails();


dht.begin();

pinMode(D1, INPUT_PULLUP);

Serial.println("Select Your Time Zone (The default time is PT):");
Serial.println("1) Eastern Time Zone (ET) - New York City, NY");
Serial.println("2) Central Time Zone (CT) - Chicago, IL");
Serial.println("3) Mountain Time Zone (MT) - Denver, CO");
Serial.println("4) Pacific Time Zone (PT) - Los Angeles, CA");
Serial.println("5) Alaska Time Zone (AKT) - Anchorage, AK");
Serial.println("6) Hawaii-Aleutian Time Zone (HAT) - Honolulu, HI");
Serial.println("7) Atlantic Time Zone (AT) - San Juan, PR");
Serial.print("-> Enter a number between 1-7 to select your time zone: ");

while (!Serial.available()){
  delay(100);
}

int select = Serial.parseInt();

Serial.println(select);

if (select >= 1 && select <= 7) {
  timeZ = timeZone[select - 1];
  Serial.print("the time zone is: ");
  Serial.println(timeZ);
}
else {
  Serial.println("Not a valid time zone selection, the default time zone will be used instead (Los Angeles, CA)");
}
}


// void loop() {
//   if (WiFi.status() == WL_CONNECTED) {
//     WiFiClientSecure client;
//     client.setInsecure();
//     HTTPClient https;

//     // For testing only

//     String fullUrl = url; // preparing the full URL
//     i = 0;
//     while (i == 0) {
//     Serial.println("Requesting: --> " + fullUrl);
//     if (https.begin(client, fullUrl)) { // start the connection 1=started / 0=failed

//       int httpCode = https.GET(); // choose GET or POST method
//       //int httpCode = https.POST(fullUrl); // need to include URL
      
//       Serial.println("Response code <--: " + String(httpCode)); // print response code: e.g.,:200
//       if (httpCode == 200) {
//         //Serial.println(https.getString()); // this is the content of the get request received
//         i = 1;
//       }

//      else{
//         Serial.printf("[HTTPS] Unable to connect\n");
//     }
//   }}
//   https.end(); // end of request
//   }
//   delay(5000); // delay between each REQUEST to the server

// }

void loop() {
  // Check if the button is pressed
  if (digitalRead(D1) == LOW) {
    Serial.println("Button pressed.");
    delay(200);  // Small debounce delay

    // Read the temperature, humidity, and light level
    float temperature = readtemp();
    float humidity = readhum();

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read sensor data.");
    } else {
      // Get current time
      String currentTime = gettime(timeZ);
      // Transmit data
      transmitData(temperature, humidity, currentTime);
      Serial.println();
    }

    // Wait for button release
    while (digitalRead(D1) == LOW) {
      yield(); // avoid watchdog timer resetting the processor
    }
  }
  delay(400); // Short delay to debounce input reading
}
