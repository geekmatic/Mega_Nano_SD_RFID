#include <SPI.h>
#include <SD.h>
#include "DS3231.h"
#include <Wire.h>
//#include <TimerOne.h>

RTClib RTC;
DateTime now;

File myFile;

#define OK 1
#define NOTOK 2
#define TIMEOUT 3
#define SIM808board Serial1
char end_c[2];
int dosend=0, dosendgps=0, dosendbalans=0;
int a2, kategor;
float balans=0.0;
String str, str1;
String signal_level_str;
float longitude, latitude;
unsigned long long prev_ms_balans, prev_ms_gps, prev_ms_gprs;

//byfer otchetov skanirovaniy kartochek
const int bufsize = 10;
byte okod[bufsize][4];
unsigned long otime[bufsize];

void setup() {
  Serial.begin(9600);
//  pinMode(7, INPUT_PULLUP);
  SIM808board.begin(9600);
  if (!SD.begin(53)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  
  Wire.begin(15);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  now = RTC.now();
//  Serial.println("S E T U P");
  delay(1000);
  SIM808board.println("AT");
  delay(100);
  SIM808board.println("AT");
  delay(100);
  SIM808board.println("AT");
  delay(100);

  A6command("AT+CGPSPWR=1", "OK", "ERROR", 5000, 1);
  


    A6command("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", "OK", "ERROR", 2000, 1);
    A6command("AT+SAPBR=3,1,\"APN\",\"internet\"", "OK", "ERROR", 2000, 1);///////////////www.ab.kyivstar.net

//    A6command("AT+SAPBR=3,1,\"USER\",\"\"", "OK", "ERROR", 2000, 1);
//    A6command("AT+SAPBR=3,1,\"PWD\",\"\"", "OK", "ERROR", 2000, 1);
    A6command("AT+SAPBR=1,1", "+SAPBR:", "ERROR", 2000, 1); //start up the connection
    A6command("AT+HTTPINIT", "OK", "ERROR", 5000, 1);

    prev_ms_gps = millis();
    prev_ms_balans = millis();
    prev_ms_gprs = millis();
//  attachInterrupt(digitalPinToInterrupt(7), func_start, FALLING);

  
  
  //myFile = SD.open("a4e12b83", FILE_WRITE);
  //myFile.close();
//  rfidtimer();
  
}

void loop() {
    now = RTC.now();
    
    func_balans();
    now = RTC.now();
    func_gps(); 
    now = RTC.now();
    func_gprs();
    now = RTC.now();
    //A6command("AT+SAPBR=4,1", "OK", "ERROR", 2000, 1);
   // if (jeka==1) digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    //A6command("AT+HTTPINIT=?", "OK", "ERROR", 5000, 1);
    delay(50);
}

