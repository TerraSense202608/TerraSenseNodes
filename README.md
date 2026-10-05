<div align="center">

# TerraSense

### Local Edge Intelligence for Multi-Hazard Environmental Monitoring

<p>
  A modular, offline-first platform for localized hazard detection,
  edge-based risk assessment, and early warning in remote and
  low-connectivity environments.
</p>

<p>
  <img src="https://img.shields.io/badge/SIH-2026-blue?style=for-the-badge" alt="SIH 2026"/>
  <img src="https://img.shields.io/badge/PS-SIH26178-orange?style=for-the-badge" alt="SIH26178"/>
  <img src="https://img.shields.io/badge/Domain-Disaster%20Management-red?style=for-the-badge" alt="Disaster Management"/>
  <img src="https://img.shields.io/badge/Platform-ESP32-green?style=for-the-badge&logo=espressif" alt="ESP32"/>
  <img src="https://img.shields.io/badge/Communication-LoRa-blueviolet?style=for-the-badge" alt="LoRa"/>
  <img src="https://img.shields.io/badge/Operation-Offline--First-0f766e?style=for-the-badge" alt="Offline First"/>
</p>

</div>

---

## Overview

TerraSense is a distributed environmental-monitoring platform built around:

- Hazard-specific sensor nodes.
- Low-power LoRa communication.
- A local edge gateway.
- Site-specific risk assessment.
- Offline-first data storage and alerts.

It is designed for remote and vulnerable locations where continuous internet connectivity cannot be guaranteed.

### Core Flow

```text
SENSOR NODES
      ↓
LoRa COMMUNICATION
      ↓
LOCAL EDGE GATEWAY
      ↓
RISK ASSESSMENT
      ↓
LOCAL ALERTS + DATA STORAGE
```

## Why TerraSense?

Existing satellite, GIS, and centralized monitoring systems provide valuable wide-area environmental intelligence. TerraSense complements them with a low-cost, ground-level layer for continuous local sensing and faster field response.

| Existing monitoring systems | TerraSense |
|---|---|
| Regional or area-level observation | Site-level ground sensing |
| Centralized processing | Local edge risk assessment |
| Institutional warning platforms | Direct field alerts |
| Remote access may depend on connectivity | Offline-first local operation |
| Hazard-specific systems | One modular multi-hazard platform |

## Validated Prototypes

### Landslide Node

```text
Tilt -  Soil Moisture -  Rainfall -  Load
ESP32 + LoRa + Local Risk State
```

Site-level slope-condition monitoring.

### Forest-Fire Node

```text
Flame -  Smoke/Gas -  Temperature
ESP32 + LoRa + Local Monitoring
```

Localized fire-condition monitoring.

### Prototype Foundation

```text
ESP32 Sensor Nodes
433 MHz LoRa
Local Gateway
SQLite Storage
Live Dashboard
```

> Flood and pollution monitoring are planned expansion modules, not current validated prototypes.

## System Architecture

```text
┌─────────────────────────────────────────────┐
│           HAZARD-SPECIFIC SENSOR NODES      │
│ Landslide -  Forest Fire -  Flood* -  Pollution*│
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│              LoRa COMMUNICATION              │
│        Low-power local wireless transfer     │
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│               LOCAL EDGE GATEWAY             │
│      Packet validation -  aggregation         │
│             Local data storage               │
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│                 RISK ENGINE                  │
│     Fusion -  trends -  persistence             │
│     validation -  risk classification          │
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│             DASHBOARD AND ALERTS             │
│       LED -  buzzer -  dashboard -  SMS         │
└─────────────────────────────────────────────┘
```

`*` Planned expansion modules.

## Key Capabilities

- ESP32-based distributed sensing.
- Low-power LoRa communication.
- Packet validation and multi-node aggregation.
- Local edge-based risk assessment.
- SQLite-based local data storage.
- Offline dashboard operation.
- Local LED and buzzer alerts.
- Optional SMS and voice notifications.
- Replaceable hazard-specific sensor modules.
- Common risk-state representation.

### Risk States

```text
SAFE → WATCH → WARNING → DANGER
                     ↘
                     FAULT
```

`FAULT` indicates missing, invalid, or unreliable sensor or communication data. It must not be interpreted as a `SAFE` condition.

## Technology Stack

| Layer | Technology |
|---|---|
| Sensor controller | ESP32 |
| Prototype radio | SX1278 / RA-02 LoRa, 433 MHz |
| Prototype gateway | Raspberry Pi 4 |
| Gateway operating system | Raspberry Pi OS |
| Gateway processing | Python |
| Data format | JSON |
| Local database | SQLite |
| Dashboard | React / local web interface |
| Current risk engine | Rule-based edge assessment |
| Future intelligence | Anomaly detection and risk classification |
| Power system | Solar PV + Li-ion battery |
| Enclosure | Weather-resistant field enclosure |

## Risk Assessment

TerraSense combines multiple indicators instead of relying on a single sensor threshold.

```text
Filtering
+ Trend Analysis
+ Rate of Change
+ Persistence
+ Sensor Agreement
+ Hysteresis
+ Packet Validation
+ Sensor Health
```

### Planned Edge-ML Roadmap

- Isolation Forest for anomaly detection.
- Random Forest for risk classification.
- Time-series trend analysis.
- Camera and thermal-sensor fusion.

Machine learning will be introduced after site calibration and collection of verified field data.

---

# TerraSense ML Baselines

The `ml-baseline/` directory contains two small machine-learning baselines that demonstrate the planned pipeline:

```text
DATA → FEATURES → MODEL → RISK / ANOMALY OUTPUT
```

> **Important:** Both models are trained on public datasets to demonstrate the pipeline. They are not trained on TerraSense sensor data and are not currently running on TerraSense nodes. The operational risk engine is rule-based. For deployment, the models will be retrained using data collected from TerraSense nodes.

## Contents

| File | Purpose |
|---|---|
| `fire_random_forest.py` | Fire/smoke baseline using Random Forest classification |
| `landslide_isolation_forest.py` | Landslide baseline using Isolation Forest anomaly detection |
| `check_random_split.py` | Compares time-ordered and random splits for the fire dataset |
| `landslide_anomaly_plot.png` | Landslide anomaly scores for the held-out period |
| `fire_prediction_plot.png` | Fire-model probabilities for held-out readings |
| `screenshots/` | Terminal outputs from model runs |

## Fire Baseline: Random Forest

- **Dataset:** [Smoke Detection Dataset — Kaggle](https://www.kaggle.com/datasets/deepcontractor/smoke-detection-dataset)
- **Dataset size:** 62,630 readings
  - 44,757 alarm readings.
  - 17,873 non-alarm readings.
- **Features:** Temperature, humidity, TVOC, eCO2, raw H2, raw ethanol, PM1.0, PM2.5, NC0.5, NC1.0, and NC2.5.
- **Excluded fields:** Pressure, row counter, and time, because they identify the recording session rather than fire conditions.
- **Label:** `Fire Alarm` — `1` for alarm and `0` for no alarm.
- **Split:** First 70% of readings for training and final 30% for testing, preserving time order.

### Held-Out Results

| Class | Precision | Recall |
|---|---:|---:|
| No alarm | 0.85 | 0.98 |
| Fire alarm | 0.99 | 0.92 |

- **Accuracy:** 0.94 on the time-ordered split.
- **Test readings:** 18,789.
