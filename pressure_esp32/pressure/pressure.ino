#include <WiFi.h>
#include <PubSubClient.h>
#include "QuickSort.h"
#include "configuration.h"

WiFiClient espClient;
PubSubClient pubClient(espClient);

void setup() {
  Serial.begin(115200);
  setupWifi();
  setupMqttClient();
  setupSensor();
}

void loop() {
  char pressure[6];
  char topic[40];

  if (!pubClient.connected()) {
    connectMqttClient();
  }

  Serial.print("Reading pressure... ");
  readPressure(pressure);
  Serial.println(pressure);

  strcpy(topic, TOPIC);
  strcat(topic, CLIENT_ID);
  strcat(topic, "/status");
  Serial.print("Publishing value on topic: ");
  Serial.println(topic);

  pubClient.publish(topic, pressure);

  Serial.print("Sleep for ");
  Serial.print(SLEEP_SECONDS);
  Serial.println("s");
  deepSleep();
}

void setupWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
}

void setupMqttClient() {
  pubClient.setServer(BROKER_IP, 1883);
}

void setupSensor() {
  pinMode(PRESSURE_PIN, INPUT);
  analogReadResolution(12);
}

void connectMqttClient() {
  while(!pubClient.connected()) {
    Serial.println("Connecting to MQTT broker");

    if(pubClient.connect(CLIENT_ID)) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.println("Failed, reconnecting...");
    }
  }
}

void readPressure(char* pressureStr) {
  int voltage = readMedianVoltage(WINDOW_SIZE);
  float pressure = float(voltage) / BAR;
  dtostrf(pressure, 3, 2, pressureStr);
}

int readMedianVoltage(int windowSize) {
  int count, mid, medianVoltage;
  int window[windowSize];

  for(count = 0; count < windowSize; ++count) {
    window[count] = int(analogRead(PRESSURE_PIN));
  }

  quickSort(window, 0, windowSize - 1);

  if(windowSize % 2 == 0) {
    mid = windowSize / 2;
    medianVoltage = (window[mid] + window[mid + 1]) / 2;
  } else {
    mid = windowSize / 2;
    medianVoltage = window[mid];
  }

  return medianVoltage;
}

void beforeSleep() {
  pubClient.disconnect();
  Serial.flush();
}

void deepSleep() {
  beforeSleep();

  esp_sleep_enable_timer_wakeup(SLEEP_SECONDS * 1000000);
  esp_deep_sleep_start();
}
