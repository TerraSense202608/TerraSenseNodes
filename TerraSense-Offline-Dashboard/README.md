# 🌍 TerraSense

## Multi-Hazard Environmental Early Warning Network

> **Sense Locally • Analyze Intelligently • Alert Early**

TerraSense is a distributed environmental monitoring and early-warning platform designed to identify changing environmental conditions and provide localized warnings before a situation becomes critical.

The platform is designed around a common architecture consisting of **sensor nodes, ESP32 controllers, 433 MHz LoRa communication, local processing, intelligent risk assessment, and multi-level alerts**.

TerraSense is designed as a **multi-hazard system** covering:

- 🌍 Landslides
- 🌊 Floods
- 🔥 Forest Fires
- 🏭 Environmental Pollution

### Current Development Focus

The **current working prototype focuses on landslide monitoring and early warning**.

The other three hazard nodes — flood, forest fire, and pollution — represent the planned expansion of the same platform.

The objective is not to build four completely different systems, but to create **one common monitoring architecture that can support different hazards by changing the sensing layer and risk parameters.**

---

# 🎯 Problem

Environmental hazards are often preceded by measurable changes in surrounding conditions.

For example:

- Increasing soil moisture and rainfall can contribute to landslide risk.
- Rapidly increasing water levels can indicate flood risk.
- Increasing temperature, decreasing humidity and smoke can indicate forest-fire risk.
- Increasing particulate matter and gas concentrations can indicate pollution events.

However, monitoring these parameters individually does not always provide enough information to understand the overall risk.

Remote and vulnerable locations can also face unreliable internet connectivity and limited access to continuous monitoring.

TerraSense addresses this by combining **multiple sensor parameters, local processing, wireless communication and risk assessment** into a single distributed platform.

---

# 💡 Proposed Solution

TerraSense places specialized sensor nodes near areas that need continuous environmental monitoring.

Each node:

1. Collects environmental data.
2. Performs initial processing and filtering.
3. Extracts useful measurements and trends.
4. Sends summarized information using 433 MHz LoRa.
5. The gateway receives and processes the information.
6. Multiple parameters are combined to estimate risk.
7. The system generates a risk level.
8. Local and remote alerts are triggered when required.

## 🏗️ Core Architecture

TerraSense follows a distributed, edge-based architecture designed to monitor multiple environmental hazards using specialized sensor nodes and a common communication, processing, risk-assessment, and alert framework.

The **current working prototype focuses on landslide monitoring**, while the same architecture is designed to support **flood, forest fire, and environmental pollution** monitoring.

```mermaid
flowchart TB

    %% =========================
    %% HAZARD SENSOR NODES
    %% =========================

    subgraph NODES["🌍 HAZARD-SPECIFIC SENSOR NODES"]

        L["🌍 LANDSLIDE NODE<br/><br/>
        MPU6050<br/>
        Soil Moisture<br/>
        Rain Sensor<br/>
        HX711 + Load Cell<br/>
        ESP32"]

        F["🌊 FLOOD NODE<br/><br/>
        Water Level<br/>
        Rain Sensor<br/>
        Temperature<br/>
        Humidity<br/>
        ESP32"]

        FF["🔥 FOREST FIRE NODE<br/><br/>
        Temperature<br/>
        Humidity<br/>
        Smoke / PM<br/>
        Gas / VOC<br/>
        ESP32"]

        P["🏭 POLLUTION NODE<br/><br/>
        PM2.5 / PM10<br/>
        CO / Gas<br/>
        VOC<br/>
        NO₂ / SO₂<br/>
        Temperature / Humidity<br/>
        ESP32"]
    end

    %% =========================
    %% EDGE PROCESSING
    %% =========================

    subgraph EDGE["⚙️ EDGE PROCESSING — SENSOR NODE"]

        E1["Sensor Data Acquisition"]
        E2["Data Validation"]
        E3["Noise Filtering"]
        E4["Feature Extraction"]
        E5["Trend & Rate-of-Change Analysis"]

        E1 --> E2 --> E3 --> E4 --> E5
    end

    %% =========================
    %% COMMUNICATION
    %% =========================

    subgraph COMM["📡 COMMUNICATION LAYER"]

        LORA["SX1278 / RA-02<br/>
        433 MHz LoRa<br/><br/>
        Low Power • Long Range • Offline Communication"]
    end

    %% =========================
    %% GATEWAY
    %% =========================

    subgraph GATEWAY["🖥️ ESP32 GATEWAY"]

        G1["LoRa Packet Reception"]
        G2["Data Parsing & Validation"]
        G3["Multi-Node Data Aggregation"]
        G4["Hazard-Specific Processing"]

        G1 --> G2 --> G3 --> G4
    end

    %% =========================
    %% RISK ENGINE
    %% =========================

    subgraph RISK["🧠 INTELLIGENT RISK ASSESSMENT"]

        R1["Multi-Sensor Risk Fusion"]
        R2["Risk Score"]
        R3["ML-Based Risk Prediction<br/>(Development Stage)"]

        R1 --> R2
        R3 --> R2
    end

    %% =========================
    %% DECISION
    %% =========================

    subgraph DECISION["🚦 RISK DECISION"]

        SAFE["🟢 SAFE<br/>Normal Conditions"]
        WARNING["🟡 WARNING<br/>Increasing / Potential Risk"]
        DANGER["🔴 DANGER<br/>High Risk"]
    end

    %% =========================
    %% RESPONSE
    %% =========================

    subgraph RESPONSE["🚨 RESPONSE & MONITORING"]

        LOCAL["🔊 LOCAL ALERTS<br/><br/>
        LED Indicators<br/>
        Buzzer / Siren<br/>
        Voice Alert"]

        DASH["📊 LOCAL DASHBOARD<br/><br/>
        Sensor Readings<br/>
        Trends<br/>
        Risk Status<br/>
        Node Monitoring"]

        REMOTE["🌐 OPTIONAL REMOTE SERVICES<br/><br/>
        Notifications<br/>
        Authority Alerts<br/>
        Historical Analytics<br/>
        Cloud Services"]
    end

    %% =========================
    %% CONNECTIONS
    %% =========================

    L --> E1
    F --> E1
    FF --> E1
    P --> E1

    E5 --> LORA
    LORA --> G1

    G4 --> R1

    R2 --> SAFE
    R2 --> WARNING
    R2 --> DANGER

    SAFE --> LOCAL
    WARNING --> LOCAL
    DANGER --> LOCAL

    R2 --> DASH
    DASH -.-> REMOTE

    %% =========================
    %% STYLING
    %% =========================

    classDef landslide fill:#EDE9FE,stroke:#7C3AED,stroke-width:2px,color:#111827
    classDef flood fill:#DBEAFE,stroke:#2563EB,stroke-width:2px,color:#111827
    classDef fire fill:#FEE2E2,stroke:#DC2626,stroke-width:2px,color:#111827
    classDef pollution fill:#D1FAE5,stroke:#059669,stroke-width:2px,color:#111827

    classDef processing fill:#F8FAFC,stroke:#475569,stroke-width:2px,color:#111827
    classDef communication fill:#E0F2FE,stroke:#0284C7,stroke-width:2px,color:#111827
    classDef gateway fill:#E0E7FF,stroke:#4F46E5,stroke-width:2px,color:#111827
    classDef risk fill:#F3E8FF,stroke:#9333EA,stroke-width:2px,color:#111827
    classDef safe fill:#DCFCE7,stroke:#16A34A,stroke-width:2px,color:#111827
    classDef warning fill:#FEF3C7,stroke:#D97706,stroke-width:2px,color:#111827
    classDef danger fill:#FEE2E2,stroke:#DC2626,stroke-width:2px,color:#111827
    classDef response fill:#ECFEFF,stroke:#0891B2,stroke-width:2px,color:#111827

    class L landslide
    class F flood
    class FF fire
    class P pollution

    class E1,E2,E3,E4,E5 processing
    class LORA communication
    class G1,G2,G3,G4 gateway
    class R1,R2,R3 risk

    class SAFE safe
    class WARNING warning
    class DANGER danger

    class LOCAL,DASH,REMOTE response
