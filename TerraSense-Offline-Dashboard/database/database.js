const Database = require("better-sqlite3");
const path = require("path");
const { app } = require("electron");

const dbPath = path.join(app.getPath("userData"), "terrasense.db");

const db = new Database(dbPath);

// =====================================================
// SENSOR DATA TABLE
// One row per LoRa packet received by the Gateway.
// Covers both node types (landslide + fire); a column
// that does not apply to a given node_type is left NULL.
// =====================================================

db.exec(`
    CREATE TABLE IF NOT EXISTS sensor_data (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        node_id TEXT NOT NULL,
        node_type TEXT NOT NULL DEFAULT 'landslide',
        timestamp TEXT NOT NULL,
        status TEXT,

        -- Landslide node (NODE1)
        tiltX REAL,
        tiltY REAL,
        maxTilt REAL,
        soilRaw INTEGER,
        soilWet TEXT,
        rainRaw INTEGER,
        rainDetected TEXT,
        hx711Raw INTEGER,
        accX REAL,
        accY REAL,
        accZ REAL,
        gyroX REAL,
        gyroY REAL,
        gyroZ REAL,

        -- Fire node (FIRE01)
        humidity REAL,
        smoke INTEGER,
        gas INTEGER,
        flame INTEGER,
        pressure REAL,

        -- Shared
        temperature REAL,
        rssi INTEGER,
        snr REAL
    )
`);

console.log("SQLite database:", dbPath);

module.exports = db;
