#include <NTPClient.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 7200, 60000);

#define LED 0
int dan; int H; int sekunde; int minute;

void setup(){
  pinMode(LED,OUTPUT);
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);
  WiFiManager wm;
 bool res;
 res = wm.autoConnect("Smart Garden","");
  
   if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {  
        Serial.println("connected...yeey :)");
    }
  timeClient.begin();
}

void loop() {
  timeClient.update();

  Serial.println(timeClient.getFormattedTime());
  H = (timeClient.getHours());
  sekunde = (timeClient.getSeconds());
  minute = (timeClient.getMinutes());
  dan = (timeClient.getDay());

  if (H > 22 || H < 8){
       digitalWrite(0,LOW);
       Serial.println("Svjetla OFF");
       Serial.print("H:");
       Serial.println(H);
            }
     else {
        digitalWrite(0,HIGH);
        Serial.println("Svjetla ON");
        Serial.print("H:");
        Serial.println(H);
           }
  Serial.println("---");
  delay(1000);
  }
  
