#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

const int motorAPin1 = 27;
const int motorAPin2 = 26;
const int motorBPin1 = 25;
const int motorBPin2 = 33;
const int dirPin = 18;
const int stepPin = 19;
const int stepDelay = 10000;

// in1 5 in2 18 in3 19 in4 21

 typedef struct {
  int left_x;
  int left_y;
  int right_x;
  int right_y;
  bool switchState1;
  bool switchState2;
} JoystickData;

JoystickData receivedData;

int leftjoystickCenterY = 1930;
int leftjoystickCenterX = 1800;
int rightjoystickCenterY = 1930;
int rightjoystickCenterX = 1850;
int leftDeadzoneY = 150;
int leftDeadzoneX = 50;
int rightDeadzoneY = 150;
int rightDeadzoneX = 50;
int rightTolerance = 300;

void onDataReceived(const uint8_t* mac, const uint8_t* data, int len) {
    memcpy(&receivedData, data, sizeof(receivedData));


  

  int stepMap = receivedData.right_x;
  


// Control stepper motor based on speed
if (stepMap >= 1810 && stepMap <= 1880) {
  // no movement
  digitalWrite(stepPin, LOW);
}

if (stepMap > 1880 ) {
  // move left
  digitalWrite(dirPin, LOW); // Set direction to counter-clockwise
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(stepDelay);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(stepDelay);
}

if (stepMap < 1810) {
  // move right
  digitalWrite(dirPin, HIGH); // Set direction to clockwise
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(stepDelay);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(stepDelay);
}





    int yDifference = receivedData.left_y - leftjoystickCenterY;
    int xDifferenceLeft = receivedData.left_x - leftjoystickCenterX;

    if (abs(yDifference) > leftDeadzoneY || abs(xDifferenceLeft) > leftDeadzoneX) {
        if (yDifference < -leftDeadzoneY) {
            digitalWrite(motorAPin1, HIGH);
            digitalWrite(motorAPin2, LOW);
            digitalWrite(motorBPin1, HIGH);
            digitalWrite(motorBPin2, LOW);
        } else if (yDifference > leftDeadzoneY) {
            digitalWrite(motorAPin1, LOW);
            digitalWrite(motorAPin2, HIGH);
            digitalWrite(motorBPin1, LOW);
            digitalWrite(motorBPin2, HIGH);
        } else {
            digitalWrite(motorAPin1, LOW);
            digitalWrite(motorAPin2, LOW);
            digitalWrite(motorBPin1, LOW);
            digitalWrite(motorBPin2, LOW);
        }

        if (xDifferenceLeft < -leftDeadzoneX) {
            digitalWrite(motorAPin1, HIGH);
            digitalWrite(motorAPin2, LOW);
            digitalWrite(motorBPin1, LOW);
            digitalWrite(motorBPin2, HIGH);
        } else if (xDifferenceLeft > leftDeadzoneX) {
            digitalWrite(motorAPin1, LOW);
            digitalWrite(motorAPin2, HIGH);
            digitalWrite(motorBPin1, HIGH);
            digitalWrite(motorBPin2, LOW);
        }
    } else {
        digitalWrite(motorAPin1, LOW);
        digitalWrite(motorAPin2, LOW);
        digitalWrite(motorBPin1, LOW);
        digitalWrite(motorBPin2, LOW);
    }
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    esp_now_register_recv_cb(onDataReceived);


    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);

    pinMode(motorAPin1, OUTPUT);
    pinMode(motorAPin2, OUTPUT);
    pinMode(motorBPin1, OUTPUT);
    pinMode(motorBPin2, OUTPUT);
  
}

void loop() {

  Serial.printf("Left X: %d, Left Y: %d, Right X: %d, Right Y: %d, Switch1: %d,Switch2: %d \n", receivedData.left_x, receivedData.left_y, receivedData.right_x, receivedData.right_y,receivedData.switchState1,receivedData.switchState2);


}