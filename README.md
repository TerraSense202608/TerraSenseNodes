<div align="center">

# TerraSense

### Local Edge Intelligence for Multi-Hazard Environmental Monitoring

<p>
  A modular, offline-first platform for localized hazard detection,
  risk assessment, and early warning in remote and low-connectivity environments.
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

TerraSense is a modular environmental-monitoring platform that combines distributed sensor nodes, LoRa communication, local edge processing, and offline-first alerts.

It is designed to provide **site-specific hazard intelligence** for locations where continuous internet connectivity may be unavailable or unreliable.

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

Large-scale systems such as satellite and GIS-based monitoring platforms provide valuable regional environmental intelligence. TerraSense complements them with a low-cost, ground-level edge layer for continuous local sensing and immediate field response.

| Existing monitoring | TerraSense |
|---|---|
| Regional or area-level observation | Site-level ground sensing |
| Centralized processing | Local edge risk assessment |
| Institutional warning platforms | Direct field alerts |
| Connectivity-dependent remote access | Offline-first local operation |
| Separate hazard systems | One modular multi-hazard platform |

## Validated Prototypes

### Landslide Node

```text
Tilt -  Soil Moisture -  Rainfall -  Load
ESP32 + LoRa + Local Risk State
```

Provides site-level slope-condition monitoring.

### Forest-Fire Node

```text
Flame -  Smoke/Gas -  Temperature
ESP32 + LoRa + Local Monitoring
```

Provides localized fire-condition monitoring.

### Prototype Foundation

```text
ESP32 Sensor Nodes
433 MHz LoRa
Offline Gateway
SQLite Local Storage
Live Dashboard
```

> Flood and pollution monitoring are planned expansion modules, not current validated prototypes.

## System Architecture

```text
┌────────────────────────────────────────────┐
│          HAZARD-SPECIFIC SENSOR NODES      │
│  Landslide -  Forest Fire -  Flood -  Pollution│
└──────────────────────┬─────────────────────┘
                       │
                       ▼
┌────────────────────────────────────────────┐
│              LoRa COMMUNICATION            │
│       Low-power local wireless transfer     │
└──────────────────────┬─────────────────────┘
                       │
                       ▼
┌────────────────────────────────────────────┐
│             LOCAL EDGE GATEWAY             │
│  Packet validation -  aggregation -  storage  │
└──────────────────────┬─────────────────────┘
                       │
                       ▼
┌────────────────────────────────────────────┐
│              RISK ASSESSMENT ENGINE        │
│ Fusion -  trends -  persistence -  validation  │
└──────────────────────┬─────────────────────┘
                       │
                       ▼
┌────────────────────────────────────────────┐
│              ALERT AND DASHBOARD LAYER      │
│ LED -  buzzer -  dashboard -  SMS -  voice      │
└────────────────────────────────────────────┘
```

## Key Capabilities

- Distributed ESP32-based sensing.
- Low-power LoRa communication.
- Local data validation and multi-node aggregation.
- Edge-based risk assessment.
- Local SQLite data storage.
- Offline dashboard operation.
- Local LED and buzzer alerts.
- Optional remote SMS and voice alerts.
- Hazard-specific sensor modules.
- Common risk-state representation.

### Risk States

```text
SAFE → WATCH → WARNING → DANGER
                     ↘
                     FAULT
```

`FAULT` indicates missing, invalid, or unreliable sensor or communication data. It should not be interpreted as a safe environmental condition.

## Technology Stack

| Layer | Technology |
|---|---|
| Sensor controller | ESP32 |
| Wireless communication | SX1278 / RA-02 LoRa, 433 MHz prototype |
| Prototype gateway | Raspberry Pi 4 |
| Gateway OS | Raspberry Pi OS |
| Gateway processing | Python |
| Data format | JSON |
| Local database | SQLite |
| Dashboard | React / local web interface |
| Risk engine | Rule-based edge assessment |
| Future AI/ML | Anomaly detection and risk classification |
| Power | Solar PV + Li-ion battery |
| Enclosure | Weather-resistant field enclosure |

## Risk-Assessment Pipeline

TerraSense does not depend only on a single threshold. The local risk engine can combine:

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

The proposed machine-learning roadmap includes:

- Isolation Forest for anomaly detection.
- Random Forest for risk classification.
- Time-series trend analysis.
- Future camera and thermal-sensor fusion.

Machine learning should be introduced after field calibration and collection of verified ground-truth data.

## Resilience and Fault Handling

TerraSense is designed to continue essential local operation when external connectivity is unavailable.

```text
Internet Failure
→ Local Processing Continues
→ Local Alerts Continue
→ Data Is Buffered Locally
→ Synchronization Can Occur Later
```

The prototype uses a LoRa star or point-to-point communication model. A self-healing multi-hop LoRa mesh is a future enhancement and should not be claimed as implemented unless alternate-path routing and automatic recovery have been demonstrated.

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

### Flood

```text
Water Level -  Rate of Rise -  Rainfall
```

### Pollution

```text
PM2.5 -  PM10 -  Gas Concentration -  Air Quality
```

Both modules use the same gateway, communication, storage, dashboard, and risk-assessment infrastructure.

## Local Development

### Prerequisites

- Python 3.10+
- Node.js 18+
- Raspberry Pi OS for gateway deployment
- ESP32 development environment
- LoRa hardware compatible with the prototype
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

> Replace commands and paths with the exact commands supported by the repository.

## Suggested Repository Structure

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

These are indicative estimates and vary according to sensor accuracy, enclosure, solar capacity, gateway hardware, installation, and calibration.

## Current Scope and Limitations

- Landslide and forest-fire modules are the validated prototypes.
- Flood and pollution are expansion modules.
- Current communication uses 433 MHz LoRa hardware.
- The current network is not yet a self-healing mesh.
- Risk thresholds require site-specific calibration.
- ML models require verified field data before deployment.
- The system supports risk assessment, not guaranteed disaster prediction.

## Impact

TerraSense supports:

- Localized hazard monitoring.
- Faster field verification.
- Offline-capable warning delivery.
- Shared infrastructure across hazards.
- Modular maintenance and sensor replacement.
- Lower dependence on continuous cloud connectivity.

## Contributing

1. Fork the repository.
2. Create a feature branch.

```bash
git checkout -b feature/your-feature
```

3. Commit your changes.

```bash
git commit -m "Add your feature"
```

4. Push the branch.

```bash
git push origin feature/your-feature
```

5. Open a pull request.

Please include hardware details, test conditions, logs, and calibration information when submitting changes related to sensing or communication.



## Team

**TerraSense Team**

For collaboration, deployment, or technical questions, open an issue in this repository or contact the project maintainers.

---

<div align="center">

### One Platform • Multiple Hazards • Local Intelligence

</div>
