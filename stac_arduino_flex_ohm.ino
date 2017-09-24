#include <SoftwareSerial.h>
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU9250.h"
SoftwareSerial BTSerial(2, 3);
/**
 * 자이로 센서 변수
 */
MPU9250 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;
float Axyz[3];

const int FLEX_PIN_T = A0; 
const int FLEX_PIN_M = A1;
const int FLEX_PIN_B = A2;
// 47k resistor
const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 47500.0; // Measured resistance of 3.3k resistor
int cnt_true = 0;
int cnt_false = 0;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(FLEX_PIN_T, INPUT);
  pinMode(FLEX_PIN_M, INPUT);
  pinMode(FLEX_PIN_B, INPUT);
  accelgyro.initialize();
}

void loop() {
  /*
   * 자이로 센서 계산식
   */
  accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
  Axyz[0] = (double) ax / 16384;
  Axyz[1] = (double) ay / 16384;
  Axyz[2] = (double) az / 16384;
  int x = Axyz[0]*100;
  int y = Axyz[1]*100;
  int z = Axyz[2]*100;

  int flexADC_T = analogRead(FLEX_PIN_T);
  float flexV_T = flexADC_T * VCC / 1023.0;
  float flexR_T = R_DIV * (VCC / flexV_T - 1.0);
  
  int flexADC_M = analogRead(FLEX_PIN_M);
  float flexV_M = flexADC_M * VCC / 1023.0;
  float flexR_M = R_DIV * (VCC / flexV_M - 1.0);
  
  int flexADC_B = analogRead(FLEX_PIN_B);
  float flexV_B = flexADC_B * VCC / 1023.0;
  float flexR_B = R_DIV * (VCC / flexV_B - 1.0);
  /*
   * 자이로 센서 출력
   */
  Serial.print("X : ");
  Serial.println(x);
  Serial.print("Y : ");
  Serial.println(y);
  Serial.print("Z : ");
  Serial.println(z);
  Serial.println();
  BTSerial.println(x);
  BTSerial.println(y);
  BTSerial.println(z);
  BTSerial.println();
  
  Serial.println("Resistance: " + String(flexR_T) + " ohms");
  Serial.println("Resistance: " + String(flexR_M) + " ohms");
  Serial.println("Resistance: " + String(flexR_B) + " ohms");
  BTSerial.println(String(flexR_T));
  BTSerial.println(String(flexR_M));
  BTSerial.println(String(flexR_B));

  if(flexR_T <= 2100 || flexR_M <= 2100 || flexR_B <= 2100) {
    cnt_true++;
    if(cnt_false > 0) {
      cnt_false--;
    } else if(cnt_false == 0) {
      cnt_false = 0;
    }
  }
  else if(flexR_T > 2100 || flexR_M > 2100 || flexR_B > 2100) {
    cnt_false++;
    if(cnt_true > 0) {
      cnt_true--;
    } else if(cnt_true == 0) {
      cnt_true = 0;
    }
  }
  if(cnt_true > 0 && cnt_false <= 3) {
    Serial.println("true");
  } else {
    Serial.println("False");
  }
  delay(1500);
}
