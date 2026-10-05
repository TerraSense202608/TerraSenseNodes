<div align="center">

# TerraSense

### Local Edge Intelligence for Multi-Hazard Environmental Monitoring

<p>
  A modular, offline-first platform that converts field-level sensor data
  into localized risk assessment and actionable early warnings.
</p>

<p>
  <img src="https://img.shields.io/badge/Smart%20India%20Hackathon-2026-2563eb?style=for-the-badge" alt="Smart India Hackathon 2026"/>
  <img src="https://img.shields.io/badge/Problem%20Statement-SIH26178-f97316?style=for-the-badge" alt="SIH26178"/>
  <img src="https://img.shields.io/badge/Domain-Disaster%20Management-dc2626?style=for-the-badge" alt="Disaster Management"/>
  <img src="https://img.shields.io/badge/ESP32-Edge%20Nodes-16a34a?style=for-the-badge&logo=espressif" alt="ESP32"/>
  <img src="https://img.shields.io/badge/LoRa-Field%20Communication-7c3aed?style=for-the-badge" alt="LoRa"/>
  <img src="https://img.shields.io/badge/Operation-Offline--First-0f766e?style=for-the-badge" alt="Offline First"/>
</p>

</div>

---

## Overview

TerraSense is a distributed environmental-intelligence platform for remote and vulnerable locations.

The platform combines:

- Hazard-specific ESP32 sensor nodes.
- Low-power LoRa communication.
- A local edge gateway.
- Multi-sensor risk assessment.
- Local data storage and alerting.

Its primary design objective is simple:

> **Convert local environmental measurements into actionable warnings, even when internet connectivity is unavailable.**

## Platform Status

| Capability | Status |
|---|---|
| Landslide monitoring node | Validated prototype |
| Forest-fire monitoring node | Validated prototype |
| ESP32 sensor acquisition | Implemented |
| 433 MHz LoRa communication | Implemented in prototype |
| Local gateway processing | Implemented in prototype |
| SQLite data storage | Implemented in prototype |
| Live local dashboard | Implemented in prototype |
| Rule-based risk assessment | Current operational engine |
| Solar-powered enclosure | Deployment configuration |
| Flood and pollution modules | Expansion roadmap |
| Edge-ML inference | Research and validation roadmap |
| Self-healing LoRa mesh | Future network enhancement |

## System Workflow

```text
FIELD SENSING
     ↓
LoRa DATA TRANSFER
     ↓
LOCAL EDGE GATEWAY
     ↓
VALIDATION AND SENSOR FUSION
     ↓
SITE-SPECIFIC RISK STATE
     ↓
LOCAL ALERTS AND DATA LOGGING
```

## System Architecture

```text
┌────────────────────────────────────────────────────┐
│                  SENSOR LAYER                      │
│  Landslide -  Forest Fire -  Flood* -  Pollution*     │
│  ESP32 nodes with hazard-specific sensors           │
└───────────────────────┬────────────────────────────┘
                        │
                        ▼
┌────────────────────────────────────────────────────┐
│              COMMUNICATION LAYER                   │
│       Low-power LoRa point-to-point / star link    │
└───────────────────────┬────────────────────────────┘
                        │
                        ▼
┌────────────────────────────────────────────────────┐
│                 EDGE GATEWAY                       │
│  Packet validation -  aggregation -  local storage   │
│  Raspberry Pi 4 -  Python -  SQLite                  │
└───────────────────────┬────────────────────────────┘
                        │
                        ▼
┌────────────────────────────────────────────────────┐
│                INTELLIGENCE LAYER                  │
│  Filtering -  trends -  persistence -  risk rules     │
│  Sensor health -  anomaly and event validation      │
└───────────────────────┬────────────────────────────┘
                        │
                        ▼
┌────────────────────────────────────────────────────┐
│                RESPONSE LAYER                      │
│  LED -  buzzer -  local dashboard -  SMS* -  voice*    │
└────────────────────────────────────────────────────┘
```

`*` Planned or deployment-dependent capabilities.

## Validated Prototypes

### Landslide Monitoring Node

```text
Tilt -  Soil Moisture -  Rainfall -  Load
ESP32 + LoRa + Local Risk State
```

Supports site-level monitoring of slope-related environmental conditions.

### Forest-Fire Monitoring Node

```text
Flame -  Smoke/Gas -  Temperature
ESP32 + LoRa + Local Monitoring
```

Supports localized monitoring of fire-related conditions.

### Prototype Foundation

```text
ESP32 Sensor Nodes
433 MHz LoRa
Raspberry Pi Gateway
Python Processing
SQLite Storage
Live Dashboard
```

## Risk-State Model

TerraSense represents environmental conditions using a common risk-state model:

```text
SAFE → WATCH → WARNING → DANGER
                     ↘
                     FAULT
```

| State | Meaning |
|---|---|
| `SAFE` | Valid data indicates normal conditions |
| `WATCH` | Early deviation or developing concern |
| `WARNING` | Persistent or multi-sensor abnormality |
| `DANGER` | Strong evidence of hazardous conditions |
| `FAULT` | Sensor, packet, power, or communication data is unreliable |

`FAULT` is intentionally separate from `SAFE` so that missing or unreliable data is not treated as normal operation.

## Edge Risk Assessment

The current engine uses rule-based local processing with multiple evidence checks:

```text
Filtering
+ Rate of Change
+ Trend Analysis
+ Persistence
+ Sensor Agreement
+ Hysteresis
+ Packet Validation
+ Sensor Health
```

This approach reduces dependence on a single threshold and supports explainable risk decisions at the gateway.

## Technology Stack

| Layer | Technology |
|---|---|
| Field controller | ESP32 |
| Prototype radio | SX1278 / RA-02 LoRa, 433 MHz |
| Prototype gateway | Raspberry Pi 4 |
| Gateway OS | Raspberry Pi OS |
| Gateway software | Python |
| Data interchange |
