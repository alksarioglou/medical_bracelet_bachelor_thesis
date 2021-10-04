/*!

  This program collects the Heart-rate and ECG data and communicates them to the Ubidots IoT Cloud Service

  CREDITS TO:
  @author linfeng(Musk.lin@dfrobot.com)
  Copyright (C) <2015>  <linfeng>
  The code of the above person was taken and modified to fit the needs of the Final-Year project of Alkinoos Sarioglou

  CREDITS TO:
  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

  Blynk library is licensed under MIT license
  This example code is in public domain.

  /*************************************************************
  Download latest Blynk library here:
  https://github.com/blynkkk/blynk-library/releases/latest

 *************************************************************
  
*/

/******************************
   Wi-Fi ESP32
 ******************************/
#include <WiFi.h>
#include <PubSubClient.h>
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define WIFISSID "Vodafone" // Put your WifiSSID here
#define PASSWORD "1894cTiD9320" // Put your wifi password here
#define TOKEN "BBFF-Mw49RtL5lzGGpMTZsBkmacAUldbTnS" // Put your Ubidots' TOKEN
#define MQTT_CLIENT_NAME "bracelet" // MQTT client Name, please enter your own 8-12 alphanumeric character ASCII string; 
//it should be a random and unique ascii string and different from all other devices


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "05yGAThpz710HDISJQjMB049iTpB16kb";
float ecg_data;
int heart_rate_data;

/****************************************
   Define Constants
 ****************************************/
#define VARIABLE_LABEL "sensor" // Assing the variable label
#define DEVICE_LABEL "esp32" // Assig the device label

// https://industrial.api.ubidots.com/api/v1.6/devices/{your-device-label}/?type=heartly
char mqttBroker[]  = "industrial.api.ubidots.com";
char payload[100];
char topic[150];
// Space to store values to send
char str_sensor[10];

#define VARIABLE_LABEL_ECG "ecg-sensor" // Assing the variable label

#define SENSOR 35 // Set the A3/IO35 as SENSOR

BlynkTimer timer;

bool turn = true;
uint8_t rateValue;

/****************************************
   Auxiliar Functions
 ****************************************/
WiFiClient ubidots;
PubSubClient client(ubidots);

/***************************************
   Heart - Rate sensor
 ***************************************/
#define heartratePin 36
#include "DFRobot_Heartrate.h"

DFRobot_Heartrate heartrate(ANALOG_MODE); ///< ANALOG_MODE or DIGITAL_MODE

/***************************************
   Functions
 ***************************************/

void callback(char* topic, byte* payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  Serial.write(payload, length);
  Serial.println(topic);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");

    // Attemp to connect
    if (client.connect(MQTT_CLIENT_NAME, TOKEN, "")) {
      Serial.println("Connected");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}


/* ECG Data  */
// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  ecg_data =  analogRead(SENSOR); // Read ECG Data
  if (rateValue)  heart_rate_data = rateValue;    // Read Heart Rate Data
  Blynk.virtualWrite(V5, ecg_data); // Send ECG Data
  Blynk.virtualWrite(V3, heart_rate_data); // Send Heart Rate Data
}

/****************************************
   Main Functions
 ****************************************/
void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFISSID, PASSWORD);
  Blynk.begin(auth, WIFISSID, PASSWORD);
  // Setup a function to be called every second for sending the values to the Mobile App
  timer.setInterval(300L, myTimerEvent);
  // Assign the pin as INPUT
  pinMode(heartratePin, INPUT);
  pinMode(SENSOR, INPUT);

  Serial.println();
  Serial.print("Waiting for WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqttBroker, 1883);
  client.setCallback(callback);
}
 


void loop() {
  if (!client.connected()) {
    reconnect();
  }

  Blynk.run();
  timer.run(); // Initiates BlynkTimer, running timer every second

  if (turn == true) {
    sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
    sprintf(payload, "%s", ""); // Cleans the payload
    sprintf(payload, "{\"%s\":", VARIABLE_LABEL); // Adds the variable label

    heartrate.getValue(heartratePin); ///< A1 foot sampled values
    rateValue = heartrate.getRate(heartratePin); ///< Get heart rate value
    if (rateValue)  {
      Serial.println(rateValue);
      dtostrf((float)rateValue, 4, 0, str_sensor);
      sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor); // Adds the value
      Serial.println("Publishing BPM data to Ubidots Cloud");
      turn = false;
      client.publish(topic, payload);
      client.loop();
    }
    delay(20);
  }

  if (turn == false) {
    sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
    sprintf(payload, "%s", ""); // Cleans the payload
    sprintf(payload, "{\"%s\":", VARIABLE_LABEL_ECG); // Adds the variable label

    float sensor = analogRead(SENSOR);

    /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
    dtostrf(sensor, 4, 2, str_sensor);

    sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor); // Adds the value
    Serial.println("Publishing ECG data to Ubidots Cloud");
    turn = true;
    client.publish(topic, payload);
    client.loop();
    delay(10);
  }

}

/******************************************************************************
  Copyright (C) <2015>  <linfeng>
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  Contact: Musk.lin@dfrobot.com
 ******************************************************************************/
