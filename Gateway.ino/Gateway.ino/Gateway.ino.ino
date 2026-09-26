#define ENABLE_USER_AUTH
#define ENABLE_DATABASE

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <SPI.h>
#include <LoRa.h>
#include <FirebaseClient.h>

// =====================================================
// WIFI
// =====================================================

#define WIFI_SSID     "vivo S2"
#define WIFI_PASSWORD "AKASH2008"

// =====================================================
// FIREBASE
// =====================================================

#define API_KEY "AIzaSyBRf67E1Fz0sDHHNQcC2mfDB5lM1X2oYQs"

#define DATABASE_URL \
"https://terrasense-2946c-default-rtdb.asia-southeast1.firebasedatabase.app"

// =====================================================
// FIREBASE AUTH
// =====================================================

#define USER_EMAIL    "terrasense.node2@gmail.com"
#define USER_PASSWORD "faseeha123"

// =====================================================
// LORA
// =====================================================

#define LORA_SCK   18
#define LORA_MISO  19
#define LORA_MOSI  23
#define LORA_SS     5
#define LORA_RST   13
#define LORA_DIO0  33

#define LORA_FREQUENCY 433E6

// =====================================================
// FIREBASE OBJECTS
// =====================================================

UserAuth user_auth(
  API_KEY,
  USER_EMAIL,
  USER_PASSWORD
);

FirebaseApp app;

// Secure connection used by FirebaseClient
WiFiClientSecure ssl_client;

// FirebaseClient 2.x API
using AsyncClient = AsyncClientClass;

AsyncClient aClient(ssl_client);

RealtimeDatabase Database;

// =====================================================
// VARIABLES
// =====================================================

bool firebaseReady = false;

// =====================================================
// FUNCTION DECLARATION
// =====================================================

void sendToFirebase(
  String data,
  int rssi,
  float snr
);

// =====================================================
// SETUP
// =====================================================

void setup()
{
  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("========================================");
  Serial.println("       TERRASENSE NODE 2");
  Serial.println("     LORA + FIREBASE RECEIVER");
  Serial.println("========================================");

  // ===================================================
  // WIFI
  // ===================================================

  Serial.println();
  Serial.println("Connecting to Wi-Fi...");

  WiFi.begin(
    WIFI_SSID,
    WIFI_PASSWORD
  );

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println("Wi-Fi : CONNECTED");

  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());

  // ===================================================
  // FIREBASE
  // ===================================================

  Serial.println();
  Serial.println("Connecting to Firebase...");

  // Skip certificate verification for prototype
  ssl_client.setInsecure();

  // Initialize Firebase
  initializeApp(
    aClient,
    app,
    getAuth(user_auth),
    120 * 1000,
    NULL
  );

  app.getApp<RealtimeDatabase>(Database);

  Database.url(DATABASE_URL);

  // Wait for Firebase authentication
  while (!app.ready())
  {
    app.loop();

    Serial.println("Waiting for Firebase...");
    delay(1000);
  }

  firebaseReady = true;

  Serial.println("Firebase : CONNECTED");

  // ===================================================
  // LORA
  // ===================================================

  Serial.println();
  Serial.println("Starting LoRa...");

  SPI.begin(
    LORA_SCK,
    LORA_MISO,
    LORA_MOSI,
    LORA_SS
  );

  LoRa.setPins(
    LORA_SS,
    LORA_RST,
    LORA_DIO0
  );

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
  Serial.println("Waiting for Node 1...");
  Serial.println("========================================");
}

// =====================================================
// LOOP
// =====================================================

void loop()
{
  // Keep Firebase authentication/tasks alive
  app.loop();

  // ===================================================
  // CHECK LORA
  // ===================================================

  int packetSize = LoRa.parsePacket();

  if (packetSize)
  {
    String data = "";

    while (LoRa.available())
    {
      data += (char)LoRa.read();
    }

    // LoRa signal information
    int rssi = LoRa.packetRssi();

    float snr = LoRa.packetSnr();

    // =================================================
    // DISPLAY RECEIVED PACKET
    // =================================================

    Serial.println();
    Serial.println("========================================");
    Serial.println("       PACKET RECEIVED");
    Serial.println("========================================");

    Serial.print("DATA : ");
    Serial.println(data);

    Serial.print("RSSI : ");
    Serial.print(rssi);
    Serial.println(" dBm");

    Serial.print("SNR  : ");
    Serial.print(snr);
    Serial.println(" dB");

    // =================================================
    // SEND TO FIREBASE
    // =================================================

    if (firebaseReady && app.ready())
    {
      sendToFirebase(
        data,
        rssi,
        snr
      );
    }
    else
    {
      Serial.println("Firebase : NOT READY");
    }

    Serial.println("========================================");
  }
}

// =====================================================
// SEND LORA DATA TO FIREBASE
// =====================================================

void sendToFirebase(
  String data,
  int rssi,
  float snr
)
{
  // ---------------------------------------------------
  // ARRAY FOR NODE 1 PACKET
  // ---------------------------------------------------

  String values[17];

  int index = 0;
  int start = 0;

  // ---------------------------------------------------
  // SPLIT PACKET USING COMMAS
  // ---------------------------------------------------

  while (
    index < 17 &&
    start < data.length()
  )
  {
    int comma = data.indexOf(',', start);

    if (comma == -1)
    {
      values[index] = data.substring(start);
      index++;
      break;
    }

    values[index] =
      data.substring(
        start,
        comma
      );

    index++;

    start = comma + 1;
  }

  // ---------------------------------------------------
  // CHECK PACKET
  // ---------------------------------------------------

  if (index != 17)
  {
    Serial.println();
    Serial.println("Firebase : INVALID PACKET");
    Serial.print("Fields received : ");
    Serial.println(index);

    return;
  }

  // ===================================================
  // EXTRACT NODE 1 DATA
  // ===================================================

  String nodeID = values[0];

  String status = values[1];

  float tiltX =
    values[2].toFloat();

  float tiltY =
    values[3].toFloat();

  float maxTilt =
    values[4].toFloat();

  int soilRaw =
    values[5].toInt();

  String soilWet =
    values[6];

  int rainRaw =
    values[7].toInt();

  String rainDetected =
    values[8];

  long hx711Raw =
    values[9].toInt();

  float accX =
    values[10].toFloat();

  float accY =
    values[11].toFloat();

  float accZ =
    values[12].toFloat();

  float gyroX =
    values[13].toFloat();

  float gyroY =
    values[14].toFloat();

  float gyroZ =
    values[15].toFloat();

  float temperature =
    values[16].toFloat();

  // ===================================================
  // FIREBASE PATH
  // ===================================================

  String basePath = "/TerraSense/";
  basePath += nodeID;

  // ===================================================
  // CREATE JSON OBJECT
  // ===================================================

  object_t json;

  JsonWriter writer;

  object_t obj1;
  object_t obj2;
  object_t obj3;
  object_t obj4;
  object_t obj5;
  object_t obj6;
  object_t obj7;
  object_t obj8;
  object_t obj9;
  object_t obj10;
  object_t obj11;
  object_t obj12;
  object_t obj13;
  object_t obj14;
  object_t obj15;
  object_t obj16;
  object_t obj17;
  object_t obj18;
  object_t obj19;
  object_t obj20;

  // ===================================================
  // CREATE FIREBASE FIELDS
  // ===================================================

  writer.create(
    obj1,
    "status",
    string_t(status)
  );

  writer.create(
    obj2,
    "tiltX",
    number_t(tiltX, 2)
  );

  writer.create(
    obj3,
    "tiltY",
    number_t(tiltY, 2)
  );

  writer.create(
    obj4,
    "maxTilt",
    number_t(maxTilt, 2)
  );

  writer.create(
    obj5,
    "soilRaw",
    soilRaw
  );

  writer.create(
    obj6,
    "soilWet",
    string_t(soilWet)
  );

  writer.create(
    obj7,
    "rainRaw",
    rainRaw
  );

  writer.create(
    obj8,
    "rainDetected",
    string_t(rainDetected)
  );

  writer.create(
    obj9,
    "hx711Raw",
    hx711Raw
  );

  writer.create(
    obj10,
    "accX",
    number_t(accX, 2)
  );

  writer.create(
    obj11,
    "accY",
    number_t(accY, 2)
  );

  writer.create(
    obj12,
    "accZ",
    number_t(accZ, 2)
  );

  writer.create(
    obj13,
    "gyroX",
    number_t(gyroX, 2)
  );

  writer.create(
    obj14,
    "gyroY",
    number_t(gyroY, 2)
  );

  writer.create(
    obj15,
    "gyroZ",
    number_t(gyroZ, 2)
  );

  writer.create(
    obj16,
    "temperature",
    number_t(temperature, 2)
  );

  writer.create(
    obj17,
    "rssi",
    rssi
  );

  writer.create(
    obj18,
    "snr",
    number_t(snr, 2)
  );

  writer.create(
    obj19,
    "online",
    true
  );

  writer.create(
    obj20,
    "lastPacket",
    string_t(data)
  );

  // ===================================================
  // COMBINE ALL OBJECTS
  // ===================================================

  writer.join(
    json,
    20,
    obj1,
    obj2,
    obj3,
    obj4,
    obj5,
    obj6,
    obj7,
    obj8,
    obj9,
    obj10,
    obj11,
    obj12,
    obj13,
    obj14,
    obj15,
    obj16,
    obj17,
    obj18,
    obj19,
    obj20
  );

  // ===================================================
  // SEND ONE JSON OBJECT TO FIREBASE
  // ===================================================

  Database.set<object_t>(
    aClient,
    basePath,
    json
  );

  // ===================================================
  // SERIAL MONITOR
  // ===================================================

  Serial.println();
  Serial.println("[ FIREBASE ]");
  Serial.println("  DATA SENT");

  Serial.print("  PATH : ");
  Serial.println(basePath);

  Serial.println("  STATUS      : " + status);
  Serial.println("  SOIL RAW    : " + String(soilRaw));
  Serial.println("  RAIN RAW    : " + String(rainRaw));
  Serial.println("  HX711 RAW   : " + String(hx711Raw));

  Serial.print("  TEMPERATURE : ");
  Serial.println(temperature);

  Serial.print("  RSSI        : ");
  Serial.println(rssi);

  Serial.print("  SNR         : ");
  Serial.println(snr);

  Serial.println("========================================");
}