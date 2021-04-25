#include <Keypad_I2C.h>
#include <Keypad.h>
#include <Wire.h>

#define i2c_addr 0x27
#define REAL_PW 2002
/*
  SevenSegment Class for controlling sevensegment.
  Using in Sirinz Project.
  @author TSphere
  25/04/2021
*/
class SevenSegment {
  private :
    /* Digital Signal for drawing number on seven segment. */
    const int NUMBER0[7] = {1, 1, 1, 1, 1, 1, 0};const int NUMBER1[7] = {0, 1, 1, 0, 0, 0, 0};const int NUMBER2[7] = {1, 1, 0, 1, 1, 0, 1};const int NUMBER3[7] = {1, 1, 1, 1, 0, 0, 1};const int NUMBER4[7] = {0, 1, 1, 0, 0, 1, 1};const int NUMBER5[7] = {1, 0, 1, 1, 0, 1, 1};const int NUMBER6[7] = {1, 0, 1, 1, 1, 1, 1};const int NUMBER7[7] = {1, 1, 1, 0, 0, 1, 0};const int NUMBER8[7] = {1, 1, 1, 1, 1, 1, 1};const int NUMBER9[7] = {1, 1, 1, 1, 0, 1, 1};
    /* Storing ABCDEFGDP segment output pin */
    int segmentPin[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    /* Storing digit common output pin */
    int digitCommonPin[4] = {0, 0, 0, 0};
    /* Weather the sevensegment has DP or not */
    boolean hasDP;
    /* Set pinmode for SevenSegment */
    void initPinMode() {if (this->hasDP) {for (int i = 0 ; i < 8; i++ ){pinMode(this->segmentPin[i], OUTPUT);}}else {for (int i = 0 ; i < 7; i++ ){pinMode(this->segmentPin[i], OUTPUT);}}for (int i = 0 ; i < 4 ; i++ ) {pinMode(this->digitCommonPin[i], OUTPUT);}}
  public :
    /* Constructor for create Sevensegment */
    SevenSegment(int segment_common_pin[11], boolean hasDP){if (!hasDP ) {for (int i = 0 ; i < 7; i++ ){this->segmentPin[i] = segment_common_pin[i];}for (int i = 7 ; i < 11 ; i++ ) {this->digitCommonPin[i - 7] = segment_common_pin[i];}}else{for (int i = 0 ; i < 8; i++ ){this->segmentPin[i] = segment_common_pin[i];}for (int i = 8 ; i < 11 ; i++ ) {this->digitCommonPin[i - 8] = segment_common_pin[i];}}this->hasDP = hasDP;this->initPinMode();}
    /* Draw number on segment */
    void drawNumber(int number) {
      switch (number){
        case 0:{for (int i = 0; i < 7; i++){digitalWrite(this->segmentPin[i], NUMBER0[i]);}break;}
        case 1:{for (int i = 0; i < 7; i++){digitalWrite(this->segmentPin[i], NUMBER1[i]);}break;}
        case 2:{for (int i = 0; i < 7; i++){digitalWrite(this->segmentPin[i], NUMBER2[i]);}break;}
        case 3:{for (int i = 0; i < 7; i++){digitalWrite(this->segmentPin[i], NUMBER3[i]);}break;}
        case 4:{for (int i = 0; i < 7; i++){digitalWrite(this->segmentPin[i], NUMBER4[i]);}break;}
        case 5:{for (int i = 0; i < 7; i++){digitalWrite(this->segmentPin[i], NUMBER5[i]);}break;}
        case 6:{for (int i = 0; i < 7; i++){digitalWrite(this->segmentPin[i], NUMBER6[i]);}break;}
        case 7:{for (int i = 0; i < 7; i++){digitalWrite(this->segmentPin[i], NUMBER7[i]);}break;}
        case 8:{for (int i = 0; i < 7; i++){digitalWrite(this->segmentPin[i], NUMBER8[i]);}break;}
        case 9:{for (int i = 0; i < 7; i++){digitalWrite(this->segmentPin[i], NUMBER9[i]);}break;}
        case -1:{for (int i = 0; i < 7; i++){digitalWrite(this->segmentPin[i], 0);}break;}}}
    /* Select digit common cathode */
    void selectDigit(int digit) {
      switch (digit){
        case 1:{for (int i = 0; i < 4; i++){digitalWrite(this->digitCommonPin[i], LOW);}digitalWrite(this->digitCommonPin[0], HIGH);break;}
        case 2:{for (int i = 0; i < 4; i++){digitalWrite(this->digitCommonPin[i], LOW);}digitalWrite(this->digitCommonPin[1], HIGH);break;}
        case 3:{for (int i = 0; i < 4; i++){digitalWrite(this->digitCommonPin[i], LOW);}digitalWrite(this->digitCommonPin[2], HIGH);break;}
        case 4:{for (int i = 0; i < 4; i++){digitalWrite(this->digitCommonPin[i], LOW);}digitalWrite(this->digitCommonPin[3], HIGH);break;}
        case 0: /* Case 0 for selecting all common.*/{for (int i = 0; i < 4; i++){digitalWrite(this->digitCommonPin[i], HIGH);}break;}}}
    /* Write 4 digit integer number */
    void writeNumber(int number) {const int time_delay = 3;int a, b, c, d;a = (number / 1000) % 10;b = (number / 100) % 10;c = (number / 10) % 10;d = (number / 1) % 10;int digit_number_array[] = {a, b, c, d};for (int i = 0; i < 4; i++){this->selectDigit(i + 1);this->drawNumber(digit_number_array[i]);delay(time_delay);}}
    /* Draw the number at the specified digit */
    void writeNumberAtDigit(int number , int digit) {this->selectDigit(digit);this->drawNumber(number);}
    /* Write number from the array for specific amount of the digit */
    void writeNumberFromArray(int numbers[] ,int amount) {const int time_delay = 4;for(int i = 0 ; i< amount ; i++ ) {this->writeNumberAtDigit(numbers[i],i+1);delay(time_delay);}}
};
int pressCount = 0;
int preNum[4] = {0};
int pw = 0;
bool pass = false;
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
//define the cymbols on the buttons of the keypads
char keypadArray[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {7, 6, 5, 4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {2, 1, 0}; //connect to the column pinouts of the keypad

//initialize an instance of the Keypad class
Keypad_I2C myKeypad(makeKeymap(keypadArray), rowPins, colPins, ROWS, COLS, i2c_addr, PCF8574);
//create a 7 Segment Display library intance

int segmentPin[] = {7, 6, 5, 4, 3, 2, 1, 11, 10, 9, 8};
SevenSegment sevseg(segmentPin, false);

void setup() {
  //  Serial.begin(9600);
  Wire.begin();
  pinMode(12, OUTPUT);
  pinMode(A3, OUTPUT);
  myKeypad.begin(makeKeymap(keypadArray));
}

void loop() {
  if (pass == false) {
    digitalWrite(A3, HIGH);
  }
  if ((analogRead(A0) < 700 | analogRead(A1) < 700 | analogRead(A2) < 700) && pass == false) {
    pass = true;
    digitalWrite(12, HIGH);
    digitalWrite(A3, LOW);
  }
  if (pass == true) {
    //    Serial.println("pass");
    char key = myKeypad.getKey();
    int num = key - '0';
    if (key) {
      if (key == '*') {
        preNum[4] = {0};
        pressCount = 0;
        pass = true;
      }
      else if (key == '#') {
        if (pw == REAL_PW) {
          pass = false;
          preNum[4] = {0};
          pressCount = 0;
          digitalWrite(12, LOW);
        }
        else {
          //ลำโพงดัง
        }
      }
      else if (pressCount <= 3) {
        preNum[pressCount] = num;
        sevseg.writeNumberFromArray(preNum, pressCount + 1);
        pressCount++;
      }
    }
  }
}
