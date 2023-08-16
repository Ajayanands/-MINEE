#include <SoftwareSerial.h>

SoftwareSerial mySerial(9, 10);

void setup()
{
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  delay(100);
}


void loop()
{
  if (Serial.available() > 0) {
    switch (Serial.read()) {
      case 'c':
        mySerial.println("ATD+916369715294;"); // Replace x with the phone number you want to call
        delay(1000);
        break;
    }
  }

  if (mySerial.available() > 0) {
    Serial.write(mySerial.read());
  }
}