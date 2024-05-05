#pragma once

class Motor {
  public:
    void setup(int pin1, int pin2) {
      Motor::pin1 = pin1;
      Motor::pin2 = pin2;
      pinMode(Motor::pin1,OUTPUT);
      pinMode(Motor::pin2,OUTPUT);
      //reverse = 1;
    }
    void run(int speed=0) {
      speed = constrain(speed,-100,100)*2.5;
      analogWrite(Motor::pin1, speed>0?speed:0);
      analogWrite(Motor::pin2, speed>0?0:-speed);
    }
  private:
    int pin1, pin2;
};

class Motors {
  public:
    Motor motor_left,motor_right;
    void setup() {
      Motors::motor_right.setup(6,9);
      Motors::motor_left.setup(3,5);
    }
    void run(int l=0, int r=0) {
      Motors::motor_left.run(l);
      Motors::motor_right.run(r);
    }
  // private:
};

Motors motors;

