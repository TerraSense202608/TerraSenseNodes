// =====================================================
// TERRASENSE GATEWAY (OFFLINE)
// LoRa Receiver -> USB Serial -> Electron App
// No WiFi. No Firebase. No internet required.
// =====================================================
//
// This ESP32 only does two jobs:
//   1. Receive LoRa packets from the sensor nodes
//   2. Print them as one JSON line over USB Serial
//
// The Electron desktop app (main.js) reads this Serial
// port and takes care of storing the data (SQLite) and
// pushing it to the dashboard. There is no cloud step
// in between anymore.
//
// -----------------------------------------------------
// NODE 1 (Landslide) PACKET FORMAT EXPECTED OVER LoRa
// (comma separated, 17 fields, sent by the sensor node):
//
//   nodeID,status,tiltX,tiltY,maxTilt,soilRaw,soilWet,
//   rainRaw,rainDetected,hx711Raw,accX,accY,accZ,
//   gyroX,gyroY,gyroZ,temperature
//
// -----------------------------------------------------
// NODE 2 / FIRE01 (Forest Fire) PACKET FORMAT
// (confirmed from the F1_ino.ino transmitter, 6 fields):
//
//   nodeID,status,flame,mq2Raw,temperature,pressure
//
// Example: FIRE01,DANGER,1,2800,52.40,1008.20
//
// There is only one MQ-2 gas/smoke sensor on this node
// (no separate smoke + gas sensors, and no humidity
// sensor - it uses a BMP280 for temperature/pressure).
// We forward mq2Raw as both "smoke" and "gas" so the
// existing dashboard cards for both still populate; feel
// free to repurpose one of those cards for "pressure"
// instead if you'd rather show that.
// =====================================================

#include <SPI.h>
#include <LoRa.h>

// =====================================================
// LORA PINS (unchanged from the original wiring)
// =====================================================

#define LORA_SCK   18
#define LORA_MISO  19
#define LORA_MOSI  23
#define LORA_SS     5
#define LORA_RST   13
#define LORA_DIO0  33

#define LORA_FREQUENCY 433E6

#define LANDSLIDE_FIELDS 17
#define FIRE_FIELDS       6

// =====================================================
// SETUP
// =====================================================

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("========================================");
  Serial.println("       TERRASENSE GATEWAY (OFFLINE)");
  Serial.println("       LORA -> USB SERIAL");
  Serial.println("========================================");

  // ===================================================
  // LORA
  // ===================================================

  Serial.println();
  Serial.println("Starting LoRa...");

  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_SS);

  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(LORA_FREQUENCY))
  {
    Serial.println("LoRa : FAILED");

    while (true)
    {
      delay(1000);
    }
  }

  Serial.println("LoRa : CONNECTED");
  Serial.println("Frequency : 433 MHz");

  Serial.println();
  Serial.println("========================================");
  Serial.println("Waiting for sensor nodes...");
  Serial.println("========================================");
}

// =====================================================
// LOOP
// =====================================================

void loop()
{
  int packetSize = LoRa.parsePacket();

  if (packetSize)
  {
    String data = "";

    while (LoRa.available())
    {
      data += (char)LoRa.read();
    }

    int rssi = LoRa.packetRssi();
    float snr = LoRa.packetSnr();

    handlePacket(data, rssi, snr);
  }
}

// =====================================================
// SPLIT A CSV PACKET INTO UP TO maxFields STRINGS
// Returns the number of fields actually found.
// =====================================================

int splitPacket(const String &data, String values[], int maxFields)
{
  int index = 0;
  int start = 0;

  while (index < maxFields && start <= (int)data.length())
  {
    int comma = data.indexOf(',', start);

    if (comma == -1)
    {
      values[index] = data.substring(start);
      index++;
      break;
    }

    values[index] = data.substring(start, comma);
    index++;
    start = comma + 1;
  }

  return index;
}

// =====================================================
// HANDLE ONE RECEIVED LORA PACKET
// =====================================================

void handlePacket(const String &data, int rssi, float snr)
{
  Serial.println();
  Serial.println("========================================");
  Serial.println("       PACKET RECEIVED");
  Serial.println("========================================");
  Serial.print("RAW  : ");
  Serial.println(data);
  Serial.print("RSSI : ");
  Serial.print(rssi);
  Serial.println(" dBm");
  Serial.print("SNR  : ");
  Serial.print(snr);
  Serial.println(" dB");

  // Try the 17-field landslide format first.
  String values[LANDSLIDE_FIELDS];
  int fieldCount = splitPacket(data, values, LANDSLIDE_FIELDS);

  if (fieldCount == LANDSLIDE_FIELDS)
  {
    printLandslideJson(values, rssi, snr, data);
    return;
  }

  // Otherwise try the 7-field fire format.
  String fireValues[FIRE_FIELDS];
  int fireFieldCount = splitPacket(data, fireValues, FIRE_FIELDS);

  if (fireFieldCount == FIRE_FIELDS)
  {
    printFireJson(fireValues, rssi, snr, data);
    return;
  }

  Serial.print("UNRECOGNISED PACKET - fields found: ");
  Serial.println(fieldCount);
  Serial.println("========================================");
}

// =====================================================
// ESCAPE A STRING FOR SAFE JSON OUTPUT
// =====================================================

String jsonEscape(const String &input)
{
  String out = "";

  for (unsigned int i = 0; i < input.length(); i++)
  {
    char c = input.charAt(i);

    if (c == '"' || c == '\\')
    {
      out += '\\';
      out += c;
    }
    else
    {
      out += c;
    }
  }

  return out;
}

// =====================================================
// BUILD + PRINT JSON FOR A NODE 1 (LANDSLIDE) PACKET
// =====================================================

void printLandslideJson(String values[], int rssi, float snr, const String &raw)
{
  String nodeID        = values[0];
  String status         = values[1];
  float  tiltX          = values[2].toFloat();
  float  tiltY          = values[3].toFloat();
  float  maxTilt        = values[4].toFloat();
  int    soilRaw        = values[5].toInt();
  String soilWet        = values[6];
  int    rainRaw        = values[7].toInt();
  String rainDetected   = values[8];
  long   hx711Raw       = values[9].toInt();
  float  accX           = values[10].toFloat();
  float  accY           = values[11].toFloat();
  float  accZ           = values[12].toFloat();
  float  gyroX          = values[13].toFloat();
  float  gyroY          = values[14].toFloat();
  float  gyroZ          = values[15].toFloat();
  float  temperature    = values[16].toFloat();

  String json = "{";
  json += "\"node_id\":\"" + jsonEscape(nodeID) + "\",";
  json += "\"node_type\":\"landslide\",";
  json += "\"status\":\"" + jsonEscape(status) + "\",";
  json += "\"tiltX\":" + String(tiltX, 2) + ",";
  json += "\"tiltY\":" + String(tiltY, 2) + ",";
  json += "\"maxTilt\":" + String(maxTilt, 2) + ",";
  json += "\"soilRaw\":" + String(soilRaw) + ",";
  json += "\"soilWet\":\"" + jsonEscape(soilWet) + "\",";
  json += "\"rainRaw\":" + String(rainRaw) + ",";
  json += "\"rainDetected\":\"" + jsonEscape(rainDetected) + "\",";
  json += "\"hx711Raw\":" + String(hx711Raw) + ",";
  json += "\"accX\":" + String(accX, 2) + ",";
  json += "\"accY\":" + String(accY, 2) + ",";
  json += "\"accZ\":" + String(accZ, 2) + ",";
  json += "\"gyroX\":" + String(gyroX, 2) + ",";
  json += "\"gyroY\":" + String(gyroY, 2) + ",";
  json += "\"gyroZ\":" + String(gyroZ, 2) + ",";
  json += "\"temperature\":" + String(temperature, 2) + ",";
  json += "\"rssi\":" + String(rssi) + ",";
  json += "\"snr\":" + String(snr, 2) + ",";
  json += "\"online\":true,";
  json += "\"lastPacket\":\"" + jsonEscape(raw) + "\"";
  json += "}";

  // This is the line the Electron app actually parses.
  Serial.println(json);
  Serial.println("========================================");
}

// =====================================================
// BUILD + PRINT JSON FOR A NODE 2 (FOREST FIRE) PACKET
// =====================================================

void printFireJson(String values[], int rssi, float snr, const String &raw)
{
  String nodeID      = values[0];
  String status      = values[1];
  String flame       = values[2];
  int    mq2Raw      = values[3].toInt();
  float  temperature = values[4].toFloat();
  float  pressure    = values[5].toFloat();

  bool flameDetected =
      flame == "1" ||
      flame.equalsIgnoreCase("YES") ||
      flame.equalsIgnoreCase("DETECTED") ||
      flame.equalsIgnoreCase("true");

  String json = "{";
  json += "\"node_id\":\"" + jsonEscape(nodeID) + "\",";
  json += "\"node_type\":\"fire\",";
  json += "\"status\":\"" + jsonEscape(status) + "\",";
  json += "\"flame\":" + String(flameDetected ? "true" : "false") + ",";
  json += "\"mq2Raw\":" + String(mq2Raw) + ",";
  // Only one physical gas sensor (MQ-2) exists on this node - the
  // dashboard has separate "smoke" and "gas" cards, so we feed the
  // same raw reading into both rather than leave one blank.
  json += "\"smoke\":" + String(mq2Raw) + ",";
  json += "\"gas\":" + String(mq2Raw) + ",";
  json += "\"temperature\":" + String(temperature, 2) + ",";
  json += "\"pressure\":" + String(pressure, 2) + ",";
  json += "\"rssi\":" + String(rssi) + ",";
  json += "\"snr\":" + String(snr, 2) + ",";
  json += "\"online\":true,";
  json += "\"lastPacket\":\"" + jsonEscape(raw) + "\"";
  json += "}";

  Serial.println(json);
  Serial.println("========================================");
}
