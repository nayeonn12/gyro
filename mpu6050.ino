#include <Wire.h>
#include <PubSubClient.h>
#include <MPU6050_light.h>
#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "Ruang Waka Atas";
const char* password = "telkomjaya1";

// MQTT broker settings
const char* mqtt_server = "LB-MQTT-d423182774d283b0.elb.ap-southeast-2.amazonaws.com";
const int mqtt_port = 1883;
const char* mqtt_user = "felly"; //input your MQTT Username
const char* mqtt_password = "feliadhi123"; //input your MQTT Password

const char* mqtt_topic = "sensor/mpu6050";

MPU6050 mpu(Wire);
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Handle incoming MQTT messages if needed
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32_DHT22", mqtt_user, mqtt_password)) {
      Serial.println("Connected to MQTT");
    } else {
      Serial.println("Failed to connect to MQTT. Retrying in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Wire.begin();
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  mpu.begin();
  mpu.calcOffsets();
  mpu.setFilterGyroCoef(0.98);
  mpu.setFilterAccCoef(0.02);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  mpu.update();

  // float humidity = mpu.getAccX();
  // float temperature = mpu.getAccX();
  float acx = mpu.getAccX();
  float acy = mpu.getAccY();
  float acz = mpu.getAccZ();
  float gx = mpu.getGyroX();
  float gy = mpu.getGyroY();
  float gz = mpu.getGyroZ();

  // Publish data to MQTT topics
  String payload = "{\"acx\":" + String(acx) + ", \"acv\":" + String(acy) + ", \"acz\":" + String(acz) + ", \"gx\":" + String(gx) + ", \"gy\":" + String(gy) + ", \"gz\":" + String(gz) + "}";
  char msgBuffer[100];
  payload.toCharArray(msgBuffer, 100);

  Serial.print("Publishing message: ");
  Serial.println(msgBuffer);
  client.publish(mqtt_topic, msgBuffer);

  delay(100);
}
