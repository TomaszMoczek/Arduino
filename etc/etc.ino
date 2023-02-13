#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <virtuabotixRTC.h>

#define LCD 0x27
#define ROWS 2
#define COLS 16
#define PIN_8 8
#define PIN_9 9
#define PIN_10 10
#define PIN_11 11
#define PIN_12 12
#define PIN_13 13
#define DEFAULT_DELAY 1000
#define BAUD_RATE_9600 9600

/*
 * stty 9600 -F /dev/ttyUSB0 raw -echo
 * cat /dev/ttyUSB0
 * 
 * echo $(date "+%Y,%m,%d,%u,%H,%M,%S") > /dev/ttyUSB0
 */

DHT dht;
LiquidCrystal_I2C lcd(LCD, COLS, ROWS);
virtuabotixRTC virtuabotixRTC(PIN_12, PIN_11, PIN_10);

/*
 * Gets particular item from separated string of characters.
 * 
 * @param data String
 * @param separator char
 * @param index int
 * @return String
 */
String getItem(String data, char separator, int index) {
  int found = 0;
  int indexes[] = { 0, -1 };
  int length = data.length();
  for (int i = 0; i <= length && found <= index; ++i) {
    if (data.charAt(i) == separator || i == length) {
        found++;
        indexes[0] = indexes[1] + 1;
        indexes[1] = (i == length) ? i+1 : i;
    }
  }
  return found > index ? data.substring(indexes[0], indexes[1]) : "";
}

/*
 * Gets formatted form of integer value.
 * 
 * @param value int
 * @return String
 */
String getDigits(int value) {
  String digits;
  if (value < 10) {
    digits.concat("0");
  }
  digits.concat(String(value));
  return digits;
}

/*
 * Program's setup.
 */
void setup() {
  Serial.begin(BAUD_RATE_9600);
  pinMode(PIN_8, OUTPUT);
  pinMode(PIN_13, OUTPUT);
  digitalWrite(PIN_8, HIGH);
  digitalWrite(PIN_13, HIGH);
  dht.setup(PIN_9);
  lcd.init();
  lcd.backlight();
}

/*
 * Program's loop.
 */
void loop() {
  if (Serial.available() != 0) {
    String date = Serial.readStringUntil('\n');
    int year = getItem(date, ',', 0).toInt();
    int month = getItem(date, ',', 1).toInt();
    int dayOfMonth = getItem(date, ',', 2).toInt();
    int dayOfWeek = getItem(date, ',', 3).toInt();
    int hours = getItem(date, ',', 4).toInt();
    int minutes = getItem(date, ',', 5).toInt();
    int seconds = getItem(date, ',', 6).toInt();
    virtuabotixRTC.setDS1302Time(seconds, minutes, hours, dayOfWeek, dayOfMonth, month, year);
  }
  virtuabotixRTC.updateTime();
  String time = getDigits(virtuabotixRTC.dayofmonth) 
                  + "/" + getDigits(virtuabotixRTC.month) 
                  + "/" + String(virtuabotixRTC.year) 
                  + " " + getDigits(virtuabotixRTC.hours) 
                  + ":" + getDigits(virtuabotixRTC.minutes);
  lcd.setCursor(0, 0);
  lcd.print(time);
  int humidity = dht.getHumidity();
  int temperature = dht.getTemperature();
  if (dht.getStatus() == DHT::ERROR_NONE) {
    String values = getDigits(temperature) + String((char)223) + "C"
                      + " "
                      + getDigits(humidity) + "%RH";
    lcd.setCursor(0, 1);
    lcd.print(values);
  }
  delay(DEFAULT_DELAY);
}
