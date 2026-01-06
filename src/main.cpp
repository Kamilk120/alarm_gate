#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

//code array max elements
#define MAX 8

const byte ROWS = 4; 
const byte COLS = 4; 
const byte SPEAKER = 13;
const byte MOVE = 4;

const int ALARM_DELAY = 30;

char hexaKeys[ROWS][COLS] = {
  {'A','3','2','1'},
  {'B','6','5','4'},
  {'C','9','8','7'},
  {'D','#','0','*'}
};

byte rowPins[ROWS] = {12, 11, 10, 9};
byte colPins[COLS] = {5, 6, 7, 8}; 

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

// inicial lcd
LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {

  Serial.begin(9600);

  pinMode(SPEAKER, OUTPUT);

  pinMode(MOVE, INPUT);

  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("Hello World");
  delay(1000);
  lcd.clear();
}

//pass virabless
char code_number[MAX];
int field_code_number = 0;

//secure alarm
bool secure = 0;

//alarm viarable
bool alarm = 0;

// alarm delay
bool alarm_count = 0;
int alarm_delay = ALARM_DELAY;
unsigned long alarm_count_start;

//pass
char admin[MAX] = {'1','2','3','4','5','6','7','8'};
char user[MAX] = {'1','2','3','4'};

// write number to array
bool add(char tab[], int *count, char value);

bool admin_pass_check();

bool user_pass_check();

void loop() {

  //keabord input
  char customKey = customKeypad.getKey();

  if(secure){

    lcd.setCursor(6,0);
    lcd.print("secure");

    if(digitalRead(MOVE) == HIGH){
      lcd.setCursor(0,0);
      lcd.print("hight");

      //set alarm
      alarm_count_start = millis();
      alarm_count = 1;
    }

    if(digitalRead(MOVE) == LOW){
      lcd.setCursor(0,0);
      lcd.print("low  ");
    }
  }else{
    lcd.setCursor(6,0);
    lcd.print("disable");
    alarm = 0;
  }

  //alarm delay
  if(alarm_count){
    if(millis() - alarm_count_start >= 1000){
      lcd.setCursor(14,1);
      lcd.print(alarm_count);
      alarm_delay--;
    }
    if(alarm_delay <= 0){
      alarm = 1;
    }
  }

  // set alarm
  if (alarm){
    digitalWrite(SPEAKER,HIGH);
  }else{
    digitalWrite(SPEAKER,LOW);
  }
  

  //keybord
  if (customKey){

    if(customKey == '#'){
      
      // code check
      if(admin_pass_check() || user_pass_check()){
        secure = !secure;
        lcd.setCursor(0,0);
        lcd.print("     ");
      }

      lcd.setCursor(1,1);
      for(int i = 0; i < MAX ;i++){
        code_number[i] = ' ';
        lcd.print(' ');
        field_code_number = 0;
      }
    }else{

      Serial.println(customKey);

      if(add( code_number, &field_code_number, customKey)){
        //if array is overrate
      }
      
      lcd.setCursor(1,1);
      for(int i = 0; i < field_code_number ;i++){
        lcd.print(code_number[i]);
      }

    }

    digitalWrite(SPEAKER,HIGH);

    //unsigned long lastTime = millis();

    delay(10);
    digitalWrite(SPEAKER,LOW);
  }
}

bool add(char tab[], int *count, char value) {
  if (*count < MAX) {
      tab[*count] = value;
      (*count)++;
  } else {
      return 0;
  }
  return 1;
}

bool admin_pass_check(){
  for(int i = 0; i < field_code_number ;i++){
    if(admin[i] != code_number[i]){
      return 0;
    }
  }
  return 1;
}

bool user_pass_check(){
  for(int i = 0; i < field_code_number ;i++){
    if(user[i] != code_number[i]){
      return 0;
    }
  }
  return 1;
}
