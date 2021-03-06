#include <SoftwareSerial.h>
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU9250.h"
/** 블루투스센서 변수 선언 **/
SoftwareSerial BTSerial(2, 3);
/** 자이로센서 변수 선언 **/
MPU9250 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;
float Axyz[3];
/** 플렉스센서 변수 선언 **/
const int FLEX_PIN_L = A0; 
const int FLEX_PIN_M = A2;
const int FLEX_PIN_R = A1;
/** analog vcc -> register **/
const float VCC = 4.98;
const float R_DIV = 47500.0;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(FLEX_PIN_L, INPUT);
  pinMode(FLEX_PIN_M, INPUT);
  pinMode(FLEX_PIN_R, INPUT);
  accelgyro.initialize();
}

void loop() {
  /** 자이로센서 계산식 **/
  accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
  Axyz[0] = (double) ax / 16384;
  Axyz[1] = (double) ay / 16384;
  Axyz[2] = (double) az / 16384;
  int x = Axyz[0]*100;
  int y = Axyz[1]*100;
  int z = Axyz[2]*100;
  /** 플렉스센서 계산식 **/
  int flexADC_L = analogRead(FLEX_PIN_L);
  float flexV_L = flexADC_L * VCC / 1023.0;
  float flexR_L = R_DIV * (VCC / flexV_T - 1.0);

  int flexADC_M = analogRead(FLEX_PIN_M);
  float flexV_M = flexADC_M * VCC / 1023.0;
  float flexR_M = R_DIV * (VCC / flexV_B - 1.0);
  
  int flexADC_R = analogRead(FLEX_PIN_R);
  float flexV_R = flexADC_R * VCC / 1023.0;
  float flexR_R = R_DIV * (VCC / flexV_R - 1.0);

  float flexR_L_fixed = flexR_L - 700;
  float flexR_M_fixed = flexR_M - 100;
  float flexR_R_fixed = flexR_M - 300;
  /** 자이로센서 출력 **/
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
  /** 플렉스센서 출력 **/
  Serial.println("Resistance: " + String(flexR_L_fixed) + " ohms");
  Serial.println("Resistance: " + String(flexR_M_fixed) + " ohms");  
  Serial.println("Resistance: " + String(flexR_R_fixed) + " ohms");
  BTSerial.println(flexR_L_fixed);
  BTSerial.println(flexR_M_fixed);
  BTSerial.println(flexR_R_fixed);
  delay(1500);
}
