#include <Wire.h>
#include<SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool hardness = 0;
int time = 50;
int loop_c = 0;
int cnt = 0;
const int obj_size = 20;

static int Objects[20][3];
int b_c_a = 0;

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

static const bool Bullet[8][8] = {
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,1,0,0,1,0,0},
{0,1,1,0,0,1,1,0},
{0,1,1,0,0,1,1,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0}
};

static const bool Rock[8][8] = {
{0,0,0,0,0,0,0,1},
{0,1,0,1,1,0,0,1},
{0,0,1,1,1,1,0,0},
{0,1,1,1,1,1,1,0},
{0,1,0,0,1,1,1,0},
{0,0,1,0,1,1,0,0},
{1,0,0,1,1,0,0,0},
{0,1,0,0,0,0,1,0}
};

static const bool HRock[8][8] = {
{0,0,0,0,0,1,0,1},
{0,1,0,0,0,0,0,1},
{0,0,1,1,0,0,0,0},
{0,1,1,1,1,0,0,1},
{0,1,0,0,1,0,1,0},
{0,0,1,0,1,1,0,0},
{1,0,0,1,1,0,0,0},
{0,1,0,0,0,0,1,0}
};

static const bool Gun[8][8] = {
{0,0,0,0,1,0,0,0},
{0,0,0,0,1,0,0,0},
{0,0,0,1,1,0,0,0},
{0,0,0,1,1,0,0,0},
{0,0,0,1,1,0,0,0},
{0,0,1,1,1,1,0,0},
{0,1,1,1,1,1,1,0},
{0,0,0,1,1,0,0,0}
};

static const bool Prize[8][8] = {
{0,0,0,1,1,0,0,0},
{0,0,1,0,1,1,0,0},
{0,1,0,0,1,1,1,0},
{0,1,0,1,1,1,1,0},
{0,1,0,1,1,1,1,0},
{0,1,0,0,1,1,1,0},
{0,0,1,0,1,1,0,0},
{0,0,0,1,1,0,0,0}
};

static const bool NTouch[8][8] = {
{0,0,0,0,0,0,0,0},
{0,0,0,1,1,0,0,0},
{0,0,1,1,1,1,0,0},
{0,0,1,0,1,1,0,0},
{0,1,0,1,1,1,1,0},
{0,1,0,1,1,1,1,0},
{0,1,1,0,1,1,1,0},
{0,0,0,1,1,0,0,0}
};

static int rocket_loc[2] = {0,4};
int life = 3;
int gun = 3;
bool gun_exec = 0;
int bullet[2] = {-1,-1};

void setup()
{
  pinMode(A0, INPUT);
  pinMode(13, INPUT);
  pinMode(12, INPUT);
  pinMode(11, INPUT);
  pinMode(10, INPUT);
  
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);

  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(28, OUTPUT);
  pinMode(29, OUTPUT);

  pinMode(30, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);

  pinMode(34, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(36, OUTPUT);

  
  pinMode(41, OUTPUT);

  digitalWrite(23,1);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  display.clearDisplay();

  int act_loc = 0;
  int up = 0;
  int down = 0;
  int select = 0;
  int i = 5;
  while(1){
    display.drawRoundRect(2, 2, 120,20,8, SSD1306_WHITE);
    display.drawRoundRect(2, 32, 120,20,8, SSD1306_WHITE);
    display.display();
    up = digitalRead(12);
    down = digitalRead(13);
    select = digitalRead(11);
    if(up)
      act_loc += 1;
    if(down)
      act_loc -= 1;
      if(act_loc == -1)
        act_loc = 1;
    act_loc = act_loc % 2;
    if(select){
      if(act_loc){
        hardness = ! hardness;
      }else{
        break;
      }
    }
    if (act_loc){
      display.fillRoundRect(4, 4, 116,14,8, SSD1306_WHITE);
      display.fillRoundRect(4, 34, 116,14,8, SSD1306_BLACK);
      display.setTextSize(1);             // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_BLACK);        // Draw white text
      display.setCursor(40,8);           // Start at top-left corner
      if(hardness){
         display.println(F("EASY"));
      }else{
        display.println(F("HARD"));
      }
      display.setTextColor(SSD1306_WHITE);        // Draw white text
      display.setCursor(40,38);             // Start at top-left corner
      display.println(F(" Start "));
    }else{
      display.fillRoundRect(4, 34, 116,14,8, SSD1306_WHITE);
      display.fillRoundRect(4, 4, 116,14,8, SSD1306_BLACK);
      display.setTextSize(1);             // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_BLACK);        // Draw white text
      display.setCursor(40,38);             // Start at top-left corner
      display.println(F(" Start "));
      display.setTextColor(SSD1306_WHITE);        // Draw white text
      display.setCursor(40,8);             // Start at top-left corner
      if(hardness){
         display.println(F("EASY"));
      }else{
        display.println(F("HARD"));
      }
    }
    delay(50);
  }
  display.display();
  display.clearDisplay();
  set_map();
}
//gets x and y as matris location
void draw(bool Obj[8][8],int x, int y){
  for(int i=0 ; i<8 ; i++){
    for(int j=0 ; j<8 ; j++){
      if(Obj[i][j] == 1)
        display.drawPixel(x*8+i, y*8+j, SSD1306_WHITE);
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

void gen_met(){
  if(b_c_a<obj_size){
    Objects[b_c_a][0] = 15;
    Objects[b_c_a][1] = random(1,9);
    Objects[b_c_a][2] = random(0,4);
    b_c_a +=1;
  }
  //engel = 0
  //ödül = 1
  //silah = 2
  //dokunulmazlık = 3
  //yarım engel = 4
}

void set_map(){
  for(int x = 0;x<obj_size;x++){
    Objects[x][0] = -1;
    Objects[x][1] = -1;
    Objects[x][2] = -1;
  }
}

void update_bar(int x){
  for(;x<obj_size-1;x++){
    if(Objects[x+1][0] != -1){
      Objects[x][0] = Objects[x+1][0];
      Objects[x][1] = Objects[x+1][1];
      Objects[x][2] = Objects[x+1][2];
    }
  }
  b_c_a -= 1;
}

void loop()
{
  //rocket events
  int sensorValue = analogRead(A0);
  //move rocket
  del(Rocket,0,rocket_loc[1]);
  rocket_loc[1] = get_loc(sensorValue);

  //gun exec button
  digitalWrite(41,LOW);
  if(gun){
    if (digitalRead(10) != gun_exec){
      gun_exec = digitalRead(10);
      if(gun_exec){
        gun -=1;
        bullet[0]=rocket_loc[0];
        bullet[1]=rocket_loc[1];
      }
    }
  }
  if(bullet[0]!=-1){
    bullet[0] +=1;
    draw(Bullet,bullet[0],bullet[1]);
    for(int x = 0;x<obj_size;x++){
      if(Objects[x][2] == 0){
        if(Objects[x][1] == bullet[1] && Objects[x][0] == bullet[0]){
          Objects[x][2] = 4;
          bullet[0]= -1;
          bullet[1]= -1;
          break;
         }
        }else if(Objects[x][2] == 4){
          if(Objects[x][1] == bullet[1] && Objects[x][0] == bullet[0]){
            update_bar(x);
            digitalWrite(41,HIGH);
            bullet[0]= -1;
            bullet[1]= -1;
            break;
          }
      }
    }
  }

  draw(Rocket,0,rocket_loc[1]);
  display.display();

  int tmr = 0;
  if(tmr%20==0){
    display.display();
    display.clearDisplay();
    //move objects
    for(int x = 0;x<obj_size;x++){
      if(Objects[x][0] != -1){
        Objects[x][0] = Objects[x][0]-1;
        if(Objects[x][0] == -1){
          update_bar(x);
        }
      }
    }

    draw(Rocket,0,rocket_loc[1]);

    //control coll
    for(int x = 0;x<obj_size;x++){
      int ob = Objects[x][2];
      if(Objects[x][0] != -1){
        if(Objects[x][0] == rocket_loc[0] && Objects[x][1] == rocket_loc[1]){
          if(ob == 0){
            digitalWrite(41,HIGH);
            delay(5000);
            }
            else if(ob == 1){
              
            }
            else if(ob == 2){
              
            }
            else if(ob == 3){
              
            }
          }
      }
    }
    for(int x = 0;x<obj_size;x++){
      int ob = Objects[x][2];
      if(Objects[x][0] != -1){
        if(ob == 0){
          draw(Rock,Objects[x][0],Objects[x][1]);
        }
        else if(ob == 1){
          draw(Prize,Objects[x][0],Objects[x][1]);
        }
        else if(ob == 2){
          draw(Gun,Objects[x][0],Objects[x][1]);
        }
        else if(ob == 3){
          draw(NTouch,Objects[x][0],Objects[x][1]);
        }else if(ob == 4){
          draw(HRock,Objects[x][0],Objects[x][1]);
        }
      }
    }

    //end of loop update gen objects of new frame
    if(loop_c%2==0){
      gen_met();
    }if(loop_c%14 == 0){
      //1 can 1 silah ekle
      if (life <3)
        life += 1;
      if (gun <3)
        gun += 1;    
    }

    // arayüz------------------------------
    // 7 segment
    int data[12] = {};
    for (int i = 0; i < 12; i++) {
      data[i] = bitRead(cnt, i);
    }
    if(data[0] == 0 && data[1] == 1 && data[2] == 0 && data[3] == 1)
      cnt += 6;
      for (int i = 0; i < 12; i++) {
        data[i] = bitRead(cnt, i);
      }
    if(data[4] == 0 && data[5] == 1 && data[6] == 0 && data[7] == 1)
      cnt += 96;
      for (int i = 0; i < 12; i++) {
        data[i] = bitRead(cnt, i);
      }
    for (int i = 0; i < 12; i++) {
      if(data[i])    
        digitalWrite(22+i,HIGH);
      else  
        digitalWrite(22+i,LOW);
    }
    //  

    // Gun
    digitalWrite(34,LOW);
    digitalWrite(35,LOW);
    digitalWrite(36,LOW);
    if(gun == 1){
      digitalWrite(34,HIGH);
    }
    else if(gun == 2){
      digitalWrite(34,HIGH);
      digitalWrite(35,HIGH);
    }
    else if(gun == 3){
      digitalWrite(34,HIGH);
      digitalWrite(35,HIGH);
      digitalWrite(36,HIGH);
    }
    //

    //zaman ayarlanması
    if(hardness){
      if(loop_c == 10){
        time = time-(time/5);
      }
    }
    loop_c += 1;
    cnt += 1;
    display.display();
    delay(time);
  }
}