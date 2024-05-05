// #include "GY-25.h"
// GY25 gy25(A0, A1); // RX, TX

#include "motor.h"
#include "IK.h"
#include "hc-sr04.h"

#include <Servo.h>
Servo servo_finger;
Servo servo_up;

long int t = 0;
#define MOTOR_SPEED 100
Filter filter_ultrasonar_1,filter_ultrasonar_2;

#define MOTOR_RUN_FLAG 0


#include <arduino_encoder.h>  // Подключаем библиотеку
Encoder enc1;                 // первым энкодером
// Encoder enc2;                 // вторым энкодером

void setup() {
  Serial.begin(9600);
  // gy25.setup();
  motors.setup(MOTOR_RUN_FLAG);
  setupIK();

  enc1.setup(8,9);            // Энкодер 1 подключен к пинам D5 и D6 (двунаправленный) 
  // enc2.setup(7,6);            // Энкодер 2 подключен к пинам D7 и D8 (двунаправленный) 

  servo_finger.attach(11);
  servo_up.attach(10);

  filter_ultrasonar_1.setup();
  filter_ultrasonar_2.setup(); 
  for (int i = 0; i<FILTER_DATA_SIZE; i++) {
    filter_ultrasonar_1.addData(readUltrasonar(1));
    filter_ultrasonar_2.addData(readUltrasonar(2));
  }

  servo_finger.write(30); // take - 120, drop - 30
  servo_up.write(0); // down - 160, up - 0
  delay(500);
  // servo_finger.detach();
  // servo_up.detach();
  // servoTake();

  forwardGyro(1000);
}

long int pid_i = 0;
int e_old = 0;

void loop() {
  
}

void forwardGyro(long int distance) {
  enc1.clear();               // Обнуляем энкодер 1
  // enc2.clear();               // Обнуляем энкодер 2
  while (1) {
    Serial.println(enc1.get());
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
