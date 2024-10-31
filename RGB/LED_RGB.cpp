#include <Arduino.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <bits/stdc++.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <connect.h> // required header file
using namespace std;

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

String readresults(String url)
{
    WiFiClientSecure client;
    HTTPClient https;
    client.setInsecure();

    Serial.println("URL: "+ String(url));
    
    https.begin(client, url);
    int httpsResponse = https.GET();

    if (httpsResponse == 200) {
        String data = https.getString();
        Serial.print("Data: ");
        Serial.println(data);
        return data;
    } 
    else {
    Serial.print("Error fetching data ");
    String response = https.getString();
    Serial.print("Server response: ");
    Serial.println(response);
    return "";
    }
    https.end();
}

int* readRGB(String url){
  static int rgb[3];
  WiFiClientSecure client;
  HTTPClient https;
  client.setInsecure();

  Serial.println("URL: " + String(url));

  https.begin(client, url);
  int httpsResponse = https.GET();

  if (httpsResponse == 200){
    String data = https.getString();
    Serial.println("Data: ");
    Serial.println(data);
    rgb[3] = {1};

    char charArray[data.length() + 1];
    strcpy(charArray, data.c_str());

    Serial.println(charArray);

    std::stringstream ss(charArray);
    std::vector<int> intArray; // Use vector for dynamic sizing
    int number;

    // Parse integers from the string
    while (ss >> number) {
        intArray.push_back(number);
    }

  for (int idx = 0; idx < 3; idx++) {
  rgb[idx] = intArray[idx];
  Serial.println(rgb[idx]);
  } 
    return rgb;
  }
  else {
    Serial.println("Error getting data "); 
      String response = https.getString();
      Serial.print("Server response: ");
      Serial.println(response);
        return 0;
  }
}
