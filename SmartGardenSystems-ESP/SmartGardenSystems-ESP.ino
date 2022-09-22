#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define LED 0
#define INIT_ON 8
#define INIT_OFF 22

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 7200, 60000);
ESP8266WebServer server(80);

int day, H, minutes, seconds;
int userLedOn = INIT_ON, userLedOff = INIT_OFF;


void handleRoot() {
  String root_ret = "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><title>Smart Garden ESP</title>";
  root_ret += "</head><body><h1>Smart Garden ESP</h1><form action=\"\" method=\"GET\">";
  root_ret += "<label for=\"on\">ON time:</label> <br>";
  root_ret += "<input type=\"text\" id=\"on\" name=\"on\"><br><br>";
  root_ret += "<label for=\"off\">OFF time:</label> <br>";
  root_ret += "<input type=\"text\" id=\"off\" name=\"off\"><br><br>";
  root_ret += "</form><br><br><br><br>";

  if (server.args() == 2) {
    userLedOn = server.arg(0).toInt();
    userLedOff = server.arg(1).toInt();
    root_ret += "<h1>ON and OFF times set!</h1>";
  }
  root_ret += "</body></html>";

  server.send(200, "text/html", root_ret);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}


void setup() {
  pinMode(LED, OUTPUT);
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);
  WiFiManager wm;
  bool res;

  res = wm.autoConnect("SmartGarden");

  if (!res) {
    Serial.println("Can't connect to Wi-Fi");
  }
  else {
    Serial.println("Connected to Wi-Fi!)");
  }


  timeClient.begin();

  if (MDNS.begin("smartgarden")) {
    Serial.println("MDNS responder started");
  }


  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  timeClient.update();

  Serial.println(timeClient.getFormattedTime());
  H = (timeClient.getHours());
  seconds = (timeClient.getSeconds());
  minutes = (timeClient.getMinutes());
  day = (timeClient.getDay());

  if (H <= userLedOff && H >= userLedOn) {
    digitalWrite(LED, 1);
    Serial.println("Lights ON");
    Serial.print("H: ");
    Serial.println(H);
  }
  else {
    digitalWrite(LED, 0);
    Serial.println("Lights OFF");
    Serial.print("H: ");
    Serial.println(H);
  }

  server.handleClient();
  delay(1000);
}
