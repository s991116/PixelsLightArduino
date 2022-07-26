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

    void drawMario();
    void drawCreeper();
    void drawGhost();
    void drawGoombas();
    void drawPacMan();
    void drawPigFace();

    byte pictureIndex;

  private:
    const int lightPin = D3;
    const int ledUpdateTime = 500;
    const float intensity = 0.2;
    Adafruit_NeoPixel* neoPixels;
    static const byte Mario[4][16][16][3];
    static const byte MarioSerie[4];
    static const byte Creeper[1][16][16][3];
    static const byte CreeperSerie[1];
    static const byte Ghost[2][16][16][3];
    static const byte GhostSerie[2];
    static const byte Goombas[2][16][16][3];
    static const byte GoombasSerie[2];
    static const byte PacMan[3][16][16][3];
    static const byte PacManSerie[4];
    static const byte PigFace[1][16][16][3];
    static const byte PigFaceSerie[1];
    static const byte numbersPixelSize7[10][7];

    void drawPictureArray(byte PictureIndex, const byte pictureArray[][16][16][3]);
};


#endif