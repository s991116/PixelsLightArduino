#ifndef __LEDSCREEN_H__
#define __LEDSCREEN_H__

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

//Settings----------------
#define SCREENPIXELS (256)
//------------------------


class LedScreen
{
  public:
    LedScreen();
    void clearScreen();
    void drawPicture(short picNr);
    void drawPixel(byte x, byte y, byte colorR, byte colorG, byte colorB);
    void draw7PixelDigit(byte digit, byte offsetX, byte offsetY, byte colorR, byte colorG, byte colorB);

  private:
    const int lightPin = D3;
    const int ledUpdateTime = 500;
    const byte intensity = 1;
    Adafruit_NeoPixel* neoPixels;

    static const byte pic[4][16][16][3];
    static const byte numbersPixelSize7[10][7];

};


#endif