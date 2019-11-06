#include "MPU9250.h"
#include <Servo.h>

MPU9250 IMU(Wire,0x68);
Servo myservo;
//MPU9250 IMU(SPI, 10);
int status;
const int servo = 8;
float accx, accy, accz;
unsigned long startmillis;
unsigned long currmillis;
const unsigned long period = 5000;
const int ledPin = 13;
boolean timeStart_init = false;
boolean initStart[11];

int threshold = 1;

const int passwordSize = 4;
int password[passwordSize];

int returnLocationNumber(float accx, float accy, float accz){
  float myXacc[] = {-6.59, -9.24, -8.17, -3.80, 2.16, 7.51, 10.23, 9.12, 4.79, -1.19};
  float myYacc[] = {7.48, 2.01, -3.93, -8.10, -8.95, -6.13, -0.63, 5.42, 9.55, 10.40};
  if(accz <= -8.0){
    return 10;
  }
  else if(abs(accx-myXacc[0])<=threshold && abs(accy-myYacc[0])<=threshold){
    return 0;
  }
  else if(abs(accx-myXacc[1])<=threshold && abs(accy-myYacc[1])<=threshold){
    return 1;
  }
  else if(abs(accx-myXacc[2])<=threshold && abs(accy-myYacc[2])<=threshold){
    return 2;
  }
  else if(abs(accx-myXacc[3])<=threshold && abs(accy-myYacc[3])<=threshold){
    return 3;
  }
  else if(abs(accx-myXacc[4])<=threshold && abs(accy-myYacc[4])<=threshold){
    return 4;
  }
  else if(abs(accx-myXacc[5])<=threshold && abs(accy-myYacc[5])<=threshold){
    return 5;
  }
  else if(abs(accx-myXacc[6])<=threshold && abs(accy-myYacc[6])<=threshold){
    return 6;
  }
  else if(abs(accx-myXacc[7])<=threshold && abs(accy-myYacc[7])<=threshold){
    return 7;
  }
  else if(abs(accx-myXacc[8])<=threshold && abs(accy-myYacc[8])<=threshold){
    return 8;
  }
  else if(abs(accx-myXacc[9])<=threshold && abs(accy-myYacc[9])<=threshold){
    return 9;
  }
  return 99;
}

void baseInitStart(){
  for(int i = 0; i < 11; i++){
    initStart[i] = false;
  }
}

void setup()
{
    Serial.begin(9600);
  while(!Serial) {}

  // start communication with IMU 
  status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while(1) {}
  }
  startmillis = millis();
}

void loop()
{
  IMU.readSensor();
  accx = IMU.getAccelX_mss();
  accy = IMU.getAccelY_mss();
  accz = IMU.getAccelZ_mss();
  //Serial.println(accx);
  //Serial.println(accy);
  //Serial.println(accz);
  int location = returnLocationNumber(accx, accy, accz);
  //Serial.println(location);
  
  if(location == 10){
    startTimer(location);
  }
  else if(location == 0){
    startTimer(location);
  }
  else if(location == 1){
    startTimer(location);
  }
  else if(location == 2){
    startTimer(location);
  }
  else if(location == 3){
    startTimer(location);
  }
  else if(location == 4){
    startTimer(location);
  }
  else if(location == 5){
    startTimer(location);
  }
  else if(location == 6){
    startTimer(location);
  }
  else if(location == 7){
    startTimer(location);
  }
  else if(location == 8){
    startTimer(location);
  }
  else if(location == 9){
    startTimer(location);
  }
  /*
  for(int i = 0; i < 4; i++){
    Serial.print(password[i]);
    Serial.print(" ");
    Serial.println();
  }
  */
/*
  for(int i = 0; i < 11; i++){
    Serial.print(initStart[i]);
    Serial.print(" ");
    Serial.println();
  }*/
}

void startTimer(int location){
  currmillis = millis();
  if(initStart[location] == false){
      baseInitStart();
      initStart[location] = true;
      startmillis = millis();
      //Serial.print(location);
      //Serial.println(" position init start!");
      delay(50);
    } else{
      if(currmillis - startmillis >= period){
        //Serial.println("5 second passed! in position ");
        Serial.println(location);
        initStart[location] = false;
        //add it to the array;
        //delay(1000);
      }
      //Serial.println("inside position");
      //Serial.print(location);
    }
}

void lock(){
  myservo.write(70);
  delay(915);
  myservo.write(90);
}

void unlock(){
  myservo.write(110);
  delay(1000);
  myservo.write(90);
}
/*
 * Serial.print("GyroX: ");
  Serial.print(IMU.getGyroX_rads(),6);
  Serial.print("  ");
  Serial.print("GyroY: ");  
  Serial.print(IMU.getGyroY_rads(),6);
  Serial.print("  ");
  Serial.print("GyroZ: ");  
  Serial.println(IMU.getGyroZ_rads(),6);

  Serial.print("MagX: ");  
  Serial.print(IMU.getMagX_uT(),6);
  Serial.print("  ");  
  Serial.print("MagY: ");
  Serial.print(IMU.getMagY_uT(),6);
  Serial.print("  ");
  Serial.print("MagZ: ");  
  Serial.println(IMU.getMagZ_uT(),6);
  
  Serial.print("Temperature in C: ");
  Serial.println(IMU.getTemperature_C(),6);
  Serial.println();
 */