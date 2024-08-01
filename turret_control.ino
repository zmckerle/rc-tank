#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>




Servo myServo;
Servo myServoUp; 
int servoPin = 12;  
int servoUp = 32;
const int motorIN1 = 18;  // Input 1 of the H-bridge
const int motorIN2 = 21;  // Input 2 of the H-bridge



 typedef struct {
  int left_x;
  int left_y;
  int right_x;
  int right_y;
  bool switchState1;
  bool switchState2;
} JoystickData;
JoystickData receivedData;

 

void onDataReceived(const uint8_t* mac, const uint8_t* data, int len) {
    memcpy(&receivedData, data, sizeof(receivedData));
 


 int up = map(receivedData.right_y, 0, 4095, 30, 70); // Adjust 4095 to the maximum value of your joystick

   myServoUp.write(up);



    if (receivedData.switchState1==1) {

    digitalWrite(motorIN1,HIGH);
    digitalWrite(motorIN2,LOW);


    
     } else{
      digitalWrite(motorIN1,LOW);
      digitalWrite(motorIN2,LOW);

      }


  


   if (receivedData.switchState2==1) {


    myServo.write(25);
    
     } else{
     
      myServo.write(160);
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

  myServo.attach(servoPin);
  myServoUp.attach(servoUp);
  pinMode(motorIN1, OUTPUT);
  pinMode(motorIN2, OUTPUT);

// set pin modes here



}

void loop() {





    Serial.printf("Left X: %d, Left Y: %d, Right X: %d, Right Y: %d, Switch1: %d,Switch2: %d\n", 
                   receivedData.left_x, receivedData.left_y, 
                   receivedData.right_x, receivedData.right_y,
                   receivedData.switchState1, receivedData.switchState2);
                   delay(50);
                   

}
