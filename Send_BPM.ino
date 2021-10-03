/****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"
#include "DFRobot_Heartrate.h"

/****************************************
 * Define Instances and Constants
 ****************************************/

const char* UBIDOTS_TOKEN = "BBFF-NfWOcO3Li4tQPZq3NaRX1EHJI2WuSz";  // Put here your Ubidots TOKEN
const char* WIFI_SSID = "SKYD0714"; // Put here your Wi-Fi SSID
const char* WIFI_PASS = "DNPCQDFMDD"; // Put here your Wi-Fi password

Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);

DFRobot_Heartrate heartrate(ANALOG_MODE); ///< ANALOG_MODE or DIGITAL_MODE
//#define heartratePin A2


/****************************************
 * Auxiliar Functions
 ****************************************/

// Put here your auxiliar functions

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  Serial.begin(115200);
  ubidots.wifiConnect(WIFI_SSID, WIFI_PASS);

  ubidots.setDebug(true);  // Uncomment this line for printing debug messages

  pinMode(2,INPUT);

}

void loop() {
//  uint8_t rateValue;
//  heartrate.getValue(A0); ///< A2 foot sampled values
//  rateValue = heartrate.getRate(); ///< Get heart rate value
//  if(rateValue)  {
//    Serial.println(rateValue);
//  }
//  delay(20);
  float value1 = analogRead(2);
  float value2 = random(0, 9) * 100;
  float value3 = random(0, 9) * 1000;
  ubidots.add("Heart Rate", value1);  // Change for your variable name
  ubidots.add("Variable_Name_Two", value2);
  ubidots.add("Variable_Name_Three", value3);

  bool bufferSent = false;
  bufferSent = ubidots.send();  // Will send data to a device label that matches the device Id

  if (bufferSent) {
    // Do something if values were sent properly
    Serial.println("Values sent by the device");
    //Serial.println(rateValue);
  }

  delay(5000);
}
