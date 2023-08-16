#include <SoftwareSerial.h>

SoftwareSerial SIM900A(7, 8); // RX, TX

void setup() {
  // initialize serial communication with the GSM module
  Serial.begin(9600);
  SIM900A.begin(9600);

  // wait for the module to start up
  delay(10000);
}

void loop() {
  // Send AT command to check network registration status
  SIM900A.println("AT+CREG?");
  delay(1000);

  if (SIM900A.available()) {
    String response = SIM900A.readString();
    if (response.indexOf("+CREG: 0,1") != -1 || response.indexOf("+CREG: 0,5") != -1) {
      Serial.println("Network is connected");
    } else {
      Serial.println("Network is not connected");
    }
  } else {
    Serial.println("No response from GSM module");
  }

  // wait for 5 seconds before checking again
  delay(5000);
}
