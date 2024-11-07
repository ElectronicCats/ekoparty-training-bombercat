#define GPIO0_PIN 20  // Arduino pin connected to GPIO0 pin on ESP32
#define RESET_PIN 24 // Arduino pin connected to RESET pin on ESP32
#define BUTTON_1 5


void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_1,INPUT_PULLUP);
  pinMode(GPIO0_PIN, OUTPUT);
  pinMode(RESET_PIN, OUTPUT);
  
  digitalWrite(NINA_GPIO0, HIGH);
  resetESP();
}

bool flagboot=0;

void loop() {
  
  if(!digitalRead(BUTTON_1)){
    while(!digitalRead(BUTTON_1)){
      digitalWrite(LED_BUILTIN, HIGH);
      }  
    digitalWrite(LED_BUILTIN, LOW);
    enterFlashMode();
  }
  
  if (Serial.available()) {      // If anything comes in Serial (USB),
    Serial2.write(Serial.read());   // read it and send it out Serial1 (pins 0 & 1)
  }

  if (Serial2.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    Serial.write(Serial2.read());   // read it and send it out Serial (USB)
  }
  
}

// Enter flashing mode
void enterFlashMode() {
  digitalWrite(NINA_GPIO0, LOW);

  digitalWrite(NINA_RESETN, HIGH);
  delay(100);
  digitalWrite(NINA_RESETN, LOW);
  delay(100);
  digitalWrite(NINA_RESETN, HIGH);
  // Now the ESP32 should be in flashing mode
}

void resetESP() {
  digitalWrite(NINA_GPIO0, HIGH);

  digitalWrite(NINA_RESETN, HIGH);
  delay(100);
  digitalWrite(NINA_RESETN, LOW);
  delay(100);
  digitalWrite(NINA_RESETN, HIGH);

  // Now the ESP32 should be in flashing mode
}
