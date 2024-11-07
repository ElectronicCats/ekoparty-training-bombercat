#include <SPI.h>
#include <WiFiNINA.h>

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("La comunicacion con el modulo WiFi Fallo");
    while (true);
  }

  // Imprime tu direccion MAC
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  printMacAddress(mac);

  // Empieza a buscar redes 
  Serial.println();
  Serial.println("Escanenando redes disponibles...");
  listNetworks();
}

void loop() {
  delay(10000);
  // Vuelve a escanear cada segundo:
  Serial.println("Escanenando redes disponibles...");
  listNetworks();
}

void listNetworks() {
  // Escanea las redes cercanas
  Serial.println("** Escaneo de redes **");
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1)
  {
    Serial.println("Fallo en la comunicacion con el modulo WiFi");
    while (true);
  }

  // imprime la lista de redes disponibles:
  Serial.print("Numero de redes disponibles: ");
  Serial.println(numSsid);

  // Imprime el numero de red que encontramos y sus detalles:
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    Serial.print(thisNet + 1);
    Serial.print(") ");
    Serial.print("Señal: ");
    Serial.print(WiFi.RSSI(thisNet));
    Serial.print(" dBm");
    Serial.print("\tCanal: ");
    Serial.print(WiFi.channel(thisNet));
    byte bssid[6];
    Serial.print("\t\tBSSID: ");
    printMacAddress(WiFi.BSSID(thisNet, bssid));
    Serial.print("\tCifrado: ");
    printEncryptionType(WiFi.encryptionType(thisNet));
    Serial.print("\t\tSSID: ");
    Serial.println(WiFi.SSID(thisNet));
    Serial.flush();
  }
  Serial.println();
}

void printEncryptionType(int thisType) {
  // Lee el tipo de encripcion e imprime el nombre:
  switch (thisType) {
    case ENC_TYPE_WEP:
      Serial.print("WEP");
      break;
    case ENC_TYPE_TKIP:
      Serial.print("WPA");
      break;
    case ENC_TYPE_CCMP:
      Serial.print("WPA2");
      break;
    case ENC_TYPE_NONE:
      Serial.print("None");
      break;
    case ENC_TYPE_AUTO:
      Serial.print("Auto");
      break;
    case ENC_TYPE_UNKNOWN:
    default:
      Serial.print("Unknown");
      break;
  }
}

void print2Digits(byte thisByte) {
  if (thisByte < 0xF) {
    Serial.print("0");
  }
  Serial.print(thisByte, HEX);
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}
