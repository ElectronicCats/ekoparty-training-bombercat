#include <ArduinoJson.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>

const char* ssid = "Bomber";
const char* password = "bomberCat";
const char* mqtt_server = "broker.mqtt.cool";

#define L1 (LED_BUILTIN)  //LED1 indicates activity

WiFiClient espClient;
int status = WL_IDLE_STATUS;

PubSubClient client(espClient);

/*****************
       WIFI
 *****************/
void setup_wifi() {

  // Verifica el modulo de WiFi
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Fallo de comunicacion con modulo WiFi!");
    while (true);
  }

  // Empezamos por conectarnos a la red WiFi predefinida
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  // Intento de conexion a red WiFi
  while (status != WL_CONNECTED) {
    Serial.print("Intento de conexion a SSID: ");
    Serial.println(ssid);
    // Conectar a WiFi 
    status = WiFi.begin(ssid, password);

    Serial.println("");
    Serial.println("WiFi Conectada");
    Serial.println("Direccion IP: ");
    Serial.println(WiFi.localIP());
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


void reconnect() {
  // Ciclo hasta que se conecte a servidor MQTT
  while (!client.connected()) {
    Serial.print("Intentando conexion MQTT ...");
    // Crea un ID random para este cliente
    String clientId = "BomberClient-";
    clientId += String(random(0xffff), HEX);
    // Intento de conexion
    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado");
      // Una vez conectado envia un mensaje
      client.publish("Bomber", "Hola EKO");
      // ... y se suscribe al canal
      client.subscribe("Bomber");
    } else {
      Serial.print("Fallo, rc=");
      Serial.print(client.state());
      Serial.println(" Volvere a intentarlo en 5 segundos");
      // Espera 5 segundos para volver a intentar
      delay(5000);
    }
  }
}

void setup() {

  pinMode(L1, OUTPUT);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  Serial.begin(115200);
  while (!Serial);

  // blink para mostrar inicializacion
  blink(L1, 200, 2);
  Serial.println("Listo ejemplo MQTT");
}

void blink(int pin, int msdelay, int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(msdelay);
    digitalWrite(pin, LOW);
    delay(msdelay);
  }
}

void loop() {
  //Siempre checa status de cliente MQTT
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}
