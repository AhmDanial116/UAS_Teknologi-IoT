#define BLYNK_TEMPLATE_ID "TMPL6DQ_QTyaM"
#define BLYNK_TEMPLATE_NAME "SUHU"
#define BLYNK_AUTH_TOKEN "tAgztYAUqQM4AF_Z8L8O6Jcqu7ZSg_p_"  

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT11.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "opowes";
char pass[] = "12345678";

const int sensorPin = 32;
DHT11 dht11(sensorPin);

// variabel status untuk notifikasi cerdas
String statusSebelumnya = "";

void setup() {
  Serial.begin(115200);
  Serial.println("Mulai sistem...");
  Blynk.begin(auth, ssid, pass);
  Serial.println("Blynk terhubung");
}

void loop() {
  Blynk.run();
  bacaSuhu();
}

void bacaSuhu() {
  int temperature = dht11.readTemperature();
  int humidity = dht11.readHumidity();

  // kirim data sensor
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);

  Serial.print("Suhu: ");
  Serial.print(temperature);
  Serial.println(" °C");

  // =============================
  // PENENTUAN STATUS SUHU
  // =============================
  String status;

  if (temperature < 20) {
    status = "DINGIN";
  } 
  else if (temperature <= 35) {
    status = "NORMAL";
  } 
  else {
    status = "PANAS";
  }

  // kirim label status ke Blynk
  Blynk.virtualWrite(V2, status);

  // =============================
  // NOTIFIKASI CERDAS
  // =============================
  if (status == "PANAS" && statusSebelumnya != "PANAS") {
    Blynk.logEvent("suhu_panas", "Peringatan! Suhu melebihi batas aman");
    Serial.println("Status: ");
    Serial.println(status);
  }

  statusSebelumnya = status;

  delay(1000);
}