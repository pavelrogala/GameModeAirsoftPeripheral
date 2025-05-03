#include <Arduino.h>

const int teamAButtonPin = 3;
const int teamBButtonPin = 2;
const int teamNButtonPin = 4;

const int teamALedPin = 6;
const int teamBLedPin = 5;

unsigned long lastPrintTime = 0;
const unsigned long printInterval = 5000; // 30 seconds

String currentTeam = "";

void setup() {
  pinMode(teamAButtonPin, INPUT_PULLUP);
  pinMode(teamBButtonPin, INPUT_PULLUP);
  pinMode(teamNButtonPin, INPUT_PULLUP);

  pinMode(teamALedPin, OUTPUT);
  pinMode(teamBLedPin, OUTPUT);

  // Ensure all LEDs are off at the start
  digitalWrite(teamALedPin, LOW);
  digitalWrite(teamBLedPin, LOW);

  Serial.begin(9600);
}

void loop() {
  if (digitalRead(teamAButtonPin) == LOW) { // Team A button pressed
    if (currentTeam != "Team:A") {
      currentTeam = "Team:A";
      Serial.println("Team:A");
    }
    digitalWrite(teamALedPin, HIGH);
    digitalWrite(teamBLedPin, LOW);
  } 
  else if (digitalRead(teamBButtonPin) == LOW) { // Team B button pressed
    if (currentTeam != "Team:B") {
      currentTeam = "Team:B";
      Serial.println("Team:B");
    }
    digitalWrite(teamALedPin, LOW);
    digitalWrite(teamBLedPin, HIGH);
  } 
  else if (digitalRead(teamNButtonPin) == LOW) { // Neutral button pressed
    if (currentTeam != "") {
      currentTeam = "";
    }
    digitalWrite(teamALedPin, LOW);
    digitalWrite(teamBLedPin, LOW);
  }

  // Handle periodic Serial printing
  if (currentTeam != "" && millis() - lastPrintTime >= printInterval) {
    Serial.println(currentTeam);
    lastPrintTime = millis();
  }
}