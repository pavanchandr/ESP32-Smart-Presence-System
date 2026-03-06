#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA 21
#define OLED_SCL 22

#define PIR_PIN 25
#define IR_PIN 26
#define LDR_PIN 27

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

bool motionDetected;
bool objectNear;
bool isDark;

enum SystemState {
  IDLE,
  PRESENCE,
  USER_NEAR,
  DARK_ACTIVITY
};

SystemState currentState;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(IR_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);

  Wire.begin(OLED_SDA, OLED_SCL);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println("OLED failed");
    while(true);
  }
  display.clearDisplay();
}

void readSensors(){

  motionDetected = digitalRead(PIR_PIN);
  objectNear = digitalRead(IR_PIN);

  int lightValue = digitalRead(LDR_PIN);

  if(lightValue == LOW){
    isDark = false;
  }
  else{
    isDark = true;
  };
}

void evaluteState(){

  if(!motionDetected){
    currentState = IDLE;
  }
  else if(motionDetected && objectNear){
    currentState = PRESENCE;
  }
  else if(motionDetected && !objectNear && !isDark){
    currentState = USER_NEAR;
  }
  else if(motionDetected && !objectNear && isDark){
    currentState = DARK_ACTIVITY;
  }
}

void updateDisplay(){
  
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0,0);
  display.print("Motion: ");
  display.println(motionDetected ? "YES" : "NO");

  display.print("IR: ");
  display.println(objectNear ? "NEAR" : "FAR");

  display.print("Light: ");
  display.println(isDark ? "DARK" : "BRIGHT");
  
  switch(currentState){
    case IDLE: display.println("IDLE"); break;
    case PRESENCE: display.println("PRESENCE"); break;
    case USER_NEAR: display.println("USER_NEAR"); break;
    case DARK_ACTIVITY: display.println("DARK_ACTIVITY"); break;
  }

  display.display();

  }
void loop() {
  // put your main code here, to run repeatedly:
  readSensors();
  evaluteState();
  updateDisplay();

  delay(300);
}
