/*
#include <Servo.h> 
Servo myservo;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
 
void setup() { 
  myservo.attach(2);  // IO2 D4 
} 
 
void loop() {
  delay(5000);
  myservo.write(0);
  delay(500);
  myservo.write(30);
} 
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h> 
#include <stdlib.h>

const char* ssid = "xxx";
const char* password = "xxx";
Servo myservo;
ESP8266WebServer server(80);

const int led = 14;       // ONBOARD 14
const int buttonPin = 0;  // 

void servo() {
  Serial.println("servo");
  digitalWrite(led, 1);
  myservo.write(20);
  delay(200);
  myservo.write(0);
  digitalWrite(led, 0);
}

void handleRoot() {
  digitalWrite(led, 1);
  String strCommand = server.argName(0);
  String strValue = server.arg(0);
  int val = atoi(strValue.c_str());
  servo();
//  Serial.println(strCommand);
  Serial.print(millis());
  Serial.print(" ");
  Serial.println(val);
  server.send(200, "text/plain", strValue);
}

void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  delay(200);
  digitalWrite(led, 0);
}

void setup(void){
  myservo.attach(2);  // IO2 D4
  myservo.write(0);
  pinMode(led, OUTPUT);
  pinMode(buttonPin, INPUT);

  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

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

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
  int buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    servo();
  }
/*  int a = analogRead(0);
  if (a > 1000) {
    digitalWrite(led, 1);
  }
  Serial.println(a);
  delay(100);
  digitalWrite(led, 0);*/
}


