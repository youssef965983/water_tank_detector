#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);    //0X27 if we write 0X3F It will not work




int trigpin=5;
int echopin=18;
float ss=0.034;
float duration;
float distance;

float Tank_height=150;          // cm
float percentage;  
float Height_of_Water;
// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP32CORE_WIFI_POINT

#include <WiFi.h>

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "ESP32"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377


#include <RemoteXY.h>

// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 67 bytes
  { 255,0,0,22,0,60,0,17,0,0,0,31,1,106,200,1,1,4,0,67,
  15,46,78,30,4,2,26,11,129,28,22,59,20,202,72,101,105,103,104,116,
  0,129,11,107,85,17,64,112,101,114,99,101,110,116,97,103,101,0,67,14,
  131,78,30,4,2,26,11 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // output variables
  char Height[11]; // string UTF8 end zero
  char percentage[11]; // string UTF8 end zero


} RemoteXY;   
#pragma pack(pop)



void setup() 
{
  RemoteXY_Init (); 
  
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);

  Serial.begin(115200);
  pinMode(trigpin,OUTPUT);
  pinMode(echopin,INPUT);
  
}

void loop() 
{ 
   
  RemoteXY_Handler ();

  
  
  digitalWrite(trigpin,LOW);
  delayMicroseconds(2) ;
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(10) ;
  digitalWrite(trigpin,LOW);
  duration = pulseIn(echopin,HIGH);
  distance= duration*ss /2 ;
 
  
  if(distance >=Tank_height) { distance=Tank_height; }
  
  Height_of_Water=Tank_height-distance;
  percentage=Height_of_Water/Tank_height*100;
  
  dtostrf(Height_of_Water,0,1,RemoteXY.Height);
  dtostrf(percentage,0,1,RemoteXY.percentage);
  
  Serial.print("distance= ");
  Serial.println(Height_of_Water);
  Serial.print("percentage= ");
  Serial.println(percentage);

  lcd.print("Height = ");
  lcd.print(Height_of_Water);
  lcd.setCursor(0, 1);
  lcd.print("percentage=");
  lcd.print(percentage);
  lcd.print('%');
  delay(1000);
  lcd.clear();

}
