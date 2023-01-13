#include <BH1750FVI.h>
#include <WiFi.h>
#include <iSYNC.h>
#include <string.h>

WiFiClient client;
iSYNC iSYNC(client);
BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);

String ssid = "Make-PC";
String pass = "makelove123";

String iSYNC_USERNAME = "makelovepppp";
String iSYNC_KEY = "60633db20ae06d53f8c4c08b";
String iSYNC_AUTH = "60633d740ae06d53f8c4c010"; //auth project
int n = 0;


void callback(char* topic, byte* payload, unsigned int length) {
  
  String msg = "";
  Serial.print("[iSYNC]-> ");
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if (msg.startsWith("LINE:"))msg = msg.substring(5);

  /* command control */
  if (msg.equals("เปิดปั้ม")) {
    pinMode(26, OUTPUT);
  
    digitalWrite(26, HIGH);
    iSYNC.mqPub(iSYNC_KEY, "เปิดปั้มแล้วครับ");  //Publish
  } else if (msg.equals("ปิดปั้ม")) {
    digitalWrite(26, LOW);
    pinMode(26, INPUT);
    iSYNC.mqPub(iSYNC_KEY, "ปิดปั้มแล้วครับ");  //Publish
  }
  else if (msg.equals("เปิดระบบลดความร้อน")) {
    pinMode(25, OUTPUT);
    digitalWrite(25, HIGH);
    iSYNC.mqPub(iSYNC_KEY, "เปิดระบบลดความร้อนแล้วครับ");  //Publish
  }
  else if (msg.equals("ปิดระบบลดความร้อน")) {
    digitalWrite(25, LOW);
    pinMode(25, INPUT);
    iSYNC.mqPub(iSYNC_KEY, "ปิดระบบลดความร้อนแล้วครับ");  //Publish
  }
  else if (msg.equals("เปิดแสง")) {
    pinMode(27, OUTPUT);
    digitalWrite(27, HIGH);
    n=1;
    iSYNC.mqPub(iSYNC_KEY, "เปิดไฟแล้วครับ");  //Publish
  }
  else if (msg.equals("ปิดแสง")) {
    digitalWrite(27, LOW);
    pinMode(27, INPUT);
    n=0;
    iSYNC.mqPub(iSYNC_KEY, "ปิดไฟแล้วครับ");  //Publish
  }
  else if (msg.equals("เปิดระบบออโต้")) {
    pinMode(26, OUTPUT);
    delay(100);
    digitalWrite(26, HIGH);
    iSYNC.mqPub(iSYNC_KEY, "เปิดระบบออโต้แล้วครับ");  //Publish 
  }
  else if (msg.equals("ปิดระบบออโต้")) {
    delay(100);
    digitalWrite(26, LOW);
    pinMode(26, INPUT);
  pinMode(27, INPUT);
  pinMode(25, INPUT);
    iSYNC.mqPub(iSYNC_KEY, "ปิดระบบออโต้แล้วครับ");  //Publish
  }
  else if (msg.equals("เปิดระบบควบคุมด้วยมือ")) {
    
    digitalWrite(27, LOW);
    delay(100);
    digitalWrite(25, LOW);
    delay(100);
    digitalWrite(26, LOW);
    pinMode(26, INPUT);
  pinMode(27, INPUT);
  pinMode(25, INPUT);
    iSYNC.mqPub(iSYNC_KEY, "เปิดระบบควบคุมด้วยมือแล้วครับ");  //Publish
  }
  else if (msg.equals("ปิดระบบควบคุมด้วยมือ")) {
    digitalWrite(27, LOW);
    delay(100);
    digitalWrite(25, LOW);
    delay(100);
    digitalWrite(26, LOW);
    pinMode(26, INPUT);
  pinMode(27, INPUT);
  pinMode(25, INPUT);
    iSYNC.mqPub(iSYNC_KEY, "ปิดระบบควบคุมด้วยมือแล้วครับ");  //Publish
  }
  else if (msg.equals("ความเข้มแสงปัจจุบัน")) {
    int lux = LightSensor.GetLightIntensity();
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lux");
    iSYNC.mqPub(iSYNC_KEY, "ความเข้มแสง: "+String(lux)+" lux");  //Publish
  }
  else if (msg.equals("HELP")) {
    iSYNC.mqPub(iSYNC_KEY, "คำสั่งต่างๆ https://drive.google.com/file/d/1m6flGdFaMC_dGO8Ko4wKHx5iXDA_OvSo/view?usp=sharing");
 
  }
  else if (msg.equals("help")) {
   iSYNC.mqPub(iSYNC_KEY, "คำสั่งต่างๆ https://drive.google.com/file/d/1m6flGdFaMC_dGO8Ko4wKHx5iXDA_OvSo/view?usp=sharing");
   
  }
  else if (msg.equals("คำสั่งต่างๆ")) {
    iSYNC.mqPub(iSYNC_KEY, "คำสั่งต่างๆ https://drive.google.com/file/d/1m6flGdFaMC_dGO8Ko4wKHx5iXDA_OvSo/view?usp=sharing");
}
else if (msg.equals("ช่วยเหลือ")) {
    iSYNC.mqPub(iSYNC_KEY, "คำสั่งต่างๆ https://drive.google.com/file/d/1m6flGdFaMC_dGO8Ko4wKHx5iXDA_OvSo/view?usp=sharing");
}
}
void connectMQTT() {
  while (!iSYNC.mqConnect()) {
    Serial.println("Reconnect MQTT...");
    delay(3000);
  }
  iSYNC.mqPub(iSYNC_KEY, "พร้อมรับคำสั่งแล้วค่ะเจ้านาย");  //Publish on Connect
  // iSYNC.mqSubProject(); //subscribe all key in your project
  iSYNC.mqSub(iSYNC_KEY); //subscribe key
}

void setup()
{
  
  Serial.begin(9600);
  LightSensor.begin();
  Serial.println(iSYNC.getVersion());

  pinMode(26, INPUT);
  pinMode(27, INPUT);
  pinMode(25, INPUT);
  
  iSYNC.begin(ssid,pass);
  iSYNC.mqInit(iSYNC_USERNAME,iSYNC_AUTH);   
  iSYNC.MQTT->setCallback(callback);
  connectMQTT();
}

void loop()
{ 
  if (!iSYNC.mqConnected())connectMQTT();
  iSYNC.mqLoop();
  
  if(n==0){
    uint16_t lux = LightSensor.GetLightIntensity();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lux");
    if(lux<=140)
      {
        pinMode(27, OUTPUT);
        digitalWrite(27, HIGH);
        delay(1000); 
      }
    else{
      digitalWrite(27, LOW);
      delay(1000);
      pinMode(27, INPUT);
      }
  }
  else{
       uint16_t lux = LightSensor.GetLightIntensity();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lux");
  }
}
