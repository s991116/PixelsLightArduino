#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <ezBuzzer.h>
#include "WifiSettings.h"
#include "LedScreen.h"

int ledPin = BUILTIN_LED;//D4; // GPIO13
bool SwitchState = 0;

#define BUZZER_PIN (D2)
ezBuzzer buzzer(BUZZER_PIN);
//Mario main theme melody
int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};
//Mario main them noteDurations
int noteDurations[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};


ESP8266WebServer server(80);

LedScreen* screen;

void setup() {
  pinMode(ledPin, OUTPUT);
  screen = new LedScreen();
  Serial.begin(115200);
  delay(10);

  // Connect to WiFi network
  Serial.println();
  Serial.println("PixelsLightArduino");
  Serial.println("Connecting to \"" + String(ssid) + "\"");

  WiFi.mode(WIFI_STA);
  String hostname = "PixelsLight1";
  WiFi.hostname(hostname.c_str());
  WiFi.begin(ssid, password);

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(String(++i) + " ");
  }
  Serial.println();
  Serial.println("Connected successfully");

  // Print the IP address
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/api/switch1On", []() {
    Serial.println("Switch 1 On");
    setLightState(true);
    
    server.send(200);
  });

  server.on("/api/switch1Off", []() {
    Serial.println("Switch 1 Off");
    setLightState(false);
    server.send(200);
  });

  server.on("/api/switch1Status", []() {
    Serial.print("Switch 1 Status: ");
    Serial.println(SwitchState);
    
    if(SwitchState)
      server.send(200, "application/json", "1");
    else
      server.send(200, "application/json", "0");

  });
    
  server.on("/api/alarmOn", []() {
    Serial.println("Alarm on");
    server.send(200);
  });

  // Start the server
  server.begin();
}

void setLightState(bool isOn) {
    digitalWrite(ledPin, !isOn);
    SwitchState = isOn;
    updateScreen(SwitchState);
    PlayMelody();
}

void updateScreen(bool state) {
  if(state)
    screen->drawPicture(1);
  else
    screen->clearScreen();
}

void PlayMelody() {
  if (buzzer.getState() == BUZZER_IDLE) {
      int length = sizeof(noteDurations) / sizeof(int);
      buzzer.playMelody(melody, noteDurations, length); // playing
    }
}
void loop() {
  server.handleClient();
  buzzer.loop();
}
