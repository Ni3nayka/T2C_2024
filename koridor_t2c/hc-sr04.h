#pragma once

#define PIN_HC_SR04_1 13
#define PIN_HC_SR04_2 12

int readUltrasonarBasic(int pin) {
  int duration, cm;

  pinMode(pin, OUTPUT);

  // Сначала генерируем короткий импульс длительностью 2-5 микросекунд.

  digitalWrite(pin, LOW);
  delayMicroseconds(2); // 5
  digitalWrite(pin, HIGH);

  // Выставив высокий уровень сигнала, ждем около 10 микросекунд. В этот момент датчик будет посылать сигналы с частотой 40 КГц.
  delayMicroseconds(10);
  digitalWrite(pin, LOW);

  pinMode(pin, INPUT);
  // delayMicroseconds(10);

  //  Время задержки акустического сигнала на эхолокаторе.
  duration = pulseIn(pin, HIGH);

  // Теперь осталось преобразовать время в расстояние
  cm = (duration / 2) / 29.1;

  // Serial.print("Расстояние до объекта: ");
  // Serial.print(cm);
  // Serial.println(" см.");

  // // Задержка между измерениями для корректной работы скеча
  // delay(250);
  cm = constrain(cm,0,100);
  if (cm<5) cm = 100;
  return cm;
}

int readUltrasonar(int number) {
  if (number==1) return readUltrasonarBasic(PIN_HC_SR04_1);
  if (number==2) return readUltrasonarBasic(PIN_HC_SR04_2);
  return 0;
}
