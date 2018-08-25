#include "LedControl.h"
#include <Keypad.h>
#include <LiquidCrystal.h>

#define CLK     11
#define CS      10
#define DIN     12

#define SIZE    8
#define ADVANCE_DELAY 25

int snake[SIZE * SIZE][2];
int length;
int food[2], v[2];
bool is_game_over = false;
long current_time;
long prev_advance;
int blink_count;
const short messageSpeed = 20;

const int rs = 27, en = 26, d4 = 25, d5 = 24, d6 = 23, d7 = 22;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


LedControl lc = LedControl(DIN, CLK, CS, 1);

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = { 2, 3, 4, 5 };
byte colPins[COLS] = { 6, 7, 8, 9 };

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );



void init_game() {
  prev_advance = current_time = 0;
  blink_count = 3;
  int half = SIZE / 2;
  length = SIZE / 3;

  for (int i = 0; i < length; i++) {
    snake[i][0] = half - 1;
    snake[i][1] = half + i;
  }

  food[0] = half + 1;
  food[1] = half - 1;

  v[0] = 0;
  v[1] = -1;
}

void render() {
  for (int i = 0; i < length; i++) {
    lc.setLed(0, snake[i][0], snake[i][1], 1);
  }
  lc.setLed(0, food[0], food[1], 1);
}

void clearScreen() {
  for (int x = 0; x < SIZE; x++) {
    for (int y = 0; y < SIZE; y++) {
      lc.setLed(0, x, y, 0);
    }
  }
}

/**
   moves the snake forward
   returns true if the game is over
*/
bool advance() {
  int head[2] = {snake[0][0] + v[0], snake[0][1] + v[1]};

  if (head[0] < 0 || head[0] >= SIZE) {

    delay(1000);
    showGameOverMessage();
    return true;
  }

  if (head[1] < 0 || head[1] >= SIZE) {

    delay(1000);
    showGameOverMessage();
    return true;
  }

  for (int i = 0; i < length; i++) {
    if (snake[i][0] == head[0] && snake[i][1] == head[1]) {
      delay(1000);
      showGameOverMessage();
      return true;
    }
  }

  bool grow = (head[0] == food[0] && head[1] == food[1]);
  if (grow) {
    length++;
    randomSeed(current_time);
    food[0] = random(SIZE);
    food[1] = random(SIZE);
  }

  for (int i = length - 1; i >= 0; i--) {
    snake[i + 1][0] = snake[i][0];
    snake[i + 1][1] = snake[i][1];
  }
  snake[0][0] += v[0];
  snake[0][1] += v[1];
  return false;
}

/*wait a bit between updates of the display */
unsigned long delaytime = 50;

unsigned long facetime = 1000;




// happy face
byte hf[8]= {B00111100,B01000010,B10010101,B10100001,B10100001,B10010101,B01000010,B00111100};
// neutral face
byte nf[8]={B00111100, B01000010,B10010101,B10010001,B10010001,B10010101,B01000010,B00111100};
// sad face
byte sf[8]= {B00111100,B01000010,B10100101,B10010001,B10010001,B10100101,B01000010,B00111100};
// P face
byte pf[8]= {B00111100,B01000010,B10010101,B10010001,B10110001,B10110101,B01000010,B00111100};
// wow face
byte wf[8]= {B00111100,B01000010,B10000101,B10110001,B10110001,B10000101,B01000010,B00111100};

//eye
byte e1f[8]= {B00000000,B00111100,B01111110,B01100110,B01100110,B01111110,B00111100,B00000000}; //mid
byte e2f[8]= {B00000000,B00000000,B01000000,B01000000,B01000000,B01000000,B00000000,B00000000}; //close
byte e3f[8]= {B00000000,B00111100,B01111110,B01111110,B01100110,B01100110,B00111100,B00000000}; //right
byte e4f[8]= {B00000000,B00111100,B01111110,B01111110,B01110010,B01110010,B00111100,B00000000}; //top-right
byte e5f[8]= {B00000000,B00111100,B01111110,B01110010,B01110010,B01111110,B00111100,B00000000}; //top
byte e6f[8]= {B00000000,B00111100,B01110010,B01110010,B01111110,B01111110,B00111100,B00000000}; //top-left
byte e7f[8]= {B00000000,B00111100,B01100110,B01100110,B01111110,B01111110,B00111100,B00000000}; //left
byte e8f[8]= {B00000000,B00111100,B01001110,B01001110,B01111110,B01111110,B00111100,B00000000}; //bot-left
byte e9f[8]= {B00000000,B00111100,B01111110,B01001110,B01001110,B01111110,B00111100,B00000000}; //bot
byte e10f[8]= {B00000000,B00111100,B01111110,B01111110,B01001110,B01001110,B00111100,B00000000};  //bot-right

byte e11f[8]= {B00000000,B00000000,B01110000,B01110000,B01110000,B01110000,B00000000,B00000000}; //half-close

byte e13f[8]= {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000}; //emp
byte e14f[8]= {B00000000,B00011000,B00011000,B00011000,B00011000,B00011000,B00011000,B00000000}; //blink
byte e12f[8]= {B00000000,B00111100,B00111100,B00100100,B00100100,B00111100,B00111100,B00000000}; //dying






void setup() {
  lcd.begin(16, 2);
  
  lcd.print("ARDUINO GCONSOLE");
  delay(1000);
  
  lcd.clear();
  lcd.print("  AMMER ASHRAF");
  lcd.setCursor(0, 1);
  lcd.print("    01408012");
  delay(1000);
  
  lcd.clear();
  lcd.print("   SNAKE GAME   ");
  
  /*
    The MAX72XX is in power-saving mode on startup,
    we have to do a wakeup call
  */
  lc.shutdown(0, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0, 8);
  /* and clear the display */
  lc.clearDisplay(0);

  init_game();
  render();
}

void loop() {
  lcd.setCursor(0, 1);
  lcd.print("    SCORE: ");
  lcd.print(length-2);

  
  if (!is_game_over) {
    clearScreen();
    render();

    if (current_time - prev_advance > ADVANCE_DELAY) {
      is_game_over = advance();
      prev_advance = current_time;
    }
  } else {
    while (blink_count > 0) {
      clearScreen();
      delay(300);
      render();
      delay(300);
      blink_count--;

    }
  }
  readControls();
  current_time++;
}

void restart() {
  init_game();
  is_game_over = false;
}

void readControls() {
  char key = keypad.getKey();
  
  if (key == NO_KEY) return;

  switch(key) {
    case '2':
      v[0] = 0;
      v[1] = 1;
      break;
    case '4':
      v[0] = -1;
      v[1] = 0;
      break;
    case '8':
      v[0] = 0;
      v[1] = -1;
      break;
    case '6':
      v[0] = 1;
      v[1] = 0;
      break;
    case '*':
      restart();
      break;
      
     case 'A':
      
      // Display happy face
      lc.setRow(0,0,hf[0]);
      lc.setRow(0,1,hf[1]);
      lc.setRow(0,2,hf[2]);
      lc.setRow(0,3,hf[3]);
      lc.setRow(0,4,hf[4]);
      lc.setRow(0,5,hf[5]);
      lc.setRow(0,6,hf[6]);
      lc.setRow(0,7,hf[7]);
      delay(facetime);
      break;

      case 'B':
      // Display neutral face
      lc.setRow(0,0,nf[0]);
      lc.setRow(0,1,nf[1]);
      lc.setRow(0,2,nf[2]);
      lc.setRow(0,3,nf[3]);
      lc.setRow(0,4,nf[4]);
      lc.setRow(0,5,nf[5]);
      lc.setRow(0,6,nf[6]);
      lc.setRow(0,7,nf[7]);
      delay(facetime);
      break;
      
      case 'C':
      // Display sad face
      
      lc.setRow(0,0,sf[0]);
      lc.setRow(0,1,sf[1]);
      lc.setRow(0,2,sf[2]);
      lc.setRow(0,3,sf[3]);
      lc.setRow(0,4,sf[4]);
      lc.setRow(0,5,sf[5]);
      lc.setRow(0,6,sf[6]);
      lc.setRow(0,7,sf[7]);
      delay(facetime);
      break;

      case 'D':
       // Display P face
      lc.setRow(0,0,pf[0]);
      lc.setRow(0,1,pf[1]);
      lc.setRow(0,2,pf[2]);
      lc.setRow(0,3,pf[3]);
      lc.setRow(0,4,pf[4]);
      lc.setRow(0,5,pf[5]);
      lc.setRow(0,6,pf[6]);
      lc.setRow(0,7,pf[7]);
      delay(facetime);
      break;

      case '#':
      // Display wow face
      lc.setRow(0,0,wf[0]);
      lc.setRow(0,1,wf[1]);
      lc.setRow(0,2,wf[2]);
      lc.setRow(0,3,wf[3]);
      lc.setRow(0,4,wf[4]);
      lc.setRow(0,5,wf[5]);
      lc.setRow(0,6,wf[6]);
      lc.setRow(0,7,wf[7]);
      delay(facetime);
      break;

      case '0':
      // Display eye 
      lc.setRow(0,0,e1f[0]);
      lc.setRow(0,1,e1f[1]);
      lc.setRow(0,2,e1f[2]);
      lc.setRow(0,3,e1f[3]);
      lc.setRow(0,4,e1f[4]);
      lc.setRow(0,5,e1f[5]);
      lc.setRow(0,6,e1f[6]);
      lc.setRow(0,7,e1f[7]);
      delay(2*facetime);
      
      lc.setRow(0,0,e14f[0]);
      lc.setRow(0,1,e14f[1]);
      lc.setRow(0,2,e14f[2]);
      lc.setRow(0,3,e14f[3]);
      lc.setRow(0,4,e14f[4]);
      lc.setRow(0,5,e14f[5]);
      lc.setRow(0,6,e14f[6]);
      lc.setRow(0,7,e14f[7]);
      delay(facetime/2);
      lc.setRow(0,0,e1f[0]);
      lc.setRow(0,1,e1f[1]);
      lc.setRow(0,2,e1f[2]);
      lc.setRow(0,3,e1f[3]);
      lc.setRow(0,4,e1f[4]);
      lc.setRow(0,5,e1f[5]);
      lc.setRow(0,6,e1f[6]);
      lc.setRow(0,7,e1f[7]);
      delay(3*facetime);
      lc.setRow(0,0,e14f[0]);
      lc.setRow(0,1,e14f[1]);
      lc.setRow(0,2,e14f[2]);
      lc.setRow(0,3,e14f[3]);
      lc.setRow(0,4,e14f[4]);
      lc.setRow(0,5,e14f[5]);
      lc.setRow(0,6,e14f[6]);
      lc.setRow(0,7,e14f[7]);
      delay(facetime/2);
      lc.setRow(0,0,e1f[0]);
      lc.setRow(0,1,e1f[1]);
      lc.setRow(0,2,e1f[2]);
      lc.setRow(0,3,e1f[3]);
      lc.setRow(0,4,e1f[4]);
      lc.setRow(0,5,e1f[5]);
      lc.setRow(0,6,e1f[6]);
      lc.setRow(0,7,e1f[7]);
      delay(2*facetime);
      lc.setRow(0,0,e3f[0]);
      lc.setRow(0,1,e3f[1]);
      lc.setRow(0,2,e3f[2]);
      lc.setRow(0,3,e3f[3]);
      lc.setRow(0,4,e3f[4]);
      lc.setRow(0,5,e3f[5]);
      lc.setRow(0,6,e3f[6]);
      lc.setRow(0,7,e3f[7]);
      delay(facetime/3);
      lc.setRow(0,0,e4f[0]);
      lc.setRow(0,1,e4f[1]);
      lc.setRow(0,2,e4f[2]);
      lc.setRow(0,3,e4f[3]);
      lc.setRow(0,4,e4f[4]);
      lc.setRow(0,5,e4f[5]);
      lc.setRow(0,6,e4f[6]);
      lc.setRow(0,7,e4f[7]);
      delay(facetime/3);
      lc.setRow(0,0,e5f[0]);
      lc.setRow(0,1,e5f[1]);
      lc.setRow(0,2,e5f[2]);
      lc.setRow(0,3,e5f[3]);
      lc.setRow(0,4,e5f[4]);
      lc.setRow(0,5,e5f[5]);
      lc.setRow(0,6,e5f[6]);
      lc.setRow(0,7,e5f[7]);
      delay(facetime/3);
      lc.setRow(0,0,e6f[0]);
      lc.setRow(0,1,e6f[1]);
      lc.setRow(0,2,e6f[2]);
      lc.setRow(0,3,e6f[3]);
      lc.setRow(0,4,e6f[4]);
      lc.setRow(0,5,e6f[5]);
      lc.setRow(0,6,e6f[6]);
      lc.setRow(0,7,e6f[7]);
      delay(facetime/3);
      lc.setRow(0,0,e7f[0]);
      lc.setRow(0,1,e7f[1]);
      lc.setRow(0,2,e7f[2]);
      lc.setRow(0,3,e7f[3]);
      lc.setRow(0,4,e7f[4]);
      lc.setRow(0,5,e7f[5]);
      lc.setRow(0,6,e7f[6]);
      lc.setRow(0,7,e7f[7]);
      delay(facetime/3);
      lc.setRow(0,0,e8f[0]);
      lc.setRow(0,1,e8f[1]);
      lc.setRow(0,2,e8f[2]);
      lc.setRow(0,3,e8f[3]);
      lc.setRow(0,4,e8f[4]);
      lc.setRow(0,5,e8f[5]);
      lc.setRow(0,6,e8f[6]);
      lc.setRow(0,7,e8f[7]);
      delay(facetime/3);
      lc.setRow(0,0,e9f[0]);
      lc.setRow(0,1,e9f[1]);
      lc.setRow(0,2,e9f[2]);
      lc.setRow(0,3,e9f[3]);
      lc.setRow(0,4,e9f[4]);
      lc.setRow(0,5,e9f[5]);
      lc.setRow(0,6,e9f[6]);
      lc.setRow(0,7,e9f[7]);
      delay(facetime/3);
      lc.setRow(0,0,e10f[0]);
      lc.setRow(0,1,e10f[1]);
      lc.setRow(0,2,e10f[2]);
      lc.setRow(0,3,e10f[3]);
      lc.setRow(0,4,e10f[4]);
      lc.setRow(0,5,e10f[5]);
      lc.setRow(0,6,e10f[6]);
      lc.setRow(0,7,e10f[7]);
      delay(facetime/3);
      lc.setRow(0,0,e3f[0]);
      lc.setRow(0,1,e3f[1]);
      lc.setRow(0,2,e3f[2]);
      lc.setRow(0,3,e3f[3]);
      lc.setRow(0,4,e3f[4]);
      lc.setRow(0,5,e3f[5]);
      lc.setRow(0,6,e3f[6]);
      lc.setRow(0,7,e3f[7]);
      delay(facetime/3);
      lc.setRow(0,0,e4f[0]);
      lc.setRow(0,1,e4f[1]);
      lc.setRow(0,2,e4f[2]);
      lc.setRow(0,3,e4f[3]);
      lc.setRow(0,4,e4f[4]);
      lc.setRow(0,5,e4f[5]);
      lc.setRow(0,6,e4f[6]);
      lc.setRow(0,7,e4f[7]);
      delay(facetime/3);
      lc.setRow(0,0,e5f[0]);
      lc.setRow(0,1,e5f[1]);
      lc.setRow(0,2,e5f[2]);
      lc.setRow(0,3,e5f[3]);
      lc.setRow(0,4,e5f[4]);
      lc.setRow(0,5,e5f[5]);
      lc.setRow(0,6,e5f[6]);
      lc.setRow(0,7,e5f[7]);
      delay(facetime/3);
      lc.setRow(0,0,e6f[0]);
      lc.setRow(0,1,e6f[1]);
      lc.setRow(0,2,e6f[2]);
      lc.setRow(0,3,e6f[3]);
      lc.setRow(0,4,e6f[4]);
      lc.setRow(0,5,e6f[5]);
      lc.setRow(0,6,e6f[6]);
      lc.setRow(0,7,e6f[7]);
      delay(facetime/3);
      lc.setRow(0,0,e7f[0]);
      lc.setRow(0,1,e7f[1]);
      lc.setRow(0,2,e7f[2]);
      lc.setRow(0,3,e7f[3]);
      lc.setRow(0,4,e7f[4]);
      lc.setRow(0,5,e7f[5]);
      lc.setRow(0,6,e7f[6]);
      lc.setRow(0,7,e7f[7]);
      delay(facetime/3);
      lc.setRow(0,0,e8f[0]);
      lc.setRow(0,1,e8f[1]);
      lc.setRow(0,2,e8f[2]);
      lc.setRow(0,3,e8f[3]);
      lc.setRow(0,4,e8f[4]);
      lc.setRow(0,5,e8f[5]);
      lc.setRow(0,6,e8f[6]);
      lc.setRow(0,7,e8f[7]);
      delay(facetime/3);
      lc.setRow(0,0,e9f[0]);
      lc.setRow(0,1,e9f[1]);
      lc.setRow(0,2,e9f[2]);
      lc.setRow(0,3,e9f[3]);
      lc.setRow(0,4,e9f[4]);
      lc.setRow(0,5,e9f[5]);
      lc.setRow(0,6,e9f[6]);
      lc.setRow(0,7,e9f[7]);
      delay(facetime/3);
      lc.setRow(0,0,e10f[0]);
      lc.setRow(0,1,e10f[1]);
      lc.setRow(0,2,e10f[2]);
      lc.setRow(0,3,e10f[3]);
      lc.setRow(0,4,e10f[4]);
      lc.setRow(0,5,e10f[5]);
      lc.setRow(0,6,e10f[6]);
      lc.setRow(0,7,e10f[7]);
      delay(facetime/3);
      lc.setRow(0,0,e3f[0]);
      lc.setRow(0,1,e3f[1]);
      lc.setRow(0,2,e3f[2]);
      lc.setRow(0,3,e3f[3]);
      lc.setRow(0,4,e3f[4]);
      lc.setRow(0,5,e3f[5]);
      lc.setRow(0,6,e3f[6]);
      lc.setRow(0,7,e3f[7]);
      delay(facetime/3);
      lc.setRow(0,0,e1f[0]);
      lc.setRow(0,1,e1f[1]);
      lc.setRow(0,2,e1f[2]);
      lc.setRow(0,3,e1f[3]);
      lc.setRow(0,4,e1f[4]);
      lc.setRow(0,5,e1f[5]);
      lc.setRow(0,6,e1f[6]);
      lc.setRow(0,7,e1f[7]);
      delay(4*facetime);
      
      lc.setRow(0,0,e14f[0]);
      lc.setRow(0,1,e14f[1]);
      lc.setRow(0,2,e14f[2]);
      lc.setRow(0,3,e14f[3]);
      lc.setRow(0,4,e14f[4]);
      lc.setRow(0,5,e14f[5]);
      lc.setRow(0,6,e14f[6]);
      lc.setRow(0,7,e14f[7]);
      delay(facetime/2);
      lc.setRow(0,0,e1f[0]);
      lc.setRow(0,1,e1f[1]);
      lc.setRow(0,2,e1f[2]);
      lc.setRow(0,3,e1f[3]);
      lc.setRow(0,4,e1f[4]);
      lc.setRow(0,5,e1f[5]);
      lc.setRow(0,6,e1f[6]);
      lc.setRow(0,7,e1f[7]);
      delay(2*facetime);
      lc.setRow(0,0,e3f[0]);
      lc.setRow(0,1,e3f[1]);
      lc.setRow(0,2,e3f[2]);
      lc.setRow(0,3,e3f[3]);
      lc.setRow(0,4,e3f[4]);
      lc.setRow(0,5,e3f[5]);
      lc.setRow(0,6,e3f[6]);
      lc.setRow(0,7,e3f[7]);
      delay(facetime);
      lc.setRow(0,0,e1f[0]);
      lc.setRow(0,1,e1f[1]);
      lc.setRow(0,2,e1f[2]);
      lc.setRow(0,3,e1f[3]);
      lc.setRow(0,4,e1f[4]);
      lc.setRow(0,5,e1f[5]);
      lc.setRow(0,6,e1f[6]);
      lc.setRow(0,7,e1f[7]);
      delay(2*facetime);
      lc.setRow(0,0,e14f[0]);
      lc.setRow(0,1,e14f[1]);
      lc.setRow(0,2,e14f[2]);
      lc.setRow(0,3,e14f[3]);
      lc.setRow(0,4,e14f[4]);
      lc.setRow(0,5,e14f[5]);
      lc.setRow(0,6,e14f[6]);
      lc.setRow(0,7,e14f[7]);
      delay(facetime/2); 
      lc.setRow(0,0,e1f[0]);
      lc.setRow(0,1,e1f[1]);
      lc.setRow(0,2,e1f[2]);
      lc.setRow(0,3,e1f[3]);
      lc.setRow(0,4,e1f[4]);
      lc.setRow(0,5,e1f[5]);
      lc.setRow(0,6,e1f[6]);
      lc.setRow(0,7,e1f[7]);
      delay(2*facetime);
      lc.setRow(0,0,e7f[0]);
      lc.setRow(0,1,e7f[1]);
      lc.setRow(0,2,e7f[2]);
      lc.setRow(0,3,e7f[3]);
      lc.setRow(0,4,e7f[4]);
      lc.setRow(0,5,e7f[5]);
      lc.setRow(0,6,e7f[6]);
      lc.setRow(0,7,e7f[7]);
      delay(facetime);
      lc.setRow(0,0,e1f[0]);
      lc.setRow(0,1,e1f[1]);
      lc.setRow(0,2,e1f[2]);
      lc.setRow(0,3,e1f[3]);
      lc.setRow(0,4,e1f[4]);
      lc.setRow(0,5,e1f[5]);
      lc.setRow(0,6,e1f[6]);
      lc.setRow(0,7,e1f[7]);
      delay(2*facetime);
      lc.setRow(0,0,e14f[0]);
      lc.setRow(0,1,e14f[1]);
      lc.setRow(0,2,e14f[2]);
      lc.setRow(0,3,e14f[3]);
      lc.setRow(0,4,e14f[4]);
      lc.setRow(0,5,e14f[5]);
      lc.setRow(0,6,e14f[6]);
      lc.setRow(0,7,e14f[7]);
      delay(facetime/2);
      lc.setRow(0,0,e1f[0]);
      lc.setRow(0,1,e1f[1]);
      lc.setRow(0,2,e1f[2]);
      lc.setRow(0,3,e1f[3]);
      lc.setRow(0,4,e1f[4]);
      lc.setRow(0,5,e1f[5]);
      lc.setRow(0,6,e1f[6]);
      lc.setRow(0,7,e1f[7]);
      delay(3*facetime);
      
      lc.setRow(0,0,e12f[0]);
      lc.setRow(0,1,e12f[1]);
      lc.setRow(0,2,e12f[2]);
      lc.setRow(0,3,e12f[3]);
      lc.setRow(0,4,e12f[4]);
      lc.setRow(0,5,e12f[5]);
      lc.setRow(0,6,e12f[6]);
      lc.setRow(0,7,e12f[7]);
      delay(2*facetime);
      lc.setRow(0,0,e1f[0]);
      lc.setRow(0,1,e1f[1]);
      lc.setRow(0,2,e1f[2]);
      lc.setRow(0,3,e1f[3]);
      lc.setRow(0,4,e1f[4]);
      lc.setRow(0,5,e1f[5]);
      lc.setRow(0,6,e1f[6]);
      lc.setRow(0,7,e1f[7]);
      delay(facetime);
      lc.setRow(0,0,e14f[0]);
      lc.setRow(0,1,e14f[1]);
      lc.setRow(0,2,e14f[2]);
      lc.setRow(0,3,e14f[3]);
      lc.setRow(0,4,e14f[4]);
      lc.setRow(0,5,e14f[5]);
      lc.setRow(0,6,e14f[6]);
      lc.setRow(0,7,e14f[7]);
      delay(facetime/2);
      lc.setRow(0,0,e1f[0]);
      lc.setRow(0,1,e1f[1]);
      lc.setRow(0,2,e1f[2]);
      lc.setRow(0,3,e1f[3]);
      lc.setRow(0,4,e1f[4]);
      lc.setRow(0,5,e1f[5]);
      lc.setRow(0,6,e1f[6]);
      lc.setRow(0,7,e1f[7]);
      delay(2*facetime);
      lc.setRow(0,0,e12f[0]);
      lc.setRow(0,1,e12f[1]);
      lc.setRow(0,2,e12f[2]);
      lc.setRow(0,3,e12f[3]);
      lc.setRow(0,4,e12f[4]);
      lc.setRow(0,5,e12f[5]);
      lc.setRow(0,6,e12f[6]);
      lc.setRow(0,7,e12f[7]);
      delay(2*facetime);
      lc.setRow(0,0,e14f[0]);
      lc.setRow(0,1,e14f[1]);
      lc.setRow(0,2,e14f[2]);
      lc.setRow(0,3,e14f[3]);
      lc.setRow(0,4,e14f[4]);
      lc.setRow(0,5,e14f[5]);
      lc.setRow(0,6,e14f[6]);
      lc.setRow(0,7,e14f[7]);
      delay(facetime);
      lc.setRow(0,0,e13f[0]);
      lc.setRow(0,1,e13f[1]);
      lc.setRow(0,2,e13f[2]);
      lc.setRow(0,3,e13f[3]);
      lc.setRow(0,4,e13f[4]);
      lc.setRow(0,5,e13f[5]);
      lc.setRow(0,6,e13f[6]);
      lc.setRow(0,7,e13f[7]);
      delay(facetime/3);
      lc.setRow(0,0,e14f[0]);
      lc.setRow(0,1,e14f[1]);
      lc.setRow(0,2,e14f[2]);
      lc.setRow(0,3,e14f[3]);
      lc.setRow(0,4,e14f[4]);
      lc.setRow(0,5,e14f[5]);
      lc.setRow(0,6,e14f[6]);
      lc.setRow(0,7,e14f[7]);
      delay(facetime/3);
      lc.setRow(0,0,e13f[0]);
      lc.setRow(0,1,e13f[1]);
      lc.setRow(0,2,e13f[2]);
      lc.setRow(0,3,e13f[3]);
      lc.setRow(0,4,e13f[4]);
      lc.setRow(0,5,e13f[5]);
      lc.setRow(0,6,e13f[6]);
      lc.setRow(0,7,e13f[7]);
      delay(facetime/3);
      lc.setRow(0,0,e14f[0]);
      lc.setRow(0,1,e14f[1]);
      lc.setRow(0,2,e14f[2]);
      lc.setRow(0,3,e14f[3]);
      lc.setRow(0,4,e14f[4]);
      lc.setRow(0,5,e14f[5]);
      lc.setRow(0,6,e14f[6]);
      lc.setRow(0,7,e14f[7]);
      delay(facetime/3);
      lc.setRow(0,0,e13f[0]);
      lc.setRow(0,1,e13f[1]);
      lc.setRow(0,2,e13f[2]);
      lc.setRow(0,3,e13f[3]);
      lc.setRow(0,4,e13f[4]);
      lc.setRow(0,5,e13f[5]);
      lc.setRow(0,6,e13f[6]);
      lc.setRow(0,7,e13f[7]);
      delay(facetime/3);
      lc.setRow(0,0,e14f[0]);
      lc.setRow(0,1,e14f[1]);
      lc.setRow(0,2,e14f[2]);
      lc.setRow(0,3,e14f[3]);
      lc.setRow(0,4,e14f[4]);
      lc.setRow(0,5,e14f[5]);
      lc.setRow(0,6,e14f[6]);
      lc.setRow(0,7,e14f[7]);
      delay(facetime/3);
      lc.setRow(0,0,e13f[0]);
      lc.setRow(0,1,e13f[1]);
      lc.setRow(0,2,e13f[2]);
      lc.setRow(0,3,e13f[3]);
      lc.setRow(0,4,e13f[4]);
      lc.setRow(0,5,e13f[5]);
      lc.setRow(0,6,e13f[6]);
      lc.setRow(0,7,e13f[7]);
      delay(facetime/3);
      break;

      

  }
}

const PROGMEM bool gameOverMessage[8][100] = {
  {0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0,  0, 1, 1, 1, 1, 1, 0, 0,  1, 1, 1, 1, 1, 1, 0, 0,  1, 1, 1, 1, 1, 1, 0, 0,  0, 1, 1, 1, 1, 1, 0, 0,  0, 0, 0, 0,  0, 1, 1, 1, 1, 1, 0, 0,  0, 0, 1, 1, 1, 1, 1, 0,  0, 1, 1, 1, 1, 1, 0, 0,  1, 1, 1, 1, 1, 1, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0,  1, 1, 1, 1, 1, 1, 1, 0,  1, 1, 1, 1, 1, 1, 1, 0,  1, 1, 1, 1, 1, 1, 1, 0,  1, 1, 1, 1, 1, 1, 1, 0,  0, 0, 0, 0,  1, 1, 1, 1, 1, 1, 1, 0,  0, 1, 1, 1, 1, 1, 1, 0,  1, 1, 1, 1, 1, 1, 1, 0,  1, 1, 1, 1, 1, 1, 1, 0,  0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0,  1, 1, 0, 0, 0, 1, 1, 0,  0, 0, 0, 1, 0, 1, 1, 0,  0, 0, 0, 0, 0, 1, 1, 0,  1, 1, 0, 1, 0, 1, 1, 0,  0, 0, 0, 0,  1, 1, 0, 0, 0, 1, 1, 0,  1, 1, 1, 0, 0, 0, 0, 0,  1, 1, 0, 1, 0, 1, 1, 0,  0, 0, 1, 1, 0, 1, 1, 0,  0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0,  1, 1, 0, 0, 0, 1, 1, 0,  0, 0, 0, 1, 0, 1, 1, 0,  0, 0, 0, 0, 1, 0, 0, 0,  1, 1, 0, 1, 0, 1, 1, 0,  0, 0, 0, 0,  1, 1, 0, 0, 0, 1, 1, 0,  1, 1, 1, 0, 0, 0, 0, 0,  1, 1, 0, 1, 0, 1, 1, 0,  0, 1, 1, 1, 0, 1, 1, 0,  0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0,  1, 1, 0, 1, 0, 1, 1, 0,  0, 0, 0, 1, 0, 1, 1, 0,  0, 0, 0, 0, 0, 1, 1, 0,  1, 1, 0, 1, 0, 1, 1, 0,  0, 0, 0, 0,  1, 1, 0, 0, 0, 1, 1, 0,  0, 1, 1, 0, 0, 0, 0, 0,  1, 1, 0, 1, 0, 1, 1, 0,  1, 1, 0, 1, 0, 1, 1, 0,  0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0,  1, 1, 1, 1, 0, 1, 1, 0,  1, 1, 1, 1, 1, 1, 1, 0,  1, 1, 1, 1, 1, 1, 1, 0,  1, 1, 0, 0, 0, 1, 1, 0,  0, 0, 0, 0,  1, 1, 1, 1, 1, 1, 1, 0,  0, 1, 1, 1, 1, 1, 1, 0,  1, 1, 0, 0, 0, 1, 1, 0,  1, 1, 0, 1, 1, 1, 1, 0,  0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0,  0, 1, 1, 1, 0, 1, 1, 0,  1, 1, 1, 1, 1, 1, 0, 0,  1, 1, 1, 1, 1, 1, 0, 0,  1, 1, 0, 0, 0, 1, 1, 0,  0, 0, 0, 0,  0, 1, 1, 1, 1, 1, 0, 0,  0, 0, 1, 1, 1, 1, 1, 0,  1, 1, 0, 0, 0, 1, 1, 0,  1, 0, 0, 1, 1, 1, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0}
};
void showGameOverMessage() {
  for (int d = 0; d < sizeof(gameOverMessage[0]) - 7; d++) {
    
    for (int col = 0; col < 8; col++) {
      delay(messageSpeed);
      for (int row = 0; row < 8; row++) {
        // this reads the byte from the PROGMEM and displays it on the screen
        lc.setLed(0, row, col, pgm_read_byte(&(gameOverMessage[row][col + d])));
      }
    }
  }
}
