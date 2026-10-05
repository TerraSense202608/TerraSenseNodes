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

## Table of Contents

- [Overview](#overview)
- [Project Status](#project-status)
- [Core Workflow](#core-workflow)
- [Why TerraSense](#why-terrasense)
- [Validated Prototypes](#validated-prototypes)
- [System Architecture](#system-architecture)
- [Key Capabilities](#key-capabilities)
- [Risk-State Model](#risk-state-model)
- [Technology Stack](#technology-stack)
- [Risk Assessment](#risk-assessment)
- [TerraSense ML Baselines](#terrasense-ml-baselines)
- [Resilience and Fault Handling](#resilience-and-fault-handling)
- [Expansion Modules](#expansion-modules)
- [Deployment Roadmap](#deployment-roadmap)
- [Local Development](#local-development)
- [Repository Structure](#repository-structure)
- [Cost Estimation](#cost-estimation)
- [Current Scope and Limitations](#current-scope-and-limitations)
- [Impact](#impact)
- [Related Work and Prior Art](#related-work-and-prior-art)
- [Regulatory Considerations](#regulatory-considerations)
- [Sources](#sources)
- [Contributing](#contributing)
- [Team](#team)

---

## Overview

TerraSense is a distributed environmental-monitoring platform built around:

- Hazard-specific sensor nodes.
- Low-power LoRa communication.
- A local edge gateway.
- Site-specific risk assessment.
- Offline-first data storage and alerts.

The platform is designed for remote and vulnerable locations where continuous internet connectivity cannot be guaranteed.

TerraSense converts field-level measurements into localized risk states and actionable warnings while keeping essential monitoring and alerting functions at the edge.

## Project Status

| Capability | Status |
|---|---|
| Landslide monitoring node | Validated prototype |
| Forest-fire monitoring node | Validated prototype |
| ESP32 sensor acquisition | Implemented |
| 433 MHz LoRa communication | Implemented in prototype |
| Local gateway processing | Implemented in prototype |
| SQLite local storage | Implemented in prototype |
| Live dashboard monitoring | Implemented in prototype |
| Rule-based risk assessment | Current operational engine |
| Solar-powered field deployment | Deployment configuration |
| Flood monitoring module | Planned expansion |
| Pollution monitoring module | Planned expansion |
| Edge-ML inference | Research and validation roadmap |
| Self-healing LoRa mesh | Future network enhancement |

## Core Workflow

```text
SENSOR NODES
      ↓
LoRa COMMUNICATION
      ↓
LOCAL EDGE GATEWAY
      ↓
DATA VALIDATION AND AGGREGATION
      ↓
RISK ASSESSMENT
      ↓
LOCAL ALERTS + DATA STORAGE
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

TerraSense is not intended to replace national or regional monitoring systems. It is designed to add a localized ground-sensing and last-mile response layer.

## Validated Prototypes

### Landslide Node

```text
Tilt -  Soil Moisture -  Rainfall -  Load
ESP32 + LoRa + Local Risk State
```

The landslide node supports site-level monitoring of slope-related environmental conditions.

### Forest-Fire Node

```text
Flame -  Smoke/Gas -  Temperature
ESP32 + LoRa + Local Monitoring
```

The forest-fire node supports localized monitoring of fire-related conditions.

### Prototype Foundation

```text
ESP32 Sensor Nodes
433 MHz LoRa
Local Gateway
SQLite Storage
Live Dashboard
```

> Flood and pollution monitoring are planned expansion modules, not current validated prototypes.

┌─────────────────────────────────────────────┐
│           HAZARD-SPECIFIC SENSOR NODES      │
│ Landslide • Forest Fire • Flood* • Pollution*│
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
│       Packet validation • aggregation        │
│              Local data storage              │
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│                  RISK ENGINE                 │
│  Sensor fusion • trends • persistence        │
│      Validation • risk classification        │
└──────────────────────┬──────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────┐
│              DASHBOARD AND ALERTS            │
│       LED • Buzzer • Dashboard • SMS         │
└─────────────────────────────────────────────┘

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
- Sensor-health and communication monitoring.
- Site-specific threshold configuration.
- Local operation during internet outages.

## Risk-State Model

```text
SAFE → WATCH → WARNING → DANGER
                     ↘
                     FAULT
```

| State | Meaning |
|---|---|
| `SAFE` | Valid sensor data indicates normal conditions |
| `WATCH` | Early deviation or developing concern |
| `WARNING` | Persistent or multi-sensor abnormality |
| `DANGER` | Strong evidence of hazardous conditions |
| `FAULT` | Sensor, packet, power, or communication data is unreliable |

`FAULT` indicates that the data cannot be trusted. It must not be interpreted as a `SAFE` condition.

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

The current operational risk engine is rule-based. It is intended to provide explainable risk states using sensor conditions, trends, persistence, and validation.

### Planned Edge-ML Roadmap

- Isolation Forest for anomaly detection.
- Random Forest for risk classification.
- Time-series trend analysis.
- Camera and thermal-sensor fusion.

Machine learning will be introduced after site calibration and collection of verified field data.

# TerraSense ML Baselines

The `ml-baseline/` directory contains two small machine-learning baselines for TerraSense. They demonstrate the intended pipeline:

```text
DATA → FEATURES → MODEL → RISK / ANOMALY OUTPUT
```

> **Important:** Both models are trained on public datasets to demonstrate the pipeline. They are not trained on TerraSense sensor data and are not currently running on TerraSense nodes. The operational risk engine today is rule-based. For deployment, the models will be retrained using data collected from TerraSense nodes.

## ML Baseline Contents

| File | Purpose |
|---|---|
| `fire_random_forest.py` | Fire/smoke baseline using Random Forest classification |
| `landslide_isolation_forest.py` | Landslide baseline using Isolation Forest anomaly detection |
| `check_random_split.py` | Compares time-ordered and random splits for the fire dataset |
| `landslide_anomaly_plot.png` | Landslide anomaly scores on the held-out period |
| `fire_prediction_plot.png` | Fire-model probabilities on held-out readings |
| `screenshots/` | Terminal outputs from model runs |

## Fire Baseline: Random Forest

- **Dataset:** Smoke Detection Dataset by Blattmann, available on [Kaggle](https://www.kaggle.com/datasets/deepcontractor/smoke-detection-dataset).
- **Dataset size:** 62,630 readings.
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
- **Missed alarm readings:** 1,032.
- **False alarms:** 135.

A random split produces approximately 1.00 accuracy because neighbouring readings are highly similar and can leak information between training and testing. The time-ordered result is therefore the primary reported result.

### Fire Baseline Limitations

- The public dataset uses sensors different from TerraSense hardware.
- The flame sensor is not represented.
- The data comes from a limited number of recording sessions.
- The result is not TerraSense accuracy.
- The model is not currently deployed on TerraSense nodes.

## Landslide Baseline: Isolation Forest

- **Dataset:** Pukrongta et al. (2025), [Landslide Monitoring Dataset — Mendeley Data](https://doi.org/10.17632/9w43sg73bt.1).
- **Dataset size:** Approximately 40,600 readings from two ESP32 nodes.
- **Collection period:** 13 March–15 April 2025.
- **Features:** Tilt, soil moisture at 20/40/60 cm, soil-moisture rate of change, rainfall, vibration, temperature, and humidity.
- **Cleaning:** Start-up rows where temperature, humidity, or all soil sensors were zero were removed.
- **Split:** First 70% of each node’s readings for training and final 30% for testing.
- **Method:** Isolation Forest learns normal patterns and flags unusual readings.

The dataset contains no verified event labels. Therefore, this is an unsupervised anomaly-detection baseline and no accuracy score is reported.

### Held-Out Output

| Node | Test readings | Flagged |
|---|---:|---:|
| Node 1 | 5,216 | 373 (7.2%) |
| Node 2 | 6,965 | 8 (0.1%) |

These flags demonstrate a functioning anomaly-detection pipeline, not confirmed landslide detection. Node 1’s test period differs from its training period, including a shift in soil moisture, which explains most of its flagged readings. The test period contains very few rainfall or vibration events, so the results cannot be verified against actual landslides.

## Running the ML Baselines

1. Download the two datasets.
2. Place them in the `ml-baseline/` directory.
3. Use the following filenames:
   - `smoke_detection_iot.csv`
   - `Raw_Sensor_Data_Landslide_Monitoring__1_.xlsx`
4. If the filenames differ, update the `FILE` variable in the relevant script.
5. Install the dependencies:

```bash
python -m pip install pandas scikit-learn matplotlib openpyxl
```

6. Run the scripts:

```bash
python fire_random_forest.py
python landslide_isolation_forest.py
python check_random_split.py
```

## ML Roadmap

1. Add sensor-data logging to the dashboard.
2. Collect normal and abnormal readings from TerraSense nodes.
3. Calibrate sensors and establish site-specific baselines.
4. Retrain both models using TerraSense data.
5. Validate using time-ordered field data.
6. Run inference on the Raspberry Pi gateway.
7. Retain the rule-based engine as a fallback.

---

## Resilience and Fault Handling

TerraSense is designed to maintain essential local operation during internet outages.

```text
Internet Failure
→ Local Processing Continues
→ Local Alerts Continue
→ Data Is Stored Locally
→ Synchronization Can Occur Later
```

The current prototype uses a LoRa point-to-point or star communication model.

A self-healing multi-hop LoRa mesh is a future enhancement and is not claimed as part of the current implementation.

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

## Deployment Roadmap

```text
Phase 1 — Validated Prototype
ESP32 + 433 MHz LoRa + Raspberry Pi 4

Phase 2 — Field Pilot
Calibrated sensors + solar power + weather-resistant enclosure

Phase 3 — Commercial Architecture
IN865 LoRa + Qualcomm Dragonwing RB3 Gen 2 edge platform

Phase 4 — Advanced Intelligence
Edge ML + camera/thermal fusion + multi-site monitoring
```

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
├── ml-baseline/
│   ├── fire_random_forest.py
│   ├── landslide_isolation_forest.py
│   ├── check_random_split.py
│   ├── screenshots/
│   └── README.md
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
- Solar field deployment requires installation and power-system validation.
- ML baselines use public datasets and are not deployment models.
- Deployment models require verified TerraSense field data.
- The landslide baseline has no verified event labels.
- Public-dataset results must not be interpreted as TerraSense accuracy.
- TerraSense provides risk assessment and early warning, not guaranteed disaster prediction.
- Accuracy, range, and cost claims will be reported only after controlled field testing.

## Impact

TerraSense supports:

- Localized hazard monitoring.
- Faster field verification.
- Offline-capable warning delivery.
- Shared infrastructure across hazards.
- Modular maintenance and sensor replacement.
- Reduced dependence on continuous cloud connectivity.
- A scalable path from prototype nodes to multi-site monitoring.

## Related Work and Prior Art

This comparison is based on publicly available information. TerraSense is a prototype-stage project; differences are stated only where they can be demonstrated.

### Regional and National Systems

| System | What it provides | Scale |
|---|---|---|
| ISRO/NRSC Landslide Atlas of India | Satellite-derived landslide inventory, susceptibility, and risk-exposure mapping | Regional |
| IMD MHEW-DSS | Multi-hazard weather forecasting and decision support | Regional |
| Forest Survey of India near-real-time fire monitoring | Satellite fire alerts using VIIRS at 375 m resolution | Regional |
| Central Water Commission flood forecasting | River-level and dam/barrage inflow forecasts at forecasting stations | River basin |
| NDMA SACHET | Multi-hazard alert dissemination | National |

TerraSense does not replace these systems. It is intended as a site-level ground-sensing layer that can add local measurements to regional environmental intelligence.

### Ground-Sensor Systems

| System | Summary |
|---|---|
| Amrita Landslide Early Warning System | Wireless sensing for rainfall-induced landslide warning using rainfall, soil moisture, pore pressure, tilt, and vibration measurements |
| Dryad Silvanet | Commercial solar-powered LoRaWAN wildfire-monitoring system with gas sensors and embedded intelligence |
| Ragnoli et al. (2020) | Low-power LoRa-based flood-monitoring system focused on flood detection |

### TerraSense Differentiators

| Aspect | TerraSense approach | Status |
|---|---|---|
| Local risk assessment without internet | Gateway computes risk state and stores data locally using Raspberry Pi, SQLite, and a local dashboard | Working prototype |
| Shared node/gateway design across hazards | Common ESP32 and LoRa architecture used for landslide and forest-fire nodes | Working prototype |
| Fault state | Separate `FAULT` state for invalid or unavailable data | In development |
| Solar power | Solar panel, charge controller, and battery | In development |
| Self-healing LoRa mesh | Multi-hop relaying and route recovery | Planned |
| ML-based risk assessment | Development after verified field-data collection | Planned |
| Flood and pollution nodes | Additional hazard modules using shared infrastructure | Planned |

TerraSense makes no claim of superior accuracy, range, or cost until these metrics are measured through controlled field testing.

### Design Lessons

- Established landslide warning systems rely on rainfall data as a primary trigger, so TerraSense plans to add rainfall intensity measurement rather than only wet/dry detection.
- Soil moisture at a single point is a limited indicator of slope stability, so thresholds must be site-specific and baseline-calibrated.
- Field deployments can be damaged or stolen, so future versions should include tamper detection and robust mounting.
- Commercial wildfire systems use gas sensing with on-device logic to reduce false alarms, so TerraSense plans persistence and multi-sensor confirmation before raising a high-level alert.
- Regional systems already cover forecasting and alert dissemination, so TerraSense is designed to keep alerts local first and may later explore standards-based integration such as CAP.

## Regulatory Considerations

The current prototype uses 433 MHz LoRa hardware. The planned deployable version targets the IN865 band.

Frequency allocation, transmit power, channel bandwidth, duty cycle, and other radio requirements must be verified against current Department of Telecommunications rules before field deployment.

## Sources

- [ISRO/NRSC — Landslide Atlas of India](https://www.isro.gov.in/ISRO_EN/Landslide_Atlas_India.html)
- [IMD — Multi-Hazard Early Warning Decision Support System](https://imdgeospatial.imd.gov.in/Resources/Multi%20Hazard%20Early%20warning%20DSS_Report_Jan2026.pdf)
- [Forest Survey of India — Near-Real-Time Fire Monitoring](https://fsiforestfire.gov.in/Home/NRTDetails)
- [Central Water Commission — Flood Management in India](https://cwc.gov.in/sites/default/files/flood-management-india-statistical-report-2023.pdf)
- [Amrita — Landslide Early Warning System](https://amrita.edu/project/landslide-early-warning-system)
- [Amrita — Early Warnings in Munnar](https://www.amrita.edu/?p=105708)
- [Dryad — Silvanet](https://dryad.net/silvanet)
- [ITU — Silvanet](https://www.itu.int/ew4all/solution/silvanet/)
- [Ragnoli et al. — LoRa Flood Monitoring](https://doi.org/10.3390/jlpea10020015)
- [Pukrongta et al. — Landslide Monitoring Dataset](https://doi.org/10.17632/9w43sg73bt.1)
- [Smoke Detection Dataset — Kaggle](https://www.kaggle.com/datasets/deepcontractor/smoke-detection-dataset)
- [DoT — Regulatory Information](https://dot.gov.in/sites/default/files/Regulation.pdf)

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

For hardware, sensing, communication, or ML changes, include:

- Hardware configuration.
- Sensor details.
- Test conditions.
- Calibration information.
- Communication logs.
- Reproduction steps.
- Known limitations.

## Team

**TerraSense Team**

For collaboration, deployment, or technical questions, open an issue in this repository or contact the project maintainers.

**Email:** [terrasense2026@gmail.com](mailto:terrasense2026@gmail.com)

---

<div align="center">

### One Platform • Multiple Hazards • Local Intelligence

</div>
