#include "application.h"
#line 1 "/Users/Tim/github/Particle_FeinstaubGauge/src/Particle_FeinstaubGauge.ino"
/*
 * Project Particle_FeinstaubGauge
 * Description:
 * Author:
 * Date:
 */

//Initializing LED Pin
void setup();
void loop();
void PM10Handler_K16(const char *event, const char *data);
void PM10Handler_F43(const char *event, const char *data);
void AlarmHandler(const char *event, const char *data);
void triggerPMReadings();
int setdac(String command);
#line 9 "/Users/Tim/github/Particle_FeinstaubGauge/src/Particle_FeinstaubGauge.ino"
int led_pin = D7;
int LED_FeinstaubAlarm = D1;
int pwm_pin = D2;
int pm10Switch = D0;

String PM10_K16data = String(10);
String PM10_F43data = String(10);
String PM10_K16 = "";
String PM10_F43 = "";
String FeinstaubAlarmStatus = String(10);

bool swing = false;
bool feinstaubAlarm = false;

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.

  // Start SW-timers
  //timer.start();

  // Establish serial connection
  Serial.begin();

  // Declaring I/Os
  pinMode(pwm_pin, OUTPUT);
  pinMode(led_pin, OUTPUT);
  pinMode(LED_FeinstaubAlarm, OUTPUT);
  pinMode(pm10Switch, INPUT_PULLDOWN);

  //Particle.function("setgauge",setdac);

  // Subscribe to the integration response event
  Particle.subscribe("hook-response/PM10_K16", PM10Handler_K16, MY_DEVICES);
  Particle.subscribe("hook-response/PM10_F43", PM10Handler_F43, MY_DEVICES);
  Particle.subscribe("hook-response/Feinstaubalarm_Stuttgart", AlarmHandler, MY_DEVICES);

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.

  triggerPMReadings();
  delay(10000);
  setdac(PM10_K16);
  delay(50000);

}

void PM10Handler_K16(const char *event, const char *data)
{

  PM10_K16 = String(data);

  Serial.print("PM10 (K16): ");
  Serial.println(PM10_K16);

  // Handle the integration response

  /*
  int commandint = latest.toInt();
    if ((commandint >=0) && (commandint <=100)){
        commandint = map(commandint, 0, 100, 0, 255); //in low, high to out low, high
        analogWrite(pwm_pin, commandint, 500); 
    }
  */

  //setdac(latest);

  // notify user about change
  digitalWrite(led_pin, HIGH);
  delay(500);
  digitalWrite(led_pin, LOW);
}

void PM10Handler_F43(const char *event, const char *data)
{

  PM10_F43 = String(data);

  Serial.print("PM10 (F43): ");
  Serial.println(PM10_F43);

  // Handle the integration response

  /*
  int commandint = latest.toInt();
    if ((commandint >=0) && (commandint <=100)){
        commandint = map(commandint, 0, 100, 0, 255); //in low, high to out low, high
        analogWrite(pwm_pin, commandint, 500); 
    }
  */

  //setdac(latest);

  // notify user about change
  digitalWrite(led_pin, HIGH);
  delay(500);
  digitalWrite(led_pin, LOW);
}

void AlarmHandler(const char *event, const char *data)
{

  String latest = String(data);

  if (latest == "true")
  {

    feinstaubAlarm = true;
    Serial.printf("Feinstaubalarm: %s\n", feinstaubAlarm ? "true" : "false");
    digitalWrite(LED_FeinstaubAlarm, HIGH);
  }
  else if (latest == "false")
  {

    feinstaubAlarm = false;
    Serial.printf("Feinstaubalarm: %s\n", feinstaubAlarm ? "true" : "false");
    digitalWrite(LED_FeinstaubAlarm, LOW);
  }
  else
  {

    Serial.println("Feinstaubalarm: Error!");
  }
}

void triggerPMReadings()
{

  // Trigger interaction
  Particle.publish("PM10_K16", PM10_K16data, PRIVATE);
  Particle.publish("PM10_F43", PM10_F43data, PRIVATE);
  Particle.publish("Feinstaubalarm_Stuttgart", FeinstaubAlarmStatus, PRIVATE);
}

int setdac(String command)
{

  // Handle the integration response
  int commandint = command.toInt();
  if ((commandint >= 0) && (commandint <= 100))
  {
    commandint = map(commandint, 0, 100, 0, 255); //in low, high to out low, high
    analogWrite(pwm_pin, commandint, 500);
  }
}