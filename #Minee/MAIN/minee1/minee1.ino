#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most accurate BPM math.
#include <Wire.h>                     // Include Wire library for I2C communication
#include <LiquidCrystal_I2C.h>        // Include LiquidCrystal_I2C library for LCD display
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.
#include <SoftwareSerial.h>
//  Variables
const int PulseWire = 0;             // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13;                // The on-board Arduino LED, close to PIN 13.
int Threshold = 450;                 // Determine which Signal to "count as a beat" and which to ignore.
                                     // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                                     // Otherwise leave the default "550" value.

PulseSensorPlayground pulseSensor;    // Creates an instance of the PulseSensorPlayground object called "pulseSensor"
SoftwareSerial mySerial(9, 10);
// LCD Display configuration
LiquidCrystal_I2C lcd(0x27, 16, 2);   // Set the LCD address to 0x27 for a 16 chars and 2 lines display

// Sound sensor configuration
const int ledpin=13; // ledpin and soundpin are not changed throughout the process
const int soundpin=A2;
const int threshold=500; // sets threshold value for sound sensor


int Ana_out = 0;

void setup() {
  mySerial.begin(9600);   // Setting the baud rate of GSM Module     // Setting the baud rate of Serial Monitor (Arduino)
  delay(100);
  Serial.begin(9600);                // For Serial Monitor

  lcd.init();                         // Initialize the LCD
  lcd.backlight();                    // Turn on the backlight

  // Configure the PulseSensor object, by assigning our variables to it.
  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED13);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);

  // Double-check the "pulseSensor" object was created and "began" seeing a signal.
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.
  }

  pinMode(ledpin,OUTPUT);
  pinMode(soundpin,INPUT);
}

void loop() {
  Ana_out = analogRead(A1);
  if (Ana_out < 100) {
    Serial.println("Vibration is aggressive");
  }

  // Check for pulse beat
  if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened".
    int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
    // "myBPM" hold this BPM value now. // If test is "true", print a message "a heartbeat happened".
    Serial.println("BPM:");                        // Print phrase "BPM: "// Plot the BPM value on the Serial Plotter
    Serial.println(myBPM);
    Serial.write(13); // CR character to return the cursor to the start of the line
    int soundSens = analogRead(soundpin);
    // Check if BPM is above 140
    if (myBPM >= 160 && soundSens >= threshold) {
      lcd.setCursor(0, 0); // Set the cursor to the first column and first row
      lcd.print("NEED HELP!"); // Print the message on the LCD
      lcd.setCursor(0, 1); // Set the cursor to the first column and second row
      lcd.print("Sound Detected!");
      delay(2000); // Wait for 2 seconds before clearing the LCD
      lcd.clear(); // Clear the LCD screen
      mySerial.println("ATD+916369715294;"); // Replace x with the phone number you want to call
        delay(1000);
    }
    if (mySerial.available() > 0) {
    Serial.write(mySerial.read());
     }else {
      // Check if sound is detected
      if (soundSens >= threshold) {
        lcd.setCursor(0, 1); // Set the cursor to the first column and second row
        lcd.print("Sound Detected!"); // Print the message on the LCD
        delay(100); // Wait for 1 second before clearing the LCD
        lcd.clear(); // Clear the LCD screen
      }
    }
  }
  delay(20); // Delay a little bit to improve simulation performance
}
