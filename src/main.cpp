#include <Arduino.h>
#include "BluetoothSerial.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

BluetoothSerial SerialBT;
//Device1 A0:B7:65:4B:EA:DA
//Device2 E0:5A:1B:E4:EF:6E

uint8_t address[6]  = {0xA0, 0xB7, 0x65, 0x4B, 0xEA, 0xDA}; //DEVICE1
String deviceName = "Device2";

// uint8_t address[6]  = {0xE0, 0x5A, 0x1B, 0xE4, 0xEF, 0x6E}; //DEVICE2
// String deviceName = "Device1";

bool connected;
bool recIsPrinted = false;
bool senIsPrinted = false;

const int bState = 26;

const int buttL = 27;
const int buttM = 14;
const int buttR = 12;
const int vibr = 25;

String patterns = "XXXXXX";
String msg;

int buttonFlip;
int arrayCount = 0;

void senderMode();
void receiverMode();

void patternReset(){
    arrayCount = 0;
    patterns[0] = 'X';
    patterns[1] = 'X';
    patterns[2] = 'X';
    patterns[3] = 'X';
    patterns[4] = 'X';
    patterns[5] = 'X';
}

String morseConverter(String msgIn){
  String conv;

  if(msgIn == "LMXXXX"){
    Serial.println("A");
    conv = "A";
  }else if(msgIn == "MLLLXX"){
    Serial.println("B");
    conv = "B";
  }else if(msgIn == "MLMLXX"){
    Serial.println("C");
    conv = "C";
  }else if(msgIn == "MLLXXX"){
    Serial.println("D");
    conv = "D";
  }else if(msgIn == "LXXXXX"){
    Serial.println("E");
    conv = "E";
  }else if(msgIn == "LLMLXX"){
    Serial.println("F");
    conv = "F";
  }else if(msgIn == "MMLXXX"){
    Serial.println("G");
    conv = "G";
  }else if(msgIn == "LLLLXX"){
    Serial.println("H");
    conv = "H";
  }else if(msgIn == "LLXXXX"){
    Serial.println("I");
    conv = "I";
  }else if(msgIn == "LMMMXX"){
    Serial.println("J");
    conv = "J";
  }else if(msgIn == "MLMXXX"){
    Serial.println("K");
    conv = "K";
  }else if(msgIn == "LMLLXX"){
    Serial.println("L");
    conv = "L";
  }else if(msgIn == "MMXXXX"){
    Serial.println("M");
    conv = "M";
  }else if(msgIn == "MLXXXX"){
    Serial.println("N");
    conv = "N";
  }else if(msgIn == "MMMXXX"){
    Serial.println("O");
    conv = "O";
  }else if(msgIn == "LMMLXX"){
    Serial.println("P");
    conv = "P";
  }else if(msgIn == "MMLMXX"){
    Serial.println("Q");
    conv = "Q";
  }else if(msgIn == "LMLXXX"){
    Serial.println("R");
    conv = "R";
  }else if(msgIn == "LLLXXX"){
    Serial.println("S");
    conv = "S";
  }else if(msgIn == "MXXXXX"){
    Serial.println("T");
    conv = "T";
  }else if(msgIn == "LLMXXX"){
    Serial.println("U");
    conv = "U";
  }else if(msgIn == "LLLMXX"){
    Serial.println("V");
    conv = "V";
  }else if(msgIn == "LMMXXX"){
    Serial.println("W");
    conv = "W";
  }else if(msgIn == "LMMLXX"){
    Serial.println("X");
    conv = "X";
  }else if(msgIn == "MLMMXXX"){
    Serial.println("Y");
    conv = "Y";
  }else if(msgIn == "MMLLXX"){
    Serial.println("Z");
    conv = "Z";
  }else if(msgIn == "LMMMMX"){
    Serial.println("1");
    conv = "1";
  }else if(msgIn == "LLMMMX"){
    Serial.println("2");
    conv = "2";
  }else if(msgIn == "LLLMMX"){
    Serial.println("3");
    conv = "3";
  }else if(msgIn == "LLLLMX"){
    Serial.println("4");
    conv = "4";
  }else if(msgIn == "LLLLLX"){
    Serial.println("5");
    conv = "5";
  }else if(msgIn == "MLLLLX"){
    Serial.println("6");
    conv = "6";
  }else if(msgIn == "MMLLLX"){
    Serial.println("7");
    conv = "7";
  }else if(msgIn == "MMMLLX"){
    Serial.println("8");
    conv = "8";
  }else if(msgIn == "MMMMLX"){
    Serial.println("9");
    conv = "9";
  }else if(msgIn == "MMMMMX"){
    Serial.println("0");
    conv = "0";
  }
  else{
    Serial.println("Invalid");
    conv = "-";
  }

  return conv;
}

void setup() {
  pinMode(vibr, OUTPUT);
  pinMode(buttL, INPUT_PULLUP);
  pinMode(buttM, INPUT_PULLUP);
  pinMode(buttR, INPUT_PULLUP);

  pinMode(bState, INPUT);

  digitalWrite(vibr, HIGH);
  delay(250);
  digitalWrite(vibr, LOW);
  delay(200);
  digitalWrite(vibr, HIGH);
  delay(500);
  digitalWrite(vibr, LOW);


  lcd.begin();
	lcd.backlight();
  lcd.println("begin...");
  lcd.clear();

  Serial.begin(115200);
 
}


void loop() {
  int bState_val = digitalRead(bState);
  // Serial.println(bState_val);

  if(bState_val == 0){

    if(!recIsPrinted){
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.println("Receiving Mode...");
      recIsPrinted = true;
      SerialBT.disconnect();
      SerialBT.end();
      SerialBT.begin(deviceName);
      Serial.println("Rec Mode");
    }else{
      senIsPrinted = false;
    }

    receiverMode();

  }else{

    SerialBT.begin(deviceName, true); 
    if(!senIsPrinted){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.println("Sending Mode...");
      senIsPrinted = true;
    }else{
      recIsPrinted = false;
    }
    
    if(!SerialBT.connect(address)){
      SerialBT.connect(address);
      lcd.setCursor(0,1);
      lcd.println("Connecting...");
    }else{
      lcd.setCursor(0,1);
      lcd.println("Connected!");
    }
    senderMode();

  }

  
}

void senderMode(){

  if(arrayCount > 5){
    patternReset();
  }

  if(digitalRead(buttL) == 0 && digitalRead(buttM) == 0 && digitalRead(buttR == 0)){
    digitalWrite(vibr, HIGH);
    delay(1000);
    digitalWrite(vibr, LOW);
    delay(50);
    patternReset();                                                                                                                                                                                                                            
  }
  else{

    if(digitalRead(buttR) == 0){

      buttonFlip = 0;

      if(digitalRead(buttL) == 0){

        Serial.print("L");
        digitalWrite(vibr, HIGH);
        delay(100);
        digitalWrite(vibr, LOW);
        patterns[arrayCount] = 'L';
        arrayCount++;
        }

      if(digitalRead(buttM) == 0){
        Serial.print("M");
        digitalWrite(vibr, HIGH);
        delay(200);
        digitalWrite(vibr, LOW);
        patterns[arrayCount] = 'M';
        arrayCount++; 
      }
    }
    else{
      // Serial.println(patterns);
      if(buttonFlip == 0){
        // Serial.print("send: ");
        // Serial.println(patterns);
        msg = morseConverter(patterns);
        if(msg == "-"){
          digitalWrite(vibr, HIGH);
          delay(50);
          digitalWrite(vibr, LOW);
          delay(50);
          digitalWrite(vibr, HIGH);
          delay(50);
          digitalWrite(vibr, LOW);
          delay(50);
          digitalWrite(vibr, HIGH);
          delay(50);
          digitalWrite(vibr, LOW);
        }else{
          SerialBT.println(msg);
        }
        buttonFlip = 1;
      }
      patternReset();
      delay(20);
  }

  }

  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }

}

void receiverMode(){
  while(SerialBT.available()){
    delay(10);
    String c = SerialBT.readString();
    msg += c;
  }

  if(msg.length()>0){
    
    lcd.setCursor(0,0);
    lcd.print(msg[0]);

    
    msg="";
    
  }
}