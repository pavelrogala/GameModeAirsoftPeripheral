#include <Arduino.h>
#include <LoRa.h>
#include <SPI.h>

#define ss   5   // Chip select pin for LoRa
#define rst  14  // Reset pin for LoRa
#define dio0 2   // Interrupt pin for LoRa

const int teamAButtonPin = 15;
const int teamBButtonPin = 16;
const int teamNButtonPin = 18;

const int teamALedPin = 25;
const int teamBLedPin = 27;

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

  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender");

  // Setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);

  // Try to initialize LoRa
  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);  // Wait and keep trying to initialize LoRa
  }

  // Set sync word to ensure you're not receiving messages from other LoRa modules
  LoRa.setSyncWord(0xF3);

  Serial.println("LoRa Initializing OK!");
}

void loop() {
  if (digitalRead(teamAButtonPin) == LOW) { // Team A button pressed
    if (currentTeam != "Team:A") {
      currentTeam = "Team:A";
      Serial.println("Team:A");
      LoRa.beginPacket();
      LoRa.print("Zone:A,Team:A");
      LoRa.endPacket();
    }
    digitalWrite(teamALedPin, HIGH);
    digitalWrite(teamBLedPin, LOW);
  } 
  else if (digitalRead(teamBButtonPin) == LOW) { // Team B button pressed
    if (currentTeam != "Team:B") {
      currentTeam = "Team:B";
      Serial.println("Team:B");
      LoRa.beginPacket();
      LoRa.print("Zone:A,Team:B");
      LoRa.endPacket();
    }
    digitalWrite(teamALedPin, LOW);
    digitalWrite(teamBLedPin, HIGH);
  } 
  else if (digitalRead(teamNButtonPin) == LOW) { // Neutral button pressed
    if (currentTeam != "") {
      currentTeam = "";
      Serial.println("Neutral");
      LoRa.beginPacket();
      LoRa.print("Zone:A,Team:N");
      LoRa.endPacket();
    }
    digitalWrite(teamALedPin, LOW);
    digitalWrite(teamBLedPin, LOW);
  }

  // Handle periodic Serial printing
  if (currentTeam != "" && millis() - lastPrintTime >= printInterval) {
    Serial.println(currentTeam);
    String message = "Zone:A,";
    message.concat(currentTeam);
    LoRa.beginPacket();
    LoRa.print(message);
    LoRa.endPacket();
    lastPrintTime = millis();
  }
}