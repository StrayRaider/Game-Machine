#include <Wire.h>
#include<SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4

Adafruit_SSD1306 display(OLED_RESET);

static const bool Rocket[8][8] = {
{0,1,0,0,0,0,1,0},
{0,1,1,1,1,1,1,0},
{0,0,0,1,1,0,0,0},
{0,0,0,1,1,0,0,0},
{1,1,1,1,1,1,1,1},
{1,0,1,1,1,1,0,1},
{0,0,0,1,1,0,0,0},
{0,0,0,1,1,0,0,0}
};
static const bool Rock[8][8] = {
{0,0,0,0,0,0,0,0},
{0,0,0,1,1,0,0,0},
{0,0,1,1,1,1,0,0},
{0,1,1,1,1,1,1,0},
{0,1,0,0,1,1,1,0},
{0,0,1,0,1,1,0,0},
{0,0,0,1,1,0,0,0},
{0,0,0,0,0,0,0,0}
};

static int rocket_loc[2] = {0,4};

void setup()
{
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  display.display();
  display.clearDisplay();
}
//gets x and y as matris location
void draw(bool Obj[8][8],int x, int y){
  int x_px = x*8;
  int y_px = y*8;
  for(int i=0 ; i<8 ; i++){
    for(int j=0 ; j<8 ; j++){
      if(Obj[i][j] == 1){
        display.drawPixel(x_px+i, y_px+j, SSD1306_WHITE);
      }
    }
  }  
}
void del(bool Obj[8][8],int x, int y){
  int x_px = x*8;
  int y_px = y*8;
  for(int i=0 ; i<8 ; i++){
    for(int j=0 ; j<8 ; j++){
      if(Obj[i][j] == 1){
        display.drawPixel(x_px+i, y_px+j, SSD1306_BLACK);
      }
    }
  }  
}

int get_loc(int sensorValue){
  return sensorValue/128;
  //0 to 8 matris data
}

void loop()
{
  int sensorValue = 0;
  sensorValue = analogRead(A1);
  // print out the value you read:
  Serial.println(sensorValue);
  rocket_loc[1] = get_loc(sensorValue);
  Serial.println(rocket_loc[1]);

  //update
  display.display();
  draw(Rocket,rocket_loc[0],rocket_loc[1]);
  draw(Rock,1,1);
  delay(1000);
}