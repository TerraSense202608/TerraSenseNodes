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
LOCAL AND REMOTE ALERTS
```

## Why TerraSense?

Existing satellite, GIS, and centralized monitoring systems provide valuable wide-area environmental intelligence. TerraSense complements these systems with a low-cost, ground-level layer for continuous local sensing and faster field response.

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
│  Landslide -  Forest Fire -  Flood* -  Pollution*│
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│               LoRa COMMUNICATION             │
│         Low-power local wireless transfer    │
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│              LOCAL EDGE GATEWAY              │
│       Packet validation -  aggregation        │
│              Local data storage              │
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│               RISK ENGINE                    │
│       Fusion -  trends -  persistence          │
│       validation -  risk classification       │
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│           DASHBOARD AND ALERTS               │
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

`FAULT` indicates missing, invalid, or unreliable sensor or communication data. It must not be interpreted as a SAFE condition.

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

# TerraSense: ML Baseline (in development)

This folder contains two small machine-learning baselines for TerraSense. They show the pipeline we plan to use: **data → features → model → risk/anomaly output**.

> **Important:** both models are trained on **public datasets** to demonstrate the pipeline. They are **not trained on TerraSense sensor data** and are **not running on our nodes**. The working risk engine today is rule-based. For deployment, we will retrain on data collected from our own nodes (data logging is in development).

## Contents

| File | Purpose |
|---|---|
| `fire_random_forest.py` | Fire/smoke baseline (Random Forest classifier) |
| `landslide_isolation_forest.py` | Landslide baseline (Isolation Forest anomaly detection) |
| `check_random_split.py` | Compares a time-ordered split with a random split on the fire data |
| `landslide_anomaly_plot.png` | Landslide anomaly scores on the held-out period |
| `fire_prediction_plot.png` | Fire model probability on the held-out readings |
| `screenshots/` | Terminal outputs from our runs |

## 1. Fire baseline: Random Forest

- **Dataset:** Smoke Detection Dataset (Blattmann), Kaggle: https://www.kaggle.com/datasets/deepcontractor/smoke-detection-dataset. Licence: **[LICENCE: copy from the dataset page]**
- **Size:** 62,630 readings (44,757 alarm, 17,873 no alarm)
- **Features (11):** temperature, humidity, TVOC, eCO2, raw H2, raw ethanol, PM1.0, PM2.5, NC0.5, NC1.0, NC2.5. Pressure, the row counter and time are left out because they identify the recording session, not the fire.
- **Label:** Fire Alarm (1 = alarm, 0 = no alarm)
- **Split:** first 70% of readings in time order for training, last 30% for testing

**Result on the held-out 30% (18,789 readings):**

| Class | Precision | Recall |
|---|---|---|
| No alarm | 0.85 | 0.98 |
| Fire alarm | 0.99 | 0.92 |

Overall accuracy is 0.94 (0.938 in `check_random_split.py`). The model missed 1,032 real alarm readings and raised 135 false alarms.

**Why a time-ordered split:** a random split of the same data gives an accuracy of 1.00, because neighbouring seconds are almost identical and leak between the training and test sets. We report the time-ordered result. Run `check_random_split.py` to reproduce both numbers.

**Limitations:** the sensors differ from ours, the flame sensor is not represented, and the data comes from a few recording sessions, so the score is not a TerraSense accuracy.

## 2. Landslide baseline: Isolation Forest

- **Dataset:** Pukrongta et al. (2025), Landslide Monitoring Dataset, Mendeley Data, DOI 10.17632/9w43sg73bt.1: https://doi.org/10.17632/9w43sg73bt.1. Licence: **[LICENCE: copy from the dataset page]**
- **Size:** about 40,600 readings from 2 ESP32 nodes (Node1 and Node2), 13 March to 15 April 2025
- **Features (12):** tilt (Rotation X, Rotation Y), soil moisture at 20, 40 and 60 cm, the rate of change of each soil reading, rain (raindrop), vibration, temperature, humidity
- **Cleaning:** start-up rows where temperature, humidity or all soil sensors read 0 are removed
- **Split:** first 70% of each node's readings for training, last 30% for testing (time order)
- **Method:** the model learns what normal readings look like and flags unusual ones. The dataset has **no event labels**, so this is unsupervised and **we report no accuracy**.

**Output on the held-out 30%:**

| Node | Test readings | Flagged |
|---|---|---|
| Node1 | 5,216 | 373 (7.2%) |
| Node2 | 6,965 | 8 (0.1%) |

**How to read it:** the flags show a working pipeline, not landslide detection. Node1's test period differs from its training period (soil moisture shifts over the weeks), which explains most of its flags. The dataset has almost no rain or vibration events in the test period, so the flags cannot be checked against real events.

## How to run

1. Download the two datasets from the links above and place them in this folder (the data files are not included here, because they belong to their authors).
   - `smoke_detection_iot.csv` (from Kaggle)
   - `Raw_Sensor_Data_Landslide_Monitoring__1_.xlsx` (from Mendeley Data). If your file name differs, edit the `FILE = ...` line in the script.
2. Install the packages:
   ```
   py -m pip install pandas scikit-learn matplotlib openpyxl
   ```
3. Run:
   ```
   py fire_random_forest.py
   py landslide_isolation_forest.py
   py check_random_split.py
   ```

## What comes next

1. Add data logging to the dashboard so the nodes save their readings.
2. Collect normal and abnormal-condition data from our own nodes.
3. Retrain both models on that data and validate in time order.
4. Run the model on the Raspberry Pi gateway, with the rule-based engine kept as the fallback.

## References

- Blattmann, Smoke Detection Dataset, Kaggle.
- Pukrongta et al. (2025), Landslide Monitoring Dataset, Mendeley Data, DOI 10.17632/9w43sg73bt.1.

Part of TerraSense, Team Yukti, Smart India Hackathon 2026 (Problem Statement 26178).


## Resilience and Fault Handling

TerraSense is designed to maintain essential local operation during internet outages.

```text
Internet Failure
→ Local Processing Continues
→ Local Alerts Continue
→ Data Is Stored Locally
→ Synchronization Can Occur Later
```

The current prototype uses a LoRa point-to-point or star communication model. A self-healing multi-hop LoRa mesh is a future enhancement and is not claimed as part of the current implementation.

## Deployment Roadmap

```text
Phase 1 — Validated Prototype
ESP32 + 433 MHz LoRa + Raspberry Pi 4

Phase 2 — Field Pilot
Calibrated sensors + solar power + weatherproof enclosure

Phase 3 — Commercial Architecture
IN865 LoRa + Qualcomm Dragonwing RB3 Gen 2 edge platform

Phase 4 — Advanced Intelligence
Edge ML + camera/thermal fusion + multi-site monitoring
```

## Expansion Modules

### Flood Monitoring

```text
Water Level -  Rate of Rise -  Rainfall
```

### Pollution Monitoring

```text
PM2.5 -  PM10 -  Gas Concentration -  Air Quality
```

These modules are designed to use the existing gateway, LoRa network, storage, dashboard, and risk-assessment infrastructure.

## Local Development

### Prerequisites

- Python 3.10+
- Node.js 18+
- Raspberry Pi OS for gateway deployment
- ESP32 development environment
- Compatible LoRa hardware
- SQLite

### Clone the Repository

```bash
git clone [https://github.com/](https://github.com/)<organization>/<repository>.git
cd terrasense
```

### Gateway Setup

```bash
python -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
python gateway/main.py
```

### Dashboard Setup

```bash
cd dashboard
npm install
npm run dev
```

> Update these commands according to the final repository structure.

## Repository Structure

```text
terrasense/
├── firmware/
│   ├── landslide-node/
│   └── forest-fire-node/
├── gateway/
│   ├── packet_handler/
│   ├── risk_engine/
│   ├── storage/
│   └── main.py
├── dashboard/
├── data/
├── docs/
│   ├── architecture.md
│   ├── deployment.md
│   └── calibration.md
├── tests/
├── requirements.txt
├── .env.example
└── README.md
```

## Cost Estimation

| Deployment level | Estimated cost |
|---|---:|
| Prototype node | ₹5,000–₹10,000 |
| Prototype system | ₹20,000–₹40,000 |
| Field pilot | ₹50,000–₹1,00,000 per site |
| Multi-site deployment | ₹1,00,000–₹3,00,000+ |

These are indicative estimates. Actual cost depends on sensor accuracy, enclosure rating, solar capacity, gateway hardware, installation, and calibration.

## Current Scope and Limitations

- Landslide and forest-fire nodes are the validated prototypes.
- Flood and pollution nodes are planned expansion modules.
- The current prototype uses 433 MHz LoRa hardware.
- The current network is not a self-healing mesh.
- Thresholds require site-specific calibration.
- ML models require verified field data.
- TerraSense provides risk assessment and early warning, not guaranteed disaster prediction.

## Impact

TerraSense supports:

- Localized hazard monitoring.
- Faster field verification.
- Offline-capable warning delivery.
- Shared infrastructure across hazards.
- Modular maintenance and sensor replacement.
- Reduced dependence on continuous cloud connectivity.

## Contributing

1. Fork the repository.
2. Create a feature branch:

```bash
git checkout -b feature/your-feature
```

3. Commit your changes:

```bash
git commit -m "Add your feature"
```

4. Push the branch:

```bash
git push origin feature/your-feature
```

5. Open a pull request.

For hardware or sensing changes, include test conditions, sensor details, logs, and calibration information.



# Related Work and Prior Art

This page compares TerraSense with existing systems, based on publicly available information (accessed 30 September 2026). TerraSense is a prototype-stage project. Differences are listed only where we can demonstrate them, and each is marked Working, In development or Planned.

## 1. Regional and national systems (complementary)

| System | What it provides | Scale |
|---|---|---|
| ISRO/NRSC Landslide Atlas of India | Satellite-derived landslide inventory (~80,000 landslides, 17 states and 2 UTs, 1998-2022) and susceptibility and risk-exposure mapping | Regional |
| IMD MHEW-DSS | Multi-hazard weather forecasting and decision support | Regional |
| Forest Survey of India near-real-time fire monitoring | Satellite fire alerts (VIIRS, 375 m resolution) | Regional |
| CWC flood forecasting | River-level and dam/barrage inflow forecasts at forecasting stations | River basin |
| NDMA / SACHET | Multi-hazard alert dissemination | National |

TerraSense does not replace these systems. It is intended as a site-level ground-sensing layer that can add local measurements to regional information.

## 2. Ground-sensor systems (closest prior art)

| System | Summary |
|---|---|
| Amrita Landslide Early Warning System (Munnar, Kerala; Sikkim) | Wireless sensor network for rainfall-induced landslide warning. Uses sensors such as rain gauges, soil moisture, pore-pressure and tilt/vibration, and has issued warnings in Munnar since 2009. Much richer sensor suite than TerraSense. |
| Dryad Silvanet | Commercial solar-powered LoRaWAN mesh with gas sensors (hydrogen, carbon monoxide) and embedded AI for early wildfire detection, deployed at scale. |
| Ragnoli et al. (2020) | Low-power LoRa-based flood-monitoring system (flood only). |

## 3. How TerraSense differs

| Aspect | TerraSense | Status |
|---|---|---|
| Local risk assessment without internet | Gateway computes risk state and stores data locally (Raspberry Pi, SQLite, local dashboard) | Working prototype <!-- CHECK: demo with internet disconnected --> |
| Shared node/gateway design across hazards | Same ESP32 + LoRa architecture used for landslide and fire nodes | Working prototype (2 hazards) |
| Fault state | Nodes report a FAULT state distinct from SAFE/WATCH/WARNING/DANGER | In development <!-- CHECK: show a fault-injection demo or change to Planned --> |
| Solar power | Solar panel, charge controller and battery | In development |
| Self-healing LoRa mesh | Multi-hop relaying between nodes | Planned |
| ML-based risk classification and anomaly detection | To be developed after verified field data is collected | Planned |
| Flood and pollution nodes | Additional hazard modules on the same architecture | Planned |

Current risk states use rule-based multi-sensor logic. We make no claim of superior accuracy, range or cost until we have measured results.

## 4. Lessons we take from this work

- Established landslide warning systems rely on rainfall data as a primary trigger, so we plan to add a rain gauge that measures intensity rather than only wet/dry.
- Soil moisture at a single point is a limited indicator of slope stability, so our thresholds will be site-specific and calibrated against a baseline.
- Field deployments can be damaged or stolen, so we plan tamper detection and robust mounting.
- Commercial wildfire systems use gas sensing with on-device logic to reduce false alarms, so we plan persistence and multi-sensor confirmation before raising a high-level alert.
- Regional systems already cover forecasts and alert dissemination, so we plan to keep our alerts local first and later explore a standards-based link (for example CAP) into existing channels.

## 5. Regulatory note

India delicensed 865-867 MHz for short-range devices, and this is our target band for a deployable version. Our current prototype uses 433 MHz modules. A 2015 regulatory summary lists 433-434.79 MHz at 10 mW ERP with a 10 kHz channel bandwidth and a 10% duty-cycle limit, so we will verify current DoT rules before any deployment outside a lab.

## Sources

- ISRO/NRSC, Landslide Atlas of India: https://www.isro.gov.in/ISRO_EN/Landslide_Atlas_India.html
- IMD MHEW-DSS report: https://imdgeospatial.imd.gov.in/Resources/Multi%20Hazard%20Early%20warning%20DSS_Report_Jan2026.pdf
- Forest Survey of India, near-real-time fire monitoring: https://fsiforestfire.gov.in/Home/NRTDetails
- CWC, Flood Management in India, Statistical Report 2023: https://cwc.gov.in/sites/default/files/flood-management-india-statistical-report-2023.pdf
- Amrita Landslide Early Warning System: https://amrita.edu/project/landslide-early-warning-system
- Amrita, early warnings in Munnar (2020): https://www.amrita.edu/?p=105708
- Dryad Silvanet: https://dryad.net/silvanet
- ITU, Silvanet entry: https://www.itu.int/ew4all/solution/silvanet/
- Ragnoli et al. (2020), LoRa flood monitoring: https://doi.org/10.3390/jlpea10020015
- Rokhideh, Fearnley and Budimir (2025), multi-hazard early warning systems: https://doi.org/10.1007/s13753-025-00622-9
- DoT, delicensing notifications: https://dot.gov.in/sites/default/files/Regulation.pdf

## Team

**TerraSense Team**

For collaboration, deployment, or technical questions, open an issue in this repository or contact the project maintainers.

---
Our Email : terrasense2026@gmail.com

<div align="center">

### One Platform • Multiple Hazards • Local Intelligence

</div>
