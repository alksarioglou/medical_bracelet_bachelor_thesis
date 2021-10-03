void setup() {
// initialize the serial communication:
Serial.begin(9600);

}

void loop() {

// send the value of analog input 1:
Serial.println(analogRead(A1));

//Wait for a bit to keep serial data from saturating
delay(1);
}
