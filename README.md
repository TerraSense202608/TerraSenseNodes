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

## Team

**TerraSense Team**

For collaboration, deployment, or technical questions, open an issue in this repository or contact the project maintainers.

---

<div align="center">

### One Platform • Multiple Hazards • Local Intelligence

</div>
