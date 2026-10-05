#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <MPU6050_tockn.h>
#include <HX711.h>
#include <math.h>

MPU6050 mpu(Wire);

// =====================================================
// TERRASENSE SENSOR PINS
// =====================================================

#define SOIL_PIN 34
#define RAIN_PIN 35

// =====================================================
// HX711 PINS
// =====================================================

#define HX711_DT 16
#define HX711_SCK 17

HX711 scale;

bool hx711Connected = false;

// =====================================================
// LED + BUZZER PINS
// =====================================================

#define GREEN_LED 25
#define YELLOW_LED 26
#define RED_LED 27
#define BUZZER 14

// =====================================================
// LED POLARITY
// =====================================================

#define GREEN_ON HIGH
#define GREEN_OFF LOW

#define YELLOW_ON HIGH
#define YELLOW_OFF LOW

#define RED_ON HIGH
#define RED_OFF LOW

// =====================================================
// BUZZER POLARITY
// =====================================================

#define BUZZER_ON LOW
#define BUZZER_OFF HIGH

// =====================================================
// SOIL / RAIN THRESHOLDS
// =====================================================

#define SOIL_WET_THRESHOLD 2000
#define RAIN_WET_THRESHOLD 2000

// =====================================================
// TILT THRESHOLDS
// =====================================================

#define TILT_WARNING 15.0
#define TILT_DANGER 35.0

// =====================================================
// BUZZER
// =====================================================

unsigned long lastBuzzerTime = 0;
bool buzzerState = false;

const unsigned long BUZZER_INTERVAL = 300;

// =====================================================
// MPU
// =====================================================

bool mpuConnected = false;

float baseTiltX = 0.0;
float baseTiltY = 0.0;

// =====================================================
// LORA - ADDED ONLY
// =====================================================

#define LORA_SCK   18
#define LORA_MISO  19
#define LORA_MOSI  23
#define LORA_SS     5
#define LORA_RST   13
#define LORA_DIO0  33

#define LORA_FREQUENCY 433E6

bool loraConnected = false;

unsigned long packetCounter = 0;
unsigned long nextSendTime = 0;

#define SEND_INTERVAL_MS 6000   // base LoRa interval
#define SEND_JITTER_MS   1500   // random extra 0 to 1500 ms
// =====================================================
// NORMALIZE ANGLE
// =====================================================

float normalizeAngle(float angle)
{
  while (angle > 180.0)
  {
    angle -= 360.0;
  }

  while (angle < -180.0)
  {
    angle += 360.0;
  }

  return angle;
}

// =====================================================
// SETUP
// =====================================================

void setup()
{
  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("========================================");
  Serial.println("        TERRASENSE SENSOR NODE");
  Serial.println("========================================");

  // ===================================================
  // ADC
  // ===================================================

  analogReadResolution(12);

  pinMode(SOIL_PIN, INPUT);
  pinMode(RAIN_PIN, INPUT);

  // ===================================================
  // LEDS
  // ===================================================

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // ===================================================
  // BUZZER
  // ===================================================

  pinMode(BUZZER, OUTPUT);

  // ===================================================
  // INITIAL OUTPUT STATE
  // ===================================================

  digitalWrite(GREEN_LED, GREEN_OFF);
  digitalWrite(YELLOW_LED, YELLOW_OFF);
  digitalWrite(RED_LED, RED_OFF);

  digitalWrite(BUZZER, BUZZER_OFF);

  // ===================================================
  // I2C
  // ===================================================

  Wire.begin(21, 22);

  // ===================================================
  // MPU6050
  // ===================================================

  Serial.println("Checking MPU6050...");

  mpu.begin();

  Serial.println("Keep MPU still...");

  mpu.calcGyroOffsets(true);

  mpuConnected = true;

  Serial.println("MPU6050 : CONNECTED");

  // ===================================================
  // STORE FLAT POSITION
  // ===================================================

  delay(500);

  mpu.update();

  float ax = mpu.getAccX();
  float ay = mpu.getAccY();
  float az = mpu.getAccZ();

  baseTiltX =
    atan2(ay, az) * 180.0 / PI;

  baseTiltY =
    atan2(
      -ax,
      sqrt(
        (ay * ay) +
        (az * az)
      )
    ) * 180.0 / PI;

  Serial.println();
  Serial.println("Flat position stored.");

  Serial.print("Base Tilt X : ");
  Serial.print(baseTiltX, 2);
  Serial.println(" deg");

  Serial.print("Base Tilt Y : ");
  Serial.print(baseTiltY, 2);
  Serial.println(" deg");

  // ===================================================
  // HX711 INITIALIZATION
  // ===================================================

  Serial.println();
  Serial.println("Checking HX711...");

  // SAME INITIALIZATION AS WORKING TEST CODE
  scale.begin(HX711_DT, HX711_SCK);

  delay(500);

  if (scale.is_ready())
  {
    hx711Connected = true;

    Serial.println("HX711 : CONNECTED");
    Serial.println("Live raw data enabled.");
  }
  else
  {
    hx711Connected = false;

    Serial.println("HX711 :  CONNECTED");
    Serial.println("Check HX711 wiring.");
  }

  // ===================================================
  // LORA INITIALIZATION - ADDED ONLY
  // ===================================================

  Serial.println();
  Serial.println("Checking RA-02 LoRa...");

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

  if (LoRa.begin(LORA_FREQUENCY))
  {
    loraConnected = true;


LoRa.enableCrc();
randomSeed((uint32_t)ESP.getEfuseMac() ^ micros());

    Serial.println("LoRa : CONNECTED");
    Serial.println("LoRa Frequency : 433 MHz");
  }
  else
  {
    loraConnected = false;

    Serial.println("LoRa : NOT CONNECTED");
  }

  // ===================================================
  // PIN INFORMATION
  // ===================================================

  Serial.println();
  Serial.println("Sensor configuration:");

  Serial.println("Soil Sensor : GPIO 34");
  Serial.println("Rain Sensor : GPIO 35");

  Serial.println("MPU6050 SDA : GPIO 21");
  Serial.println("MPU6050 SCL : GPIO 22");

  Serial.println("HX711 DT    : GPIO 16");
  Serial.println("HX711 SCK   : GPIO 17");

  Serial.println();
  Serial.println("Indicators:");

  Serial.println("Green LED   : GPIO 25");
  Serial.println("Yellow LED  : GPIO 26");
  Serial.println("Red LED     : GPIO 27");
  Serial.println("Buzzer      : GPIO 14");

  Serial.println();
  Serial.println("LoRa:");

  Serial.println("SCK         : GPIO 18");
  Serial.println("MISO        : GPIO 19");
  Serial.println("MOSI        : GPIO 23");
  Serial.println("NSS         : GPIO 5");
  Serial.println("RST         : GPIO 13");
  Serial.println("DIO0        : GPIO 33");

  // ===================================================
  // STATUS LEVELS
  // ===================================================

  Serial.println();
  Serial.println("========================================");

  Serial.println("TILT STATUS");

  Serial.println("0 - 14.99 deg : SAFE");
  Serial.println("15 - 34.99 deg: WARNING");
  Serial.println(">= 35 deg     : DANGER");

  Serial.println();
  Serial.println("UPDATE RATE   : 1000 ms");

  Serial.println("========================================");

  delay(1000);
}

// =====================================================
// MAIN LOOP
// =====================================================

void loop()
{
  // ===================================================
  // 1. SOIL SENSOR
  // ===================================================

  int soilValue = analogRead(SOIL_PIN);

  bool soilWet = false;

  if (soilValue >= 0 && soilValue <= 4095)
  {
    if (soilValue < SOIL_WET_THRESHOLD)
    {
      soilWet = true;
    }
  }

  // ===================================================
  // 2. RAIN SENSOR
  // ===================================================

  int rainValue = analogRead(RAIN_PIN);

  bool raining = false;

  if (rainValue >= 0 && rainValue <= 4095)
  {
    if (rainValue < RAIN_WET_THRESHOLD)
    {
      raining = true;
    }
  }

  // ===================================================
  // 3. MPU VARIABLES
  // ===================================================

  float accX = 0.0;
  float accY = 0.0;
  float accZ = 0.0;

  float gyroX = 0.0;
  float gyroY = 0.0;
  float gyroZ = 0.0;

  float temperature = 0.0;

  float tiltX = 0.0;
  float tiltY = 0.0;

  float relativeTiltX = 0.0;
  float relativeTiltY = 0.0;

  float maxTilt = 0.0;

  // ===================================================
  // 4. READ MPU6050
  // ===================================================

  if (mpuConnected)
  {
    mpu.update();

    accX = mpu.getAccX();
    accY = mpu.getAccY();
    accZ = mpu.getAccZ();

    gyroX = mpu.getGyroX();
    gyroY = mpu.getGyroY();
    gyroZ = mpu.getGyroZ();

    temperature = mpu.getTemp();

    // =================================================
    // CALCULATE TILT
    // =================================================

    tiltX =
      atan2(
        accY,
        accZ
      ) * 180.0 / PI;

    tiltY =
      atan2(
        -accX,
        sqrt(
          (accY * accY) +
          (accZ * accZ)
        )
      ) * 180.0 / PI;

    // =================================================
    // RELATIVE TILT
    // =================================================

    relativeTiltX =
      normalizeAngle(
        tiltX - baseTiltX
      );

    relativeTiltY =
      normalizeAngle(
        tiltY - baseTiltY
      );

    // =================================================
    // MAXIMUM TILT
    // =================================================

    maxTilt =
      fmax(
        fabs(relativeTiltX),
        fabs(relativeTiltY)
      );
  }

  // ===================================================
  // 4B. HX711 LIVE READING
  // ===================================================

  long hx711Value = 0;

  if (hx711Connected)
  {
    if (scale.is_ready())
    {
      // Read LIVE raw value
      hx711Value = scale.read();
    }
    else
    {
      Serial.println("WARNING: HX711 NOT READY!");
    }
  }

  // ===================================================
  // 5. DETERMINE STATUS
  // ===================================================

  const char* status;

  if (maxTilt >= TILT_DANGER)
  {
    status = "DANGER";
  }
  else if (maxTilt >= TILT_WARNING)
  {
    status = "WARNING";
  }
  else
  {
    status = "SAFE";
  }

  // ===================================================
  // 6. TURN ALL OUTPUTS OFF
  // ===================================================

  digitalWrite(
    GREEN_LED,
    GREEN_OFF
  );

  digitalWrite(
    YELLOW_LED,
    YELLOW_OFF
  );

  digitalWrite(
    RED_LED,
    RED_OFF
  );

  // ===================================================
  // 7. LED INDICATION
  // ===================================================

  if (maxTilt < TILT_WARNING)
  {
    digitalWrite(
      GREEN_LED,
      GREEN_ON
    );
  }
  else if (maxTilt < TILT_DANGER)
  {
    digitalWrite(
      YELLOW_LED,
      YELLOW_ON
    );
  }
  else
  {
    digitalWrite(
      RED_LED,
      RED_ON
    );
  }

  // ===================================================
  // 8. BUZZER
  // ===================================================

  if (maxTilt >= TILT_DANGER)
  {
    unsigned long currentTime = millis();

    if (
      currentTime - lastBuzzerTime
      >= BUZZER_INTERVAL
    )
    {
      lastBuzzerTime = currentTime;

      buzzerState = !buzzerState;

      if (buzzerState)
      {
        digitalWrite(
          BUZZER,
          BUZZER_ON
        );
      }
      else
      {
        digitalWrite(
          BUZZER,
          BUZZER_OFF
        );
      }
    }
  }
  else
  {
    buzzerState = false;

    digitalWrite(
      BUZZER,
      BUZZER_OFF
    );

    lastBuzzerTime = millis();
  }

  // ===================================================
  // 9. SERIAL OUTPUT
  // ===================================================

  Serial.println();

  Serial.println("----------------------------------------");

  // ===================================================
  // SOIL
  // ===================================================

  Serial.println("[ SOIL SENSOR ]");

  Serial.print("  Moisture (raw)  : ");
  Serial.println(soilValue);

  Serial.print("  WET             : ");

  if (soilWet)
  {
    Serial.println("YES");
  }
  else
  {
    Serial.println("NO");
  }

  // ===================================================
  // RAIN
  // ===================================================

  Serial.println("[ RAIN SENSOR ]");

  Serial.print("  Rain Level (raw): ");
  Serial.println(rainValue);

  Serial.print("  RAIN DETECTED   : ");

  if (raining)
  {
    Serial.println("YES");
  }
  else
  {
    Serial.println("NO");
  }

  // ===================================================
  // HX711
  // ===================================================

  Serial.println("[ HX711 LOAD CELL ]");

  if (hx711Connected)
  {
    Serial.print("  Load Cell RAW   : ");
    Serial.println(hx711Value);
  }
  else
  {
    Serial.println("  STATUS          : NOT CONNECTED");
  }

  // ===================================================
  // MPU6050
  // ===================================================

  Serial.println("[ MPU6050 ]");

  if (mpuConnected)
  {
    Serial.println("  -- Acceleration --");

    Serial.print("  X : ");
    Serial.print(accX, 2);
    Serial.println(" g");

    Serial.print("  Y : ");
    Serial.print(accY, 2);
    Serial.println(" g");

    Serial.print("  Z : ");
    Serial.print(accZ, 2);
    Serial.println(" g");

    Serial.println("  -- Gyro --");

    Serial.print("  X : ");
    Serial.print(gyroX, 2);
    Serial.println(" deg/s");

    Serial.print("  Y : ");
    Serial.print(gyroY, 2);
    Serial.println(" deg/s");

    Serial.print("  Z : ");
    Serial.print(gyroZ, 2);
    Serial.println(" deg/s");

    Serial.println("  -- Tilt --");

    Serial.print("  X : ");
    Serial.print(relativeTiltX, 2);
    Serial.println(" deg");

    Serial.print("  Y : ");
    Serial.print(relativeTiltY, 2);
    Serial.println(" deg");

    Serial.print("  Maximum Tilt : ");
    Serial.print(maxTilt, 2);
    Serial.println(" deg");

    Serial.print("  -- Temperature : ");
    Serial.print(temperature, 2);
    Serial.println(" C");
  }
  else
  {
    Serial.println("  STATUS : NOT CONNECTED");
  }

  // ===================================================
  // TERRASENSE STATUS
  // ===================================================

  Serial.println();

  Serial.println("[ TERRASENSE STATUS ]");

  Serial.print("  STATUS : ");
  Serial.println(status);

  // ===================================================
  // TILT PRIORITY
  // ===================================================

  Serial.print("  TILT PRIORITY : ");

  if (maxTilt >= TILT_DANGER)
  {
    Serial.println("DANGER");
  }
  else if (maxTilt >= TILT_WARNING)
  {
    Serial.println("WARNING");
  }
  else
  {
    Serial.println("NORMAL");
  }

  // ===================================================
  // LED STATUS
  // ===================================================

  Serial.print("  GREEN LED  : ");

  if (maxTilt < TILT_WARNING)
  {
    Serial.println("YES");
  }
  else
  {
    Serial.println("NO");
  }

  Serial.print("  YELLOW LED : ");

  if (
    maxTilt >= TILT_WARNING &&
    maxTilt < TILT_DANGER
  )
  {
    Serial.println("YES");
  }
  else
  {
    Serial.println("NO");
  }

  Serial.print("  RED LED    : ");

  if (maxTilt >= TILT_DANGER)
  {
    Serial.println("YES");
  }
  else
  {
    Serial.println("NO");
  }

  // ===================================================
  // BUZZER STATUS
  // ===================================================

  Serial.print("  BUZZER     : ");

  if (maxTilt >= TILT_DANGER)
  {
    Serial.println("YES");
  }
  else
  {
    Serial.println("NO");
  }

  // ===================================================
  // LORA TRANSMISSION - ADDED ONLY
  // ===================================================

 if (loraConnected && millis() >= nextSendTime)
  {
    String packet = "";

    packet += "NODE1,";
    packet += status;
    packet += ",";
    packet += String(relativeTiltX, 2);
    packet += ",";
    packet += String(relativeTiltY, 2);
    packet += ",";
    packet += String(maxTilt, 2);
    packet += ",";
    packet += String(soilValue);
    packet += ",";
    packet += (soilWet ? "YES" : "NO");
    packet += ",";
    packet += String(rainValue);
    packet += ",";
    packet += (raining ? "YES" : "NO");
    packet += ",";
    packet += String(hx711Value);
    packet += ",";
    packet += String(accX, 2);
    packet += ",";
    packet += String(accY, 2);
    packet += ",";
    packet += String(accZ, 2);
    packet += ",";
    packet += String(gyroX, 2);
    packet += ",";
    packet += String(gyroY, 2);
    packet += ",";
    packet += String(gyroZ, 2);
    packet += ",";
    packet += String(temperature, 2);

    packet += ",";
packet += String(packetCounter++);

    LoRa.beginPacket();
    LoRa.print(packet);
    LoRa.endPacket();

    nextSendTime = millis() + SEND_INTERVAL_MS + random(0, SEND_JITTER_MS + 1);

    Serial.println();
    Serial.println("[ LORA ]");
    Serial.println("  DATA SENT TO NODE 2");
  }

  Serial.println("----------------------------------------");

  // ===================================================
  // UPDATE EVERY 1 SECOND
  // ===================================================

  delay(1000);
}