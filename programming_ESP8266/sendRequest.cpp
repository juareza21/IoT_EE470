/*
 * ----------------------------------------------
 * Project/Program Name : programming_ESP8266
 * File Name            : sendRequest.cpp
 * Author               : Alex Juarez
 * Date                 : 23/10/2024
 * Version              : Version: 1.94.2 (Universal)
 * 
 * Purpose:
 *    The purpose of this program is to execute the required function to fulfill the tasks of the main.cpp program, such as
 *    fetching the tmperature, humidity, and time using the provided url.
 * 
 * Inputs:
 *      temp, hum: used to contain the values of the temperature and humidty respectively.
 *      timeZ: used for finding the timezone the user input into the program.
 * 
 * Outputs:
 *    Collected data such as teh temperature, humidity, and the date and time
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
 *    Change the url to be able to insert into the desired database, as well as re-ordering the variables of 
 *    "node name", time, temperature and humidty as needed to be added into the database you are trying to add to.
 * 
 * ---------------------------------------------------------------------------
 */

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <sendRequest.h> // required header file
#include <Adafruit_Sensor.h>
#include <DHT.h>

DHT dht (D2, DHT11);

void connectionDetails()
{
  // Details of the connection
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer
  Serial.print("Channel:\t");
  Serial.println(WiFi.channel());          
  Serial.print("RSSI:\t");
  Serial.println(WiFi.RSSI());   
  Serial.print("DNS IP:\t");
  Serial.println(WiFi.dnsIP(1));            // get the DNS IP Address    
  Serial.print("Gateway IP:\t");
  Serial.println(WiFi.gatewayIP()); 
  Serial.println("---------------------");    
}

float readtemp(){
    float temp = dht.readTemperature();
    if(isnan(temp)) {
        Serial.println("Failed temp read");
        return -1;
    }
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" °C");
    return temp;
}

float readhum(){
    float hum = dht.readHumidity();
    if(isnan(hum)) {
        Serial.println("Failed humidity read");
        return -1;
    }
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.println(" %");
    return hum;
}

String gettime(const String& timeZ){
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient https;

    String timeurl = "https://timeapi.io/api/Time/current/zone?timeZone=" + timeZ;
    https.begin(client, timeurl);
    int response = https.GET();
    if (response == 200){
        String data = https.getString();

        DynamicJsonDocument jsonDoc(400);
        DeserializationError error = deserializeJson(jsonDoc, data);
        if (error) {
            Serial.print("JSON data error: ");
            Serial.println(error.c_str());
            return "";
        }
        String date = jsonDoc["dateTime"];
        date.replace("T", " ");
        int dotI = date.indexOf('.');
        if (dotI != -1) {
            date = date.substring(0, dotI);
    }
    Serial.println("Current_Time: " + date);
    return date;
}
else {
    Serial.print("Error fetching time");
    return "";
}
https.end();
}

void transmitData(float temp, float hum, const String& timestamp) {
    WiFiClientSecure client;
    HTTPClient https;
    client.setInsecure(); // Bypass SSL certificate validation

    // Encode timestamp
    String encodedTime = timestamp;
    encodedTime.replace(" ", "%20");
    encodedTime.replace(":", "%3A");

    // Construct the URL
    String url = "https://juareza.net/insert.php";
    url += "?node_name=node-1";
    url += "&time_received=" + encodedTime;
    url += "&temperature=" + String(temp);
    url += "&humidity=" + String(hum);

    Serial.println("URL: " + String(url));

    // GET request
    https.begin(client, url);
    int httpResponse = https.GET();

    // print response code: e.g.,:200
    Serial.println("Response code <-- " + String(httpResponse));
    if (httpResponse == 200) {
        String response = https.getString();
        Serial.print("Server response: ");
        Serial.println(response);
    } else {
        Serial.println("Failed to send data");
    }
    // Close the connection
    https.end(); 
}
