#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <connect.h> // required header file

int RGB[3];
String LEDstatus = "on";

int RED;
int GREEN;
int BLUE;

// put function declarations here:
const char* ssid     = "BHAGateway"; // must be exactly as it apears - case sensitive
const char* password = "Rocio12191966";

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

   pinMode(D1, OUTPUT);
   pinMode(D2, OUTPUT);
   pinMode(D3, OUTPUT);
   pinMode(D7, OUTPUT);

}
void loop() {
  // Check if the button is pressed
  LEDstatus = readresults("https://juareza.net/dashboard/results.txt");
  Serial.println();

  int* RGBvalue = readRGB("https://juareza.net/dashboard/results2.txt");

  if (RGBvalue != NULL){
    for (int i = 0; i < 3; i++){
      RGB[i] = RGBvalue[i];
    }
  }

if (LEDstatus == "on")
  {
    digitalWrite(D7, HIGH);
  }
  if (LEDstatus == "off")
  {
    digitalWrite(D7, LOW);
  }
  else
  {
    Serial.println("Fail to read LEDstatus");
  }

  RED = RGB[0];
  GREEN = RGB[2];
  BLUE = RGB[1];
  analogWrite(D1, RED);
  analogWrite(D2, GREEN);
  analogWrite(D3, BLUE);
  
  Serial.print("please wait 5 seconds... ");
  Serial.println("");
  delay(20000); // Short delay to debounce input reading
}
