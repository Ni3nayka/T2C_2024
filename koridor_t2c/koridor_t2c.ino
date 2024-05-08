#include "GY-25.h"
GY25 gy25(A0, A1); // RX, TX

#include "motor.h"
#include "IK.h"
#include "hc-sr04.h"


long int t = 0;
#define MOTOR_SPEED 100
Filter filter_ultrasonar_1,filter_ultrasonar_2;

#define MOTOR_RUN_FLAG 1

// #include <arduino_encoder.h>  // Подключаем библиотеку
// Encoder enc1;                 // первым энкодером
// Encoder enc2;                 // вторым энкодером

void setup() {
  Serial.begin(9600);
  gy25.setup(); // calibration
  motors.setup(MOTOR_RUN_FLAG);
  setupIK();

  // enc1.setup(8,7);            // Энкодер 1 подключен к пинам D5 и D6 (двунаправленный) 
  // enc2.setup(4,2);            // Энкодер 2 подключен к пинам D7 и D8 (двунаправленный) 

  // servo_finger.attach(11);
  // servo_up.attach(10);

  // filter_ultrasonar_1.setup();
  // filter_ultrasonar_2.setup(); 
  // for (int i = 0; i<FILTER_DATA_SIZE; i++) {
  //   filter_ultrasonar_1.addData(readUltrasonar(1));
  //   filter_ultrasonar_2.addData(readUltrasonar(2));
  // }

  // motors.run(50,50); delay(2000);
  // motors.run(0,0); delay(2000);
  // motors.run(-50,-50); delay(2000);
  // motors.run(-50,50); delay(2000);
  // motors.run(50,-50); delay(2000);
  // motors.run(0,0); delay(2000);

  forwardGyro();
  leftGyro(1);
  forwardGyro();
  leftGyro(-1);
  forwardGyro();
  leftGyro(-1);
  forwardGyro1();

}

long int pid_i = 0;
int e_old = 0;

void loop() {
 
}

long int ggg = 0;
long int ggg_cache = 0;
long int ggg_old = 0;

void kolhozGyro() {
  gy25.update();
  int a = gy25.angle[0];
  if (a>100 && ggg_old<-100) ggg_cache -= 360;
  if (a<-100 && ggg_old>100) ggg_cache += 360;
  ggg_old = a;
  ggg = a + ggg_cache;
}

void forwardGyro() {
  // enc1.clear();               // Обнуляем энкодер 1
  // enc2.clear();               // Обнуляем энкодер 2
  filter_ik_2.ik();
  filter_ik_2.ik();
  filter_ik_2.ik();
    kolhozGyro();
  int ik = filter_ik_2.ik();
  long int gy_start = ggg;//gy25.angle[0];
  while (ik>10) {
    ik = filter_ik_2.ik();
    // gy25.update();
    kolhozGyro();
    // int gy = gy25.angle[0]+0;
    // Serial.println(ggg);
    // Serial.println(ik);
    int p = ggg - gy_start;
    // Serial.println(p);
    motors.run(80+p,80-p);
  }

  motors.run(0,0);
}

void forwardGyro1() {
  // enc1.clear();               // Обнуляем энкодер 1
  // enc2.clear();               // Обнуляем энкодер 2
  filter_ik_2.ik();
  filter_ik_2.ik();
  filter_ik_2.ik();
    kolhozGyro();
  int ik = filter_ik_2.ik();
  long int gy_start = ggg;//gy25.angle[0];
  long int t = millis() + 2500;
  while (t>millis()) {
    // ik = filter_ik_2.ik();
    // gy25.update();
    kolhozGyro();
    // int gy = gy25.angle[0]+0;
    // Serial.println(ggg);
    // Serial.println(ik);
    int p = ggg - gy_start;
    // Serial.println(p);
    motors.run(80+p,80-p);
  }
  motors.run(0,0);
}

void leftGyro(int abc) { // 1,-1
  // enc1.clear();               // Обнуляем энкодер 1
  // enc2.clear();               // Обнуляем энкодер 2
  filter_ik_2.ik();
  filter_ik_2.ik();
  filter_ik_2.ik();
    kolhozGyro();
  int ik = filter_ik_2.ik();
  long int gy_start = ggg + abc*90;//gy25.angle[0];
  long int t = millis();
  while (t+700>millis()) {
    // ik = filter_ik_2.ik();
    // gy25.update();
    kolhozGyro();
    // int gy = gy25.angle[0]+0;
    // Serial.println(ggg);
    // Serial.println(ik);
    int p = (ggg - gy_start) * 7;
    // Serial.println(p);
    motors.run(+p,-p);
    if (abs(gy_start-ggg)>5) {
      t = millis();
    }
  }
  motors.run(0,0);
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
