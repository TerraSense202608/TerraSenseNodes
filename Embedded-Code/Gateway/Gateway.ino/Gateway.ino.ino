// =====================================================
// TERRASENSE GATEWAY (OFFLINE)
// LoRa Receiver -> USB Serial -> Electron App / Logger
// No WiFi. No Firebase. No internet required.
// =====================================================
//
// Packet formats accepted over LoRa (comma separated):
//
// LANDSLIDE (17 fields, or 18 with a trailing packet counter)
//   nodeID,status,tiltX,tiltY,maxTilt,soilRaw,soilWet,
//   rainRaw,rainDetected,hx711Raw,accX,accY,accZ,
//   gyroX,gyroY,gyroZ,temperature[,counter]
//
// FIRE (6 fields, or 7 with a trailing packet counter)
//   nodeID,status,flame,mq2Raw,temperature,pressure[,counter]
//
// Packets without a counter are logged with counter = -1.
//
// Serial output (115200 baud):
//   - One JSON line per good packet (parsed by Electron / logger)
//   - One "BAD," line per packet that could not be parsed
//   - Banner text, only if VERBOSE is 1
// =====================================================

#include <SPI.h>
#include <LoRa.h>

// ---------- LoRa pins ----------
#define LORA_SCK   18
#define LORA_MISO  19
#define LORA_MOSI  23
#define LORA_SS     5
#define LORA_RST   13
#define LORA_DIO0  33

#define LORA_FREQUENCY 433E6

// ---------- Packet field counts ----------
#define LANDSLIDE_FIELDS       17
#define LANDSLIDE_FIELDS_CTR   18
#define FIRE_FIELDS             6
#define FIRE_FIELDS_CTR         7
#define MAX_FIELDS             24   // safe upper limit for splitting

// ---------- Logging ----------
// 1 = print the readable banner for every packet (good for debugging)
// 0 = print only the JSON line (cleaner and lighter for long runs)
#define VERBOSE 0

// =====================================================
// SETUP
// =====================================================

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("TERRASENSE GATEWAY (OFFLINE) - LORA -> USB SERIAL");

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
  // Record the other radio settings (SF, bandwidth, coding rate,
  // TX power) from your node code in your test notes. They must
  // match on the node and the gateway.
  Serial.println("Waiting for sensor nodes...");
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

    data.trim();   // remove stray \r or \n

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
#if VERBOSE
  Serial.println();
  Serial.println("========================================");
  Serial.println("       PACKET RECEIVED");
  Serial.print("RAW  : ");
  Serial.println(data);
  Serial.print("RSSI : ");
  Serial.print(rssi);
  Serial.println(" dBm");
  Serial.print("SNR  : ");
  Serial.print(snr);
  Serial.println(" dB");
#endif

  String values[MAX_FIELDS];
  int fieldCount = splitPacket(data, values, MAX_FIELDS);

  // ---------- Landslide ----------
  if (fieldCount == LANDSLIDE_FIELDS)
  {
    printLandslideJson(values, -1, rssi, snr, data);
    return;
  }

  if (fieldCount == LANDSLIDE_FIELDS_CTR)
  {
    long counter = values[LANDSLIDE_FIELDS].toInt();
    printLandslideJson(values, counter, rssi, snr, data);
    return;
  }

  // ---------- Fire ----------
  if (fieldCount == FIRE_FIELDS)
  {
    printFireJson(values, -1, rssi, snr, data);
    return;
  }

  if (fieldCount == FIRE_FIELDS_CTR)
  {
    long counter = values[FIRE_FIELDS].toInt();
    printFireJson(values, counter, rssi, snr, data);
    return;
  }

  // ---------- Unrecognised ----------
  // Logged so corrupted packets are counted in the test results.
  Serial.print("BAD,");
  Serial.print(millis());
  Serial.print(",");
  Serial.print(fieldCount);
  Serial.print(",");
  Serial.print(rssi);
  Serial.print(",");
  Serial.println(snr, 2);
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
// BUILD + PRINT JSON FOR A LANDSLIDE PACKET
// =====================================================

void printLandslideJson(String values[], long counter, int rssi, float snr, const String &raw)
{
  String nodeID        = values[0];
  String status        = values[1];
  float  tiltX         = values[2].toFloat();
  float  tiltY         = values[3].toFloat();
  float  maxTilt       = values[4].toFloat();
  int    soilRaw       = values[5].toInt();
  String soilWet       = values[6];
  int    rainRaw       = values[7].toInt();
  String rainDetected  = values[8];
  long   hx711Raw      = values[9].toInt();
  float  accX          = values[10].toFloat();
  float  accY          = values[11].toFloat();
  float  accZ          = values[12].toFloat();
  float  gyroX         = values[13].toFloat();
  float  gyroY         = values[14].toFloat();
  float  gyroZ         = values[15].toFloat();
  float  temperature   = values[16].toFloat();

  String json = "{";
  json += "\"node_id\":\"" + jsonEscape(nodeID) + "\",";
  json += "\"node_type\":\"landslide\",";
  json += "\"status\":\"" + jsonEscape(status) + "\",";
  json += "\"counter\":" + String(counter) + ",";
  json += "\"gw_ms\":" + String(millis()) + ",";
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

  Serial.println(json);
}

// =====================================================
// BUILD + PRINT JSON FOR A FIRE PACKET
// =====================================================

void printFireJson(String values[], long counter, int rssi, float snr, const String &raw)
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
  json += "\"counter\":" + String(counter) + ",";
  json += "\"gw_ms\":" + String(millis()) + ",";
  json += "\"flame\":" + String(flameDetected ? "true" : "false") + ",";
  json += "\"mq2Raw\":" + String(mq2Raw) + ",";
  // Only one physical gas sensor (MQ-2) on this node; the same raw
  // value feeds both the "smoke" and "gas" dashboard cards.
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
}