#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 4; 
const byte SPEAKER = 13;
const byte MOVE = 4;

char hexaKeys[ROWS][COLS] = {
  {'A','3','2','1'},
  {'B','6','5','4'},
  {'C','9','8','7'},
  {'D','*','0','#'}
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

}

void loop() {

  char customKey = customKeypad.getKey();
  if(digitalRead(MOVE) == HIGH){
  }
  if (customKey){
    Serial.println(customKey);
    lcd.setCursor(2,1);
    lcd.print(customKey);
    digitalWrite(SPEAKER,HIGH);
    delay(10);
    digitalWrite(SPEAKER,LOW);
  }
}
