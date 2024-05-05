#pragma once

#define IK_PIN_1 A4
#define IK_PIN_2 A5
#define IK_PIN_3 A6

#define IK_MIN 5
#define IK_MAX 90
#define IK_MIN_BIG 5
#define IK_MAX_BIG 150

int getIK(int port) {
  return constrain(16*pow(analogRead(port)*0.0048828125, -1.10),IK_MIN,IK_MAX);
}

int getIKBig(int port) {
  return constrain(64*pow(analogRead(port)*0.0048828125, -1.10),IK_MIN_BIG,IK_MAX_BIG);
}

#define FILTER_DATA_SIZE 5
class Filter {
  public:
    void setup(int pin = A2) {
      for (int i = 0; i<FILTER_DATA_SIZE; i++) Filter::data[i] = 0;
      Filter::pin = pin;
    }
    void addData(int data) {
      for (int i = 0; i<FILTER_DATA_SIZE-1; i++) Filter::data[i] = Filter::data[i+1];
      Filter::data[FILTER_DATA_SIZE-1] = data;
    }
    int getData() {
      long int answer = 0;
      for (int i = 0; i<FILTER_DATA_SIZE; i++) answer += Filter::data[i];
      return answer/FILTER_DATA_SIZE;
    }
    int addGetData(int data) {
      Filter::addData(data);
      return Filter::getData();
    }
    int ik() {
      // Serial.println(Filter::addGetData(getIK(Filter::pin)));
      // Serial.println(analogRead(Filter::pin));
      return Filter::addGetData(getIK(Filter::pin));
    }
  private:
    int data[FILTER_DATA_SIZE];
    int pin;
};
Filter filter_ik_1, filter_ik_2, filter_ik_3; 

void setupIK(){
  filter_ik_1.setup(IK_PIN_1);
  filter_ik_2.setup(IK_PIN_2);
  filter_ik_3.setup(IK_PIN_3); 
  for (int i = 0; i<FILTER_DATA_SIZE; i++) {
    filter_ik_1.ik();
    filter_ik_2.ik();
    filter_ik_3.ik();
  }
}
