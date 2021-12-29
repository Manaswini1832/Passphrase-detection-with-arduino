// Include keypad and LCD libraries
#include <Keypad.h>
#include <LiquidCrystal.h>

// Sound for failed auth
#define NOTE_B2  123 
//Sound for successful auth
#define NOTE_G3  196


String keyPhrase = "AB2351#";
String typedString;
int btn1 = 0;// Keeps track of checkPhrase button state
//int btn2 = 0;// Keeps track of reset button state

//LCD initialization
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

// Keypad initializations
const int ROWS = 4;
const int COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

byte pin_rows[ROWS] = {9, 8, 7, 6};
byte pin_column[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROWS, COLS );

int startCursor = 0;// Tells the LCD where to place the cursor

//Arduino pin to which the buzzer is connected
int buzzerPin = 13;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(10, OUTPUT);//Reset button
  pinMode(11, OUTPUT);//Check phrase button
}

void loop() {
  char enteredKey = keypad.getKey();
  btn1 = digitalRead(10);
  btn2 = digitalRead(11);
  if(enteredKey) {// first if
    Serial.println(enteredKey);
    lcd.setCursor(startCursor, 0);
    lcd.print(enteredKey);
    typedString += enteredKey;
    startCursor++;
    
    //If typed phrase reaches the end of the line, clear the LCD and start afresh
    if(startCursor == 16){//second if statement
      lcd.clear();
      typedString = "";
      startCursor = 0;
    }//end of second if
  }//end of first if

   //If a reset button is connected use this
  //if(btn2 == HIGH){
  //    Serial.println("Button1 is high");
  //    lcd.clear();
  //    startCursor = 0;
  //    typedString = "";
  //}
  
  if(btn1 == HIGH){
    Serial.println("Button2 is high");
    if(typedString == keyPhrase){
        tone(buzzerPin, NOTE_G3, 130);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" Authentication ");
        lcd.setCursor(0, 1);
        lcd.print("  successful :) ");
        delay(1000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("You can enter");
        lcd.setCursor(0, 1);
        lcd.print("next passphrase");
        delay(2000);
        lcd.clear();
        startCursor = 0;
        typedString = "";
      }else{
        lcd.clear();
        tone(buzzerPin, NOTE_B2, 100);
        lcd.setCursor(0, 0);
        lcd.print(" Authentication ");
        lcd.setCursor(0, 1);
        lcd.print("    failed!     ");
        delay(1000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Please enter");
        lcd.setCursor(0, 1);
        lcd.print("passphrase again");
        delay(2000);
        lcd.clear();
        startCursor = 0;
        typedString = "";
      }
}
}
 
