#include "GY-25.h"
GY25 gy25(A0, A1); // RX, TX

#include "motor.h"
#include "IK.h"
#include "hc-sr04.h"

long int t = 0;
#define MOTOR_SPEED 50
Filter filter_ultrasonar_1,filter_ultrasonar_2;

void setup() {
  Serial.begin(9600);
  gy25.setup();
  motors.setup();
  setupIK();

  filter_ultrasonar_1.setup();
  filter_ultrasonar_2.setup(); 
  for (int i = 0; i<FILTER_DATA_SIZE; i++) {
    filter_ultrasonar_1.addData(readUltrasonar(1));
    filter_ultrasonar_2.addData(readUltrasonar(2));
  }
}

long int pid_i = 0;
int e_old = 0;

void loop() {
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
  gy25.update();
  if (t < millis()) {
    gy25.print();
    t = millis() + 100;
  }
}
