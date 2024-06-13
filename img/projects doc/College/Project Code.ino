#define BLYNK_TEMPLATE_ID "TMPL3cP6rsDjy"
#define BLYNK_TEMPLATE_NAME "Water level monitoring"
#define BLYNK_AUTH_TOKEN "9Tv1ypxzBYY_ReJNgFHkZFPBbBNCxrHY"
#include<WiFi.h>
#include<WiFiClient.h>
#include<BlynkSimpleEsp32.h>

#define trigPin 12
#define echoPin 14
#define trigPin2 4
#define echoPin2 2
#define SOUND_SPEED 0.034
#define motor 27
#define motor2 18
#define tankDepth 400
#define upto85 60

char auth[]= BLYNK_AUTH_TOKEN;
char ssid[] = "404 error not found";
char pass[] = "vadapav88";

float distanceCm2;
long duration2;
long duration;
int pinValue = 1;
int pinValue2 = 1;
float distanceCm;
unsigned int level=0;
unsigned int level2=0;

BLYNK_WRITE(V0){
  pinValue = param.asInt();
}

BLYNK_WRITE(V4){
  pinValue2 = param.asInt();
}

void setup() {
  SSerial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(motor, OUTPUT);
  pinMode(motor2, OUTPUT);
  Serial.print("connecting to wifi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }
  Serial.println("CONNECTED");
  Blynk.begin(auth,"","");
}

void loop() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);

  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  int level = 100 - ((distanceCm / tankDepth) * 100);
  Blynk.virtualWrite(V1, level);
  Blynk.virtualWrite(V2, distanceCm);

  distanceCm2 = duration2 * SOUND_SPEED/2;
  int level2 = 100 - ((distanceCm2 / tankDepth) * 100);
  Blynk.virtualWrite(V3, level2);
  if(distanceCm >= upto85 && pinValue == 1)
  {
    digitalWrite(motor, LOW);
    Blynk.virtualWrite(V0, pinValue);
  }
  else{
    digitalWrite(motor, HIGH);
    Blynk.virtualWrite(V0, 0);
  }

  if(distanceCm2 >= upto85 && pinValue2 == 1)
  {
    digitalWrite(motor2, LOW);
    Blynk.virtualWrite(V4, pinValue2);
  }
  else{
    digitalWrite(motor2, HIGH);
    Blynk.virtualWrite(V4, 0);
  }
  if(level >= 85){
    Blynk.logEvent("level","Tank level is 85%");
  }
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.println(distanceCm2);
  Blynk.run();
}