// Automated formicarium by Ivaylo Georgiev
// TUES 2022

#include <IRremote.h>
#include <Servo.h>
#include <dht.h>
#include <RTClib.h>
#include <LiquidCrystal.h>

//Setting the days of the week
#define SUNDAY    0
#define MONDAY    1
#define TUESDAY   2
#define WEDNESDAY 3
#define THURSDAY  4
#define FRIDAY    5
#define SATURDAY  6

// Event to turn on the lights
// Еvery day from 8:00 to 20:59
// Duration - 12:59 hours
uint8_t LIGHT_EVENT_START_HH = 8;
uint8_t LIGHT_EVENT_END_HH   = 20;

// Feeding event 1
// Every MONDAY from 11:58:00 to 11:58:12
// Duration - 12 seconds
uint8_t FEEDING_EVENT1_DAY      = MONDAY;
uint8_t FEEDING_EVENT1_START_HH = 11;
uint8_t FEEDING_EVENT1_START_MM = 58;
uint8_t FEEDING_EVENT1_END_SS   = 12;

// Feeding event 2
// Every THURSDAY from 11:58:00 to 11:58:12
// Duration - 12 seconds
uint8_t FEEDING_EVENT2_DAY      = THURSDAY;
uint8_t FEEDING_EVENT2_START_HH = 11;
uint8_t FEEDING_EVENT2_START_MM = 58;
uint8_t FEEDING_EVENT2_END_SS   = 12;

dht DHT;

RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {
  "Sunday",
  "Monday",
  "Tuesday",
  "Wednesday",
  "Thursday",
  "Friday",
  "Saturday"
};

#define DHT11_PIN 9

int Contrast = 100;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int receiver = 8;
const int led = 13;
const int pump = 7;
Servo myservo;

IRrecv ir_receiver(receiver);
decode_results results;

unsigned long interval = 6000;
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(9600);
  analogWrite(6, Contrast);
  lcd.begin(16, 2);
  lcd.print( "T = " );
  lcd.setCursor(10, 0);
  lcd.print( "C" );
  lcd.setCursor(0, 2);
  lcd.print( "H = " );
  lcd.setCursor(10, 1);
  lcd.print( "%" );

  ir_receiver.enableIRIn();
  pinMode(led, OUTPUT);
  pinMode(pump, OUTPUT);
  myservo.attach(10);

  // Check if the RTC module works
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Sets the RTC to the date and time of the PC
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

}

void loop() {
  unsigned long currentMillis = millis();

  // Checking for signals from the remote control
  if (ir_receiver.decode(&results)) {
    Serial.println(results.value, HEX);
    translateIRforLED();
    translateIRforPump();
    translateIRforServo();
    ir_receiver.resume();
  }

  digitalWrite(pump, LOW);
  myservo.write(0);

  if ((unsigned long)(currentMillis - previousMillis) >= interval) {
    int chk = DHT.read11(DHT11_PIN);
    float percentage = constrain(DHT.humidity, 0, 99);
    float temperature = constrain(DHT.temperature, -25, 60);
    Serial.print("Humidity = ");
    Serial.println(percentage);

    lcd.setCursor(4, 0);
    lcd.print(temperature);
    lcd.setCursor(4, 1);
    lcd.print(percentage);

    if (percentage < 65) {
      checkDryness();
    }

    DateTime now = rtc.now();

    //Checks if the first event has occurred - lights
    if (now.hour()   >= LIGHT_EVENT_START_HH &&
        now.hour()   <=  LIGHT_EVENT_END_HH) {
      Serial.println("The lights are ON scheduled time");
      Serial.println(" LIGHTS ON ");
      digitalWrite(led, HIGH);
    } else {
      Serial.println("The light are NOT on scheduled time");
      Serial.println(" LIGHTS OFF ");
      digitalWrite(led, LOW);
    }

    // Checks if the second event has occurred - feeding
    if (now.dayOfTheWeek() == FEEDING_EVENT1_DAY      &&
        now.hour()         == FEEDING_EVENT1_START_HH &&
        now.minute()       == FEEDING_EVENT1_START_MM &&
        now.second()       <=  FEEDING_EVENT1_END_SS) {
      Serial.println("It is FEEDING time!");
      Serial.println(" SERVO ON ");
      myservo.write(30);
      delay(100);
      myservo.write(0);
    }

    // Checks if the third event has occurred - feeding
    if (now.dayOfTheWeek() == FEEDING_EVENT2_DAY      &&
        now.hour()         == FEEDING_EVENT2_START_HH &&
        now.minute()       == FEEDING_EVENT2_START_MM &&
        now.second()       <= FEEDING_EVENT2_END_SS) {
      Serial.println("It is FEEDING time!");
      Serial.println(" SERVO ON ");
      myservo.write(30);
      delay(100);
      myservo.write(0);
    }

    printTime(now);
    Serial.println(" ");
    previousMillis = millis();
  }
}

// Remote signal processing function for the LEDs
// Button 1
void translateIRforLED() {
  int sensorValue = 0;
  sensorValue = digitalRead(led);
  switch (results.value) {

    case 0x202807F:
      if (sensorValue == 0) {

        Serial.println(" LIGHTS ON ");
        digitalWrite(led, HIGH);
        break;
      }
      if (sensorValue == 1) {

        Serial.println(" LIGHTS OFF ");
        digitalWrite(led, LOW);
        break;
      }
    default:
      Serial.println(" Not button №1 ");
  }
}

// Remote signal processing function for the pump
// Button 2
void translateIRforPump() {
  switch (results.value) {
    case 0x20240BF:
      Serial.println(" PUMP ON ");
      myservo.write(0);
      digitalWrite(pump, HIGH);
      delay(1000);
      digitalWrite(pump, LOW);
      break;
    default:
      Serial.println(" Not button №2 ");
  }
}

// Remote signal processing function for the servo
// Button 3
void translateIRforServo() {
  switch (results.value) {
    case 0x202C03F:
      Serial.println(" SERVO ON ");
      myservo.write(30);
      delay(100);
      myservo.write(0);
      break;
    default:
      Serial.println(" Not button №3 ");
  }
}

// Temperature and humidity printing function for the serial monitor
void checkParams() {
  int chk2 = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
}

// Function to switch on the pump if the humidity is low
void checkDryness() {
  Serial.println(" Too dry! pump on");
  digitalWrite(pump, HIGH);
  delay(2000);
  digitalWrite(pump, LOW);
}

// Date and time preview function
void printTime(DateTime time) {
  Serial.print("TIME: ");
  Serial.print(time.year(), DEC);
  Serial.print('/');
  Serial.print(time.month(), DEC);
  Serial.print('/');
  Serial.print(time.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[time.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(time.hour(), DEC);
  Serial.print(':');
  Serial.print(time.minute(), DEC);
  Serial.print(':');
  Serial.println(time.second(), DEC);
}
