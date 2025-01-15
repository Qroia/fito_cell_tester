#include <LiquidCrystal_I2C.h>  // подключаем библу
#include <microDS18B20.h>       // Библиотека Dallas Термодатчика
// адрес дисплея 0x3f или 0x27
// размер дисплея 16x2 (поддерживаются и другие, например 20x4)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // адрес, столбцов, строк
MicroDS18B20<2> sensor; // Инцилизация датчика на порте D2
int HIH4000_Pin = 1; // Иницилизация датчика влажности на порте A1

void setup() {
  Serial.begin(9600);
  lcd.init();           // инициализация
  lcd.backlight();      // включить подсветку  
  lcd.setCursor(1, 0);  // столбец 1 строка 0
  lcd.print("t: 0.0");
  lcd.setCursor(1, 1);  // столбец 4 строка 1
  lcd.print("rH: 0.0");
}
void loop() {
  // конструкция программного таймера на 800 мс и 250мс
  static uint32_t tmr_temp;
  static uint32_t tmr_rh;
  String temp;
  String rh;
  // конструкция программного таймера на 800 мс для датчика температуры
  if (millis() - tmr_temp >= 800) {
    tmr_temp = millis();

    sensor.requestTemp(); // запрашиваем новое измерение
    
    // PC Debug Monitor
    if (sensor.readTemp()) Serial.println(sensor.getTemp()); 
    else Serial.println("t - error");

    // LCD Display Monitor
    if (sensor.readTemp()) temp = sensor.getTemp();
    else (temp = "Err");
    
    // Обновляем данные на экране
    lcd.setCursor(1, 0);  // столбец 1 строка 0
    lcd.print("t: " + temp);
  }

  // конструкция программного таймера на 250 мс для датчика влажности
  if (millis() - tmr_rh >= 500) {
    tmr_rh = millis();
    //To properly caculate relative humidity, we need the temperature.
    float relativeHumidity = analogRead(HIH4000_Pin);

    float av=0.0048875*relativeHumidity;

    float rh_res=(av-0.86)/0.03;

    // Обработка данных итоговая
    if (rh_res > -20) rh = String(rh_res);
    else (rh = "err   ");
    
    // PC Debug RH
    Serial.println("rH: " + rh);

    // LCD Monitor
    lcd.setCursor(1, 1);
    lcd.print("rH: " + rh);
  }
}