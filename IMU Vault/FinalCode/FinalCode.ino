#include "MPU9250.h"
#include <Servo.h>
#include <Wire.h>

MPU9250 IMU(Wire, 0x68);
Servo myservo;
//MPU9250 IMU(SPI, 10);
int status;
const int servo = 8;
float accx, accy, accz;
const int ledPin = 13;
int threshold = 1;
int delayTime = 2000;

int correctPassword[] = {0,0,0,0};
int passwordSize = 4;
int password[] = {-1, -1, -1, -1};
int currentPlace = 0;
int newPassword[] = {-1, -1, -1, -1};
int currentPlaceForNewPassword = 0;

boolean passwordChangeMode = false;
boolean locked = true;

int returnLocationNumber(float accx, float accy, float accz){
  float myXacc[] = {-6.59, -9.24, -8.17, -3.80, 2.16, 7.51, 10.23, 9.12, 4.79, -1.19};
  float myYacc[] = {7.48, 2.01, -3.93, -8.10, -8.95, -6.13, -0.63, 5.42, 9.55, 10.40};
  // when facing the sky
  if(accz <= -8.0){
    return 10;
  }
  // when facing the ground
  else if (accz >= 8.0){
    return 11;
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

void setup()
{
  myservo.attach(servo);
  myservo.write(90);
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
}

void loop()
{
  int location = getLocation();
  if(!locked) {
    if(!passwordChangeMode && location==11) {
      // go into password ChangeMode
      toPasswordChangeMode();
    }
  
    else if(passwordChangeMode && location==10) {
      // save new password. If fails, reset written new password
      saveNewPassword();
    }
  
    else if(passwordChangeMode && location >=0 && location <= 9) {
      // fill in new password
      writeNewPassword(location);
    }
    else if(location==0) {
      // lock the door
      lock();
    }
  }
  else if(location>=0 && location<=9) {
    // fill in password
    writePassword(location);
  }
  else if(location==10) {
    // try to unlock the door. If fails, reset written password
    unlock();
  }
  else if(location==11) {
    delay(delayTime);
  }
}

boolean stayedLongEnough(int location){
  for(int i=0; i<100; i++) {
    delay(10);
    int currentLocation = getLocation();
    if(location != currentLocation) {
      return false;
    }
  }
  Serial.print("stayed long enough at ");
  Serial.println(location);
  return true;
}

void toPasswordChangeMode() {
  passwordChangeMode = true;
  for(int i=0; i<passwordSize; i++) {
    newPassword[i] = -1;
  }
  currentPlaceForNewPassword = 0;
  Serial.println("now in password change mode");
}

void saveNewPassword() {
  if(currentPlaceForNewPassword ==4) {
    for(int i=0; i<passwordSize; i++) {
      correctPassword[i] = newPassword[i];
    }
    for(int i=0; i<passwordSize; i++) {
      newPassword[i] = -1;
    }
    currentPlaceForNewPassword = 0;
    passwordChangeMode = false;
    Serial.println("saving the new password and resetting the written new password");
  }
}

void writeNewPassword(int location) {
  if(stayedLongEnough(location) && currentPlaceForNewPassword < 4) {
    newPassword[currentPlaceForNewPassword] = location;
    currentPlaceForNewPassword = currentPlaceForNewPassword + 1;
    Serial.print("writing new password. current length of new password is ");
    Serial.println(currentPlaceForNewPassword);
    delay(delayTime);
  }
}

void lock() {
  if(stayedLongEnough(0)) {
    Serial.println("locking the door");
    lockServo();
    delay(delayTime);
  }
}

void writePassword(int location) {
  if(stayedLongEnough(location) && currentPlace < 4) {
    password[currentPlace] = location;
    currentPlace = currentPlace + 1;
    Serial.print("writing password. current length of written password is ");
    Serial.println(currentPlace);
    delay(delayTime);
  }
}

void unlock() {
  if(stayedLongEnough(10)) {
    //password check and open if correct
    boolean correct = true;
    for (int i = 0; i<passwordSize; i++) {
      if(password[i] != correctPassword[i]) {
        correct = false;
        break;
      }
    }
    if(correct) {
      Serial.println("password correct, unlocking the door and resetting the written password");
      for(int i=0; i<passwordSize; i++) {
        password[i] = -1;
      }
      currentPlace = 0;
      unlockServo();
      delay(delayTime);
    }
    else {
      // password is wrong or incomplete, reset the writen password
      Serial.println("password incorrect, resetting the written password");
      for(int i=0; i<passwordSize; i++) {
        password[i] = -1;
      }
      currentPlace = 0;
      delay(delayTime);
    }
  }
}

void lockServo(){
  myservo.write(70);
  delay(915);
  myservo.write(90);
  locked = true;
}

void unlockServo(){
  myservo.write(110);
  delay(1000);
  myservo.write(90);
  locked = false;
}

int getLocation() {
  IMU.readSensor();
  accx = IMU.getAccelX_mss();
  accy = IMU.getAccelY_mss();
  accz = IMU.getAccelZ_mss();
  return returnLocationNumber(accx, accy, accz);
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
