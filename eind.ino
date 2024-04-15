#define BLYNK_TEMPLATE_ID "user20"
#define BLYNK_TEMPLATE_NAME "user20@server.wyns.it"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;
BlynkTimer timer;
WidgetLED led1(V6);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "QddllR7eEuSt7dBJjC6xwHAccHH_4frN";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "telenet-B115D97";
char pass[] = "wkd3ucpeXfch";

#define BMP_SCK (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS (10)
#define led 18

int gewensteWaarde;
int pot2 = 0.0;
int ap1 = 0.0;
int ap2 = 0.0;
int POT1 = 0.0;
float Val = 0;
float pot = 34;


void sendSensor() {
  float t = bmp.readTemperature();
  float tt = int(t * 10) / 10.0;
  Blynk.virtualWrite(V5, tt);
}

#define gewensteWaarde V7

BLYNK_WRITE(gewensteWaarde) {
  int gt = param.asInt();
  Val = gt;
  Serial.print("Gewenste waarde ingesteld op: ");
  Serial.println(gt);
}
void setup() {
  Serial.begin(115200);
  Serial.println(F("BMP280 test"));

  pinMode(led, OUTPUT);

  if (!bmp.begin(0x76)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or try a different address!"));
    while (1)
      ;
  }

  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");

  Blynk.begin(auth, ssid, pass, "server.wyns.it", 8081);

// Additional lines to define the Blynk template ID and name
#define BLYNK_TEMPLATE_ID "user20"
#define BLYNK_TEMPLATE_NAME "user20@server.wyns.it"

  timer.setInterval(1000L, sendSensor);
}


void loop() {
  Blynk.run();
  timer.run();
    float t = bmp.readTemperature();

  float potvalue = analogRead(pot);
  POT1 = map(potvalue, 0, 4095, 0, 30);
  if (POT1 != pot2) {
    pot2 = POT1;
    Blynk.virtualWrite(V7, pot2);
    t = pot2;
  }
  if (ap2 != ap1) {
    ap2 = ap1;
    Blynk.virtualWrite(V7, ap2);
        t = ap2;

  }

  if (t <= Val) {
    digitalWrite(led, HIGH);
    led1.on();
  }

  else {
    digitalWrite(led, LOW);
    led1.off();
  }



  Serial.print(F("Temperature = "));
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");
  Serial.println();

delay(1000);
}
BLYNK_WRITE(V6) {
  float value = param.asFloat();
  Serial.print("float value received from Blynk app: ");
  Serial.print(value);
}
