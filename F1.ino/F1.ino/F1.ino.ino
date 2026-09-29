#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

// =====================================================
// TERRASENSE - FOREST FIRE NODE
// ESP32 DEVKIT V1
// =====================================================

// -----------------------------------------------------
// NODE ID
// -----------------------------------------------------

#define NODE_ID "FIRE01"

// =====================================================
// SENSOR PIN DEFINITIONS
// =====================================================

// 🔥 Flame Sensor
#define FLAME_PIN 27

// 🌡️ BMP280 / HW-611 I2C
#define BMP_SDA 21
#define BMP_SCL 22

// 💨 MQ-2 Analog Output
#define MQ2_PIN 34

// =====================================================
// LoRa SX1278 / RA-02 PIN DEFINITIONS
// =====================================================

#define LORA_SCK   18
#define LORA_MISO  19
#define LORA_MOSI  23
#define LORA_NSS   5
#define LORA_RST   13
#define LORA_DIO0  33

#define LORA_FREQUENCY 433E6

// =====================================================
// BMP280 OBJECT
// =====================================================

Adafruit_BMP280 bmp;

bool bmp280Available = false;

// =====================================================
// SENSOR VARIABLES
// =====================================================

int flameRaw;
int mq2Raw;

bool flameDetected;

float temperature = 0.0;
float pressure = 0.0;

// =====================================================
// RISK STATUS
// =====================================================

String riskStatus;

// =====================================================
// MQ-2 PROTOTYPE THRESHOLDS
// =====================================================

// IMPORTANT:
// These are starting values only.
// Calibrate them using your actual MQ-2 sensor.

// SAFE      < 1800
// WARNING   1800 - 2599
// DANGER    >= 2600

// =====================================================

#define MQ2_WARNING 1800
#define MQ2_DANGER  2600

// =====================================================
// TEMPERATURE SUPPORTING THRESHOLDS
// =====================================================

// Temperature alone does NOT prove a forest fire.
// It is used as a supporting environmental parameter.

// =====================================================

#define TEMP_WARNING 40.0
#define TEMP_DANGER  50.0

// =====================================================
// SETUP
// =====================================================

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("==============================================");
  Serial.println("       TERRASENSE - FOREST FIRE NODE");
  Serial.println("==============================================");

  Serial.print("Node ID: ");
  Serial.println(NODE_ID);

  // =================================================
  // FLAME SENSOR
  // =================================================

  pinMode(FLAME_PIN, INPUT);

  Serial.println("Flame sensor initialized.");

  // =================================================
  // MQ-2
  // =================================================

  pinMode(MQ2_PIN, INPUT);

  // ESP32 ADC configuration
  analogReadResolution(12);

  Serial.println("MQ-2 initialized.");

  // =================================================
  // BMP280 / HW-611
  // =================================================

  Wire.begin(BMP_SDA, BMP_SCL);

  Serial.println("Initializing HW-611 / BMP280...");

  // Try address 0x76
  if (bmp.begin(0x76))
  {
    bmp280Available = true;
    Serial.println("HW-611 / BMP280 found at 0x76.");
  }
  else
  {
    Serial.println("HW-611 / BMP280 not found at 0x76.");
    Serial.println("Trying 0x77...");

    // Try address 0x77
    if (bmp.begin(0x77))
    {
      bmp280Available = true;
      Serial.println("HW-611 / BMP280 found at 0x77.");
    }
    else
    {
      bmp280Available = false;

      Serial.println("WARNING: HW-611 / BMP280 not found!");
      Serial.println("Temperature and pressure will show 0.");
      Serial.println("Check SDA, SCL, VCC and GND.");
      Serial.println("Continuing with other sensors...");
    }
  }

  // =================================================
  // LoRa
  // =================================================

  Serial.println("Initializing LoRa...");

  SPI.begin(
    LORA_SCK,
    LORA_MISO,
    LORA_MOSI,
    LORA_NSS
  );

  LoRa.setPins(
    LORA_NSS,
    LORA_RST,
    LORA_DIO0
  );

  if (!LoRa.begin(LORA_FREQUENCY))
  {
    Serial.println("ERROR: LoRa initialization failed!");
    Serial.println("Check RA-02 wiring and 3.3V power.");

    while (true)
    {
      delay(1000);
    }
  }

  // LoRa configuration
  LoRa.setTxPower(17);
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);

  Serial.println("LoRa initialized successfully.");
  Serial.println("Frequency: 433 MHz");

  Serial.println();
  Serial.println("==============================================");
  Serial.println("        FOREST FIRE NODE READY");
  Serial.println("==============================================");
}

// =====================================================
// READ ALL SENSORS
// =====================================================

void readSensors()
{
  // ---------------------------------------------------
  // FLAME SENSOR
  // ---------------------------------------------------

  flameRaw = digitalRead(FLAME_PIN);

  /*
     Most flame sensor modules:

     LOW  = Flame detected
     HIGH = No flame

     If your module behaves opposite,
     change the condition below.
  */

  flameDetected = (flameRaw == LOW);

  // ---------------------------------------------------
  // MQ-2
  // ---------------------------------------------------

  mq2Raw = analogRead(MQ2_PIN);

  // ---------------------------------------------------
  // BMP280 / HW-611
  // ---------------------------------------------------

  if (bmp280Available)
  {
    temperature = bmp.readTemperature();

    pressure = bmp.readPressure() / 100.0F;
  }
  else
  {
    temperature = 0.0;
    pressure = 0.0;
  }
}

// =====================================================
// DETERMINE RISK
// =====================================================

void calculateRisk()
{
  bool smokeWarning = false;
  bool smokeDanger = false;

  bool temperatureWarning = false;
  bool temperatureDanger = false;

  // ---------------------------------------------------
  // MQ-2
  // ---------------------------------------------------

  if (mq2Raw >= MQ2_DANGER)
  {
    smokeDanger = true;
  }
  else if (mq2Raw >= MQ2_WARNING)
  {
    smokeWarning = true;
  }

  // ---------------------------------------------------
  // TEMPERATURE
  // ---------------------------------------------------

  // Only evaluate temperature if BMP280 is available
  if (bmp280Available)
  {
    if (temperature >= TEMP_DANGER)
    {
      temperatureDanger = true;
    }
    else if (temperature >= TEMP_WARNING)
    {
      temperatureWarning = true;
    }
  }

  // ===================================================
  // DANGER
  // ===================================================

  // Flame detection is treated as a direct danger
  // indicator in this prototype.

  // ===================================================

  if (
    flameDetected ||
    smokeDanger ||
    temperatureDanger
  )
  {
    riskStatus = "DANGER";
  }

  // ===================================================
  // WARNING
  // ===================================================

  else if (
    smokeWarning ||
    temperatureWarning
  )
  {
    riskStatus = "WARNING";
  }

  // ===================================================
  // SAFE
  // ===================================================

  else
  {
    riskStatus = "SAFE";
  }
}

// =====================================================
// PRINT SENSOR DATA
// =====================================================

void printSensorData()
{
  Serial.println();
  Serial.println("--------------- SENSOR DATA ---------------");

  Serial.print("Node ID       : ");
  Serial.println(NODE_ID);

  // Flame
  Serial.print("Flame         : ");

  if (flameDetected)
  {
    Serial.println("🔥 DETECTED");
  }
  else
  {
    Serial.println("No flame");
  }

  Serial.print("Flame Raw     : ");
  Serial.println(flameRaw);

  // MQ-2
  Serial.print("MQ-2 Raw      : ");
  Serial.println(mq2Raw);

  // Temperature
  Serial.print("Temperature   : ");

  if (bmp280Available)
  {
    Serial.print(temperature, 2);
    Serial.println(" °C");
  }
  else
  {
    Serial.println("N/A");
  }

  // Pressure
  Serial.print("Pressure      : ");

  if (bmp280Available)
  {
    Serial.print(pressure, 2);
    Serial.println(" hPa");
  }
  else
  {
    Serial.println("N/A");
  }

  // Risk
  Serial.print("Risk Status   : ");
  Serial.println(riskStatus);

  Serial.println("--------------------------------------------");
}

// =====================================================
// SEND LoRa PACKET
// =====================================================

void sendLoRaPacket()
{
  /*
     Packet format:

     FIRE01,DANGER,1,2800,52.40,1008.20

     Field 1 = Node ID
     Field 2 = Risk Status
     Field 3 = Flame
     Field 4 = MQ-2 raw
     Field 5 = Temperature
     Field 6 = Pressure
  */

  String packet = "";

  packet += NODE_ID;
  packet += ",";

  packet += riskStatus;
  packet += ",";

  packet += flameDetected ? "1" : "0";
  packet += ",";

  packet += String(mq2Raw);
  packet += ",";

  packet += String(temperature, 2);
  packet += ",";

  packet += String(pressure, 2);

  // ---------------------------------------------------
  // Display packet
  // ---------------------------------------------------

  Serial.println();
  Serial.println("--------------- LoRa TX ------------------");

  Serial.print("Packet: ");
  Serial.println(packet);

  // ---------------------------------------------------
  // Transmit
  // ---------------------------------------------------

  LoRa.beginPacket();

  LoRa.print(packet);

  LoRa.endPacket();

  Serial.println("Status: SENT");
  Serial.println("-------------------------------------------");
}

// =====================================================
// MAIN LOOP
// =====================================================

void loop()
{
  // 1. Read sensors
  readSensors();

  // 2. Calculate risk
  calculateRisk();

  // 3. Display values
  printSensorData();

  // 4. Send through LoRa
  sendLoRaPacket();

  // 5. Wait 3 seconds
  delay(3000);
}