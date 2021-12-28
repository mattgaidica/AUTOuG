#include "ADS1X15.h"
#include <Stepper.h>

ADS1115 ADS(0x48);

const int BTN1 = 2;
const int BTN2 = 3;
const int LED1 = 4;
const int LED2 = 5;
const int stepsPerRevolution = 200;
int isInit = 1;
Stepper myStepper(stepsPerRevolution,6,7,8,9);
int16_t val_0_init = 0;
const int16_t adcError = 40;
const int stepAmt = 10;

void setup() {
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);      
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  myStepper.setSpeed(60);
  
  Serial.begin(115200);
  Serial.println("Init...");
  ADS.begin();
  ADS.setGain(16);
}

void checkInput() {
  if (digitalRead(BTN1) == LOW && digitalRead(BTN2) == LOW) {
    if (ADS.isReady()) {
      delay(200);
      val_0_init = ADS.readADC_Differential_0_1();
      isInit = 0;
    }
  } else {
    if (digitalRead(BTN1) == LOW) {
      digitalWrite(LED1, HIGH);
      myStepper.step(5);
    }
    if (digitalRead(BTN2) == LOW) {
      digitalWrite(LED2, HIGH);
      myStepper.step(-5);
    }
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
  }
}

void loop() {
  if (isInit == 0) {
    if (ADS.isReady()) {
      int16_t val_0 = ADS.readADC_Differential_0_1();
    
      Serial.print("\tAnalog0-1: ");
      Serial.print(val_0);
      Serial.print("\tInit0-1: ");
      Serial.println(val_0_init);
      if (val_0 > val_0_init + adcError) {
        myStepper.step(stepAmt);
        digitalWrite(LED1, HIGH);
      } else if (val_0 < val_0_init - adcError) {
        myStepper.step(-stepAmt);
        digitalWrite(LED2, HIGH);
      }
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
    }
  }
  checkInput();
}
