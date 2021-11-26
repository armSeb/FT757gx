#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESPAsyncTCP.h>
// https://randomnerdtutorials.com/esp8266-nodemcu-async-web-server-espasyncwebserver-library/
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include "mainpage.h"
#include "ft757.h"

#ifndef STASSID
#define STASSID "MyISP"
#define STAPSK  ""
#endif

#ifndef APSSID
#define APSSID "FT757gx"
#define APPSK  "87654321" //Change key
#endif

#define APMODE
//#define CAPTIVE

#ifdef APMODE

#ifdef CAPTIVE
DNSServer dnsServer;
const byte DNS_PORT = 53;
#endif
const char *ssid = APSSID;
const char *password = APPSK;
#else
const char *ssid = STASSID;
const char *password = STAPSK;
#endif

FT757 ft757;
AsyncWebServer server(80);
long freq;

// Processor to replace elements in the webpage
String processor(const String& var){
  if (var == "FREQ") {
    if (freq != 0) {
      return String(freq);
    }
  }
  return String();
}

void handleFreq(AsyncWebServerRequest *request) {
     
  if (request->hasParam("freq")) {
    String reqfreq = request->getParam("freq")->value();
    Serial.println(reqfreq);
    long f=strtoul(reqfreq.c_str(), NULL, 10);
    if (ft757.setFreq(f)) {
      freq = f;
      request->send_P(200, "text/plain", "OK");
      return;
    } else {
      request->send_P(400, "text/plain", "Invalid Frequency");
    }
  } else {
    if (freq != 0) {
      request->send_P(200, "text/plain", String(freq).c_str());
    } else {
      request->send_P(200, "text/plain", "");
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Transceiver is on D6
  ft757.begin(4800);
  
#ifdef APMODE
  WiFi.mode(WIFI_AP);
 // WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);
  Serial.println("AP Mode");
#ifdef CAPTIVE
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
  Serial.println("Captive mode enabled");
#endif
#else
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  digitalWrite(LED_BUILTIN, LOW);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
#endif

  if (MDNS.begin("ft757gx")) {
    Serial.println("MDNS responder started");
  }
  MDNS.addService("http", "tcp", 80);

  // Configure web server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", mainPage, processor);
  });
  server.on("/freq", HTTP_GET, handleFreq);
 // server.on("/inc", HTTP_GET, handleInc);
  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", mainPage, processor);
  });  

  server.begin();
  Serial.println("HTTP server started");
  digitalWrite(LED_BUILTIN, HIGH);
  
}

void loop() {
  // put your main code here, to run repeatedly:
#ifdef APMODE
#ifdef CAPTIVE
  dnsServer.processNextRequest();
#endif
#endif
  MDNS.update();
 
}
