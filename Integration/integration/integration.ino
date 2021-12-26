///////// LCD //////////
// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = A0, en = A1, d4 = A2, d5 = A3, d6 = A4, d7 = A5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

///////// IR pins  //////////
int IRSensor = 2; // connect first ir sensor to arduino pin 2
int IRSensorClose=4; // connect second ir sensor to arduino pin 4

////////// RFID ///////////
#include <SPI.h>                // include SPI library to communicate with RFID.
#include <MFRC522.h>            // include RFID library to use its related functions.

  
#define RST_PIN         5          
#define SS_PIN          10         
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
#define RED_PIN          9      // Set Red led pin
#define GREEN_PIN        8      // Set Green led pin

/////// servo pins /////////
#include <Servo.h> // include servo library to use its related functions
#define Servo_PWM 6 // pin 6 of Arduino to provide PWM signal
Servo SG90_Servo;  // Define an instance of of Servo with the name of "SG90_Servo"


String BlueCard = " 167 238 66 178";    // Authenticated Card
double scanMillis;
bool passed = false;

//////////// return card id ////////////
String getCardID(MFRC522 object) {
  String cardId="";
  for (byte i = 0; i < object.uid.size; i++) {
    if(object.uid.uidByte[i] < 0x10)
      cardId+=(F(" 0"));
    else
      cardId+=(F(" "));
    cardId+=object.uid.uidByte[i];
  } 
  return cardId;
} 

void setup() 
{
  Serial.begin(9600);    // Initialize serial communications with the PC
  pinMode (IRSensor, INPUT); // IR sensor pin INPUT
  pinMode (RED_PIN, OUTPUT); // Led pin OUTPUT
  pinMode (GREEN_PIN, OUTPUT); // Led pin OUTPUT
  pinMode (IRSensorClose, INPUT); // IR sensor pin INPUT
  
  //////// LCD setup  ////////
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  //////// RFID setup /////////
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  delay(4);       // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  //////// servo setup ///////
  SG90_Servo.attach(Servo_PWM);
  SG90_Servo.write(0);
  lcd.print("Welcome!      ");
}



void loop()
{
  int statusSensor = digitalRead (IRSensor);
  int closeStatusSensor=digitalRead(IRSensorClose);
  lcd.setCursor(0, 0);
  // Exit State --> a car exits the gate. 
  if(!closeStatusSensor){
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
      lcd.print("busy!            ");
      SG90_Servo.write(0);
      passed=false;
  }
  else {
    
    // Idle state --> there is no car in front of the gate
    if (statusSensor == 1  && passed==false)
    {
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
      lcd.print("Welcome!      ");
//      SG90_Servo.write(0);
      passed = false;
    }
    
    // Car State --> there is a car in front of the gate
    else if(statusSensor == 0  && passed==false)
    
    {
      lcd.print("Scan your card");
      // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
      if ( ! mfrc522.PICC_IsNewCardPresent()) {
        return;
      }
      
      // Select one of the cards
      if ( ! mfrc522.PICC_ReadCardSerial()) {
        return;
      }
      
        String s = getCardID(mfrc522);
        
        scanMillis = millis();
        // Pass State --> Scanned authenticated card
        if(s == BlueCard){
          Serial.println("correct access");
          lcd.setCursor(0, 0);
          lcd.print("pass          ");  
          SG90_Servo.write(90);
          digitalWrite(GREEN_PIN, HIGH);
          passed = true;
  
        }
        // Fail State --> Scanned unauthenticated card
        else{
          Serial.println("incorrect access");
          lcd.setCursor(0, 0);
          lcd.print("Faild         ");  
          SG90_Servo.write(0);
          digitalWrite(RED_PIN, HIGH);
          while(millis()-scanMillis <3000);
          digitalWrite(RED_PIN, LOW);
          }
    }
  }
}
