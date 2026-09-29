<div align="center">

# 🌍 TerraSense

### *Resilient Multi-Hazard Environmental Early Warning Network*

**Sense Locally • Analyze Intelligently • Alert Early**

<p>
  <img src="https://img.shields.io/badge/SIH-2026-blue?style=for-the-badge" alt="SIH 2026"/>
  <img src="https://img.shields.io/badge/PS-SIH26178-orange?style=for-the-badge" alt="SIH26178"/>
  <img src="https://img.shields.io/badge/Domain-Disaster%20Management-red?style=for-the-badge" alt="Disaster Management"/>
  <img src="https://img.shields.io/badge/Platform-ESP32-green?style=for-the-badge&logo=espressif" alt="ESP32"/>
  <img src="https://img.shields.io/badge/Communication-LoRa-blueviolet?style=for-the-badge" alt="LoRa"/>
  <img src="https://img.shields.io/badge/Operation-Offline--First-0f766e?style=for-the-badge" alt="Offline First"/>
</p>

<p>
  <strong>Hardware | Edge Computing | LoRa Communication | Local Intelligence | Environmental Monitoring</strong>
</p>

</div>

---

# 🌐 Overview

**TerraSense** is a modular multi-hazard environmental early warning platform designed for deployment in vulnerable and connectivity-constrained regions.

The system combines:

- 🌱 Ground-level environmental sensing
- 📡 Long-range LoRa communication
- 🖥️ Local gateway processing
- 💾 Offline data storage
- 📊 Real-time local monitoring
- 🚨 Localized warning states
- 🤖 AI/ML-based risk assessment as the next development layer

Unlike systems that depend entirely on continuous cloud connectivity, TerraSense is designed around an **offline-first core architecture**.

The fundamental monitoring path can operate locally:

```text
SENSORS
   ↓
ESP32 SENSOR NODE
   ↓
LoRa COMMUNICATION
   ↓
LOCAL GATEWAY
   ↓
LOCAL PROCESSING
   ↓
RISK ASSESSMENT
   ↓
LOCAL ALERTS + DASHBOARD
