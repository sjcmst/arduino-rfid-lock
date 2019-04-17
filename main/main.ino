#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
 
#define SS_PIN 10
#define RST_PIN 9
#define SERVO_PIN 3
#define LOCK_ANGLE 90

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
Servo myservo; //define servo name
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  Serial.println("Initializing...");
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  myservo.attach(SERVO_PIN); //servo pin
  Serial.println("Done");

  Serial.println("Put your card to the reader...");
  Serial.println();
}

void loop() 
{
  // Detect for a card 
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Read from the detected card
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "D9 42 BD 55") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    spinServo(0,LOCK_ANGLE,1,15);
    printDelay(5000);
    spinServo(LOCK_ANGLE,0,1,15);
    Serial.println("");
  }
  else {
    Serial.println(" Access denied");
    Serial.println("");
  }

  Serial.println("Put your card to the reader...");
}

void printDelay(int delayms){
  int count = delayms / 1000;
  if(count >= 1){
    Serial.print("Waiting : ");
    for(count; count > 0; count-=1){
      Serial.print(" ");
      Serial.print(count);
      delay(1000);
    }
    Serial.println("");
  }
  delay(delayms%1000);  
}
void spinServo(int posstart, int posend, int degree, int delayms){
    int pos = posstart;
    if (posstart < posend){
      Serial.print("Opening Door:");
      for (pos; pos <= posend; pos += degree) { // goes from start to end in steps of degree
        myservo.write(pos);                     // tell servo to go to position in variable 'pos'
        delay(delayms);  // waits 15ms for the servo to reach the position
        if (pos%10==0){
          Serial.print(" .");
        }
      }
      Serial.println(" !");
    }else{
      Serial.print("Closing Door:");
      for (pos; pos >= posend; pos -= degree) { // goes from start to end in steps of degree
        myservo.write(pos);                     // tell servo to go to position in variable 'pos'
        delay(delayms);  // waits 15ms for the servo to reach the position
        if (pos%10==0) {
          Serial.print(" .");
        }
      }
      Serial.println(" !");
    }
    
}
