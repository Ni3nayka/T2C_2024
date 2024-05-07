// #include "GY-25.h"
// GY25 gy25(A0, A1); // RX, TX

#include "motor.h"
#include "IK.h"
#include "hc-sr04.h"

// #include <Servo.h>
// Servo servo_finger;
// Servo servo_up;


#include <using_servo.h>                // подключение библиотеки (принцип работы почти такой-же, как и <servo.h>)
// const int pin = 9;
ServoMotor servo_finger(11);             // инициализация серводвигателя, названного "test_servo", который находится на порте D9
ServoMotor servo_up(10);             // инициализация серводвигателя, названного "test_servo", который находится на порте D9


long int t = 0;
#define MOTOR_SPEED 100
Filter filter_ultrasonar_1,filter_ultrasonar_2;

#define MOTOR_RUN_FLAG 1


#include <arduino_encoder.h>  // Подключаем библиотеку
Encoder enc1;                 // первым энкодером
Encoder enc2;                 // вторым энкодером

void setup() {
  Serial.begin(9600);
  // gy25.setup();
  motors.setup(MOTOR_RUN_FLAG);
  setupIK();

  enc1.setup(8,7);            // Энкодер 1 подключен к пинам D5 и D6 (двунаправленный) 
  enc2.setup(4,2);            // Энкодер 2 подключен к пинам D7 и D8 (двунаправленный) 

  // servo_finger.attach(11);
  // servo_up.attach(10);

  filter_ultrasonar_1.setup();
  filter_ultrasonar_2.setup(); 
  for (int i = 0; i<FILTER_DATA_SIZE; i++) {
    filter_ultrasonar_1.addData(readUltrasonar(1));
    filter_ultrasonar_2.addData(readUltrasonar(2));
  }

  servo_finger.write(30); // take - 120, drop - 30
  servo_up.write(0); // down - 160, up - 0
  for (long int t = millis(); t+500>millis();) ServoUpdate();

  motors.run(50,50); delay(2000);
  motors.run(0,0); delay(2000);
  motors.run(-50,-50); delay(2000);
  motors.run(-50,50); delay(2000);
  motors.run(50,-50); delay(2000);
  motors.run(0,0); delay(2000);

  // servo_finger.detach();
  // servo_up.detach();
  // servoTake();

  // rotationEnc(-360);
}

long int pid_i = 0;
int e_old = 0;

void loop() {
  ServoUpdate();
}

void forwardEnc(long int distance) {
  distance *= 69; // попугаев в сантиметре
  int speed = 90;
  enc1.clear();               // Обнуляем энкодер 1
  enc2.clear();               // Обнуляем энкодер 2
  while (abs(distance-enc1.get())>100) {
    long int p = enc2.get()*1.09-enc1.get();
    int pid = constrain(p*0.1,-200,200);
    motors.run(speed+pid,speed-pid);
    Serial.println(enc2.get());
  }
  motors.run(-50,-50);
  delay(50);
  motors.run(0,0);
}

void rotationEnc(long int angle) {
  angle *= 17.5; // попугаев в градусе
  int speed = 90;
  if (angle<0) {
    // angle *= -1;
    speed *= -1;
  }
  enc1.clear();               // Обнуляем энкодер 1
  enc2.clear();               // Обнуляем энкодер 2
  motors.run(-speed,speed);
  while (abs(angle-enc2.get())>100) {
    // long int p = enc2.get()*1.09-enc1.get();
    // int pid = constrain(p*0.1,-200,200);
    // motors.run(speed+pid,speed-pid);
    Serial.println(enc1.get());
  }
  motors.run(speed,-speed);
  delay(50);
  motors.run(0,0);
}

void forwardGyro(long int distance) {
  enc1.clear();               // Обнуляем энкодер 1
  // enc2.clear();               // Обнуляем энкодер 2
  while (1) {
    Serial.println(enc2.get());
  }
}

void servoTake() {
  int speed = 10;
  servo_finger.write(30); // take - 120, drop - 30
  servo_up.write(0); // down - 160, up - 0
  delay(1000);
  for (int i = 0; i<160; i++) {
    servo_up.write(i);
    delay(speed);
  }
  for (int i = 30; i<120; i++) {
    servo_finger.write(i);
    delay(speed);
  }
  for (int i = 160; i>0; i--) {
    servo_up.write(i);
    delay(speed);
  }
  for (int i = 120; i>30; i--) {
    servo_finger.write(i);
    delay(speed);
  }
}

void koridor() {
  filter_ultrasonar_1.addData(readUltrasonar(1));
  filter_ultrasonar_2.addData(readUltrasonar(2));
  int ik = filter_ik_2.ik();


  // Serial.print(filter_ultrasonar_1.getData());
  // Serial.print(" ");
  // Serial.print(ik);
  // Serial.print(" ");
  // Serial.println(filter_ultrasonar_2.getData());

  if (ik>10) {
    int e = -filter_ultrasonar_1.getData() + filter_ultrasonar_2.getData();
    int pid_p = e;
    pid_i = pid_i*0.95 + e;
    int pid_d = e-e_old;
    e_old = e;
    int pid = pid_p*1 + pid_i*0 + pid_d*0;
    // Serial.println(pid);
    motors.run(MOTOR_SPEED+pid,MOTOR_SPEED-pid);
  } else {
    motors.run(-50,-50);
    delay(500);
    if (pid_i<0) motors.run(50,-50);
    else motors.run(-50,50);
    delay(500);
  }
}

void testMotor() {
  motors.run(100,100); delay(2000);
  motors.run(50,50); delay(2000);
  motors.run(-100,100); delay(2000);
  motors.run(100,-100); delay(2000);
  motors.run(0,0);
}

void testGyro() {
  // gy25.update();
  if (t < millis()) {
    // gy25.print();
    t = millis() + 100;
  }
}
