#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <ezBuzzer.h>
#include "WifiSettings.h"
#include "LedScreen.h"

int ledPin = BUILTIN_LED;
bool PictureState = 0;
bool ClockState = 0;
unsigned long nextPrint = 0;
unsigned long nextPrintTheme = 0;
unsigned long nextClockPictureState = 0;

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

int PictureTheme;

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

//  webclock = new WebClock();
  
  server.on("/api/pictureOn", []() {
    Serial.println("Picture On");
    PictureState = true;
    setLightState(PictureState);
    screen->pictureIndex = 0;
  
    server.send(200);
  });

  server.on("/api/pictureOff", []() {
    Serial.println("Picture 1 Off");
    PictureState = false;
    nextPrint = 0;
    nextPrintTheme = 0;
    nextClockPictureState = 0;
    screen->clearScreen();
    server.send(200);
  });

  server.on("/api/pictureStatus", []() {
    Serial.print("Picture 1 Status: ");
    Serial.println(PictureState);
    
    if(PictureState)
      server.send(200, "application/json", "1");
    else
      server.send(200, "application/json", "0");

  });

  server.on("/api/clockOn", []() {
    Serial.println("Clock On");
    ClockState = true;
    server.send(200);
  });

  server.on("/api/clockOff", []() {
    Serial.println("Clock Off");
    ClockState = false;
    nextPrint = 0;
    nextPrintTheme = 0;
    nextClockPictureState = 0;
    screen->clearScreen();
    server.send(200);
  });

  server.on("/api/clockStatus", []() {
    Serial.print("Clock Status: ");
    Serial.println(ClockState);
    
    if(ClockState)
      server.send(200, "application/json", "1");
    else
      server.send(200, "application/json", "0");

  });

  server.on("/api/alarmOn", []() {
    Serial.println("Alarm on");
    PlayMelody();
    server.send(200);
  });

  server.begin();

  WiFiClockSetup();
}

void setLightState(bool isOn) {
    digitalWrite(ledPin, isOn);
}

void PlayMelody() {
  if (buzzer.getState() == BUZZER_IDLE) {
      int length = sizeof(noteDurations) / sizeof(int);
      buzzer.playMelody(melody, noteDurations, length); // playing
    }
}

bool ClockPictureState;
void loop() {
  server.handleClient();
  buzzer.loop();

  if(ClockState && PictureState) {
    if(millis() > nextClockPictureState) {
      ClockPictureState = !ClockPictureState;
      nextClockPictureState += 5000;
      screen->clearScreen();      
    }
  } else if(ClockState) {
    ClockPictureState = true;
  } else if(PictureState) {
    ClockPictureState = false;
  }


  if(ClockState && ClockPictureState && (millis() > nextPrint)) {
    int hours = GetHours();
    int minutes = GetMinutes();

    int hoursSmallDigit = hours % 10;
    int minutesSmallDigit = minutes % 10;

    int hoursBigDigit = hours / 10;
    int minutesBigDigit = minutes / 10;
        
    screen->draw7PixelDigit(hoursBigDigit, 0, 1, 20, 0, 30);
    screen->draw7PixelDigit(hoursSmallDigit, 7, 1, 20, 0, 30);
    screen->draw7PixelDigit(minutesBigDigit, 1, 9, 20, 0, 30);
    screen->draw7PixelDigit(minutesSmallDigit, 9, 9, 20, 0, 30);
    nextPrint = millis()+300;
  }

  if(millis() > nextPrintTheme) {
    PictureTheme++;
    nextPrint = 0;
    if(PictureTheme > 5)
      PictureTheme = 0;    
    nextPrintTheme = millis() + 5000;
  }

  if(PictureState && !ClockPictureState && (millis() > nextPrint)) {
    switch(PictureTheme) {
      case 0:
        screen->drawMario();
        nextPrint = millis()+200;
        break;

      case 1:
        screen->drawCreeper();
        nextPrint = millis()+500;
        break;

      case 2:
        screen->drawGhost();
        nextPrint = millis()+200;
        break;
  
      case 3:
        screen->drawGoombas();
        nextPrint = millis()+200;
        break;

      case 4:
        screen->drawPacMan();
        nextPrint = millis()+200;
        break;

      case 5:
        screen->drawPigFace();
        nextPrint = millis()+500;
        break;
    }
  }
}