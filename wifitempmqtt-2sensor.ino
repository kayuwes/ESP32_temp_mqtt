#include <WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 5
// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Update these with values suitable for your network.
const char* ssid = "xx";
const char* password = "xx";
const char* mqtt_server = "xx";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
float temp = 0;

int inPin = 5; //oneWireBus
int ccount = 0; ///connecttimer for WifiConnectfailuer starts reset

void(* resetFunc) (void) = 0; //declare reset function @ address 0
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);

    Serial.print(ccount);
    ccount = ccount + 1;
    if (ccount >= 10)

    resetFunc();

  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient_temperature_sensor")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  pinMode(inPin, INPUT);
  sensors.begin();
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 6000) {
    lastMsg = now;
    sensors.setResolution(9);
    sensors.requestTemperatures(); // Send the command to get temperatures
 //   Sensor0
    temp = sensors.getTempCByIndex(0);
    Serial.print("1:");
    Serial.println(temp);
    if((temp > -20) && (temp <60))
      {
      client.publish("ha/temp0", String(temp).c_str(),true);
      }

//    Sensor 1
    temp = sensors.getTempCByIndex(1);
    Serial.print("2:");
    Serial.println(temp);
    if((temp > -20) && (temp <60))
      {
      client.publish("ha/temp1", String(temp).c_str(),true);
      }

//    Sensor 2
  }
}
