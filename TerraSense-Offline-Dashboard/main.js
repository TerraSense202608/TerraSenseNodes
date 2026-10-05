const {
    app,
    BrowserWindow,
    ipcMain,
    powerSaveBlocker
} = require("electron");

const path = require("path");
const fs = require("fs");

let db;
let mainWindow;

const { SerialPort } = require("serialport");
const { ReadlineParser } = require("@serialport/parser-readline");

let serialPort = null;

// =====================================================
// CONTINUOUS-RUN CSV LOGGING
// A new pair of files is created each time the app starts.
// Timestamps are the PC clock, in UTC (ISO format).
// counter = -1 means the node did not send a packet counter.
// =====================================================

const LOG_DIR = path.join(__dirname, "logs");
let packetLogFile = null;
let badLogFile = null;

function fileStamp() {
    const d = new Date();
    const p = (n) => String(n).padStart(2, "0");
    return (
        `${d.getFullYear()}${p(d.getMonth() + 1)}${p(d.getDate())}_` +
        `${p(d.getHours())}${p(d.getMinutes())}${p(d.getSeconds())}`
    );
}

function initLogFiles() {
    try {
        fs.mkdirSync(LOG_DIR, { recursive: true });

        const s = fileStamp();
        packetLogFile = path.join(LOG_DIR, `gateway_log_${s}.csv`);
        badLogFile = path.join(LOG_DIR, `gateway_bad_${s}.csv`);

        fs.writeFileSync(
            packetLogFile,
            "timestamp,node,type,status,counter,rssi,snr,gw_ms\n"
        );
        fs.writeFileSync(
            badLogFile,
            "timestamp,gw_ms,fields,rssi,snr\n"
        );

        console.log("Logging packets to:", packetLogFile);

    } catch (error) {
        console.error("Could not create log files:", error);
        packetLogFile = null;
        badLogFile = null;
    }
}

function csvCell(value) {
    if (value === undefined || value === null) return "";
    const s = String(value);
    return /[",\n]/.test(s) ? `"${s.replace(/"/g, '""')}"` : s;
}

function logPacket(data, nodeType) {
    if (!packetLogFile) return;

    try {
        const row = [
            new Date().toISOString(),
            data.node_id,
            nodeType,
            data.status,
            numOrNull(data.counter),
            numOrNull(data.rssi),
            numOrNull(data.snr),
            numOrNull(data.gw_ms)
        ].map(csvCell).join(",") + "\n";

        fs.appendFileSync(packetLogFile, row);

    } catch (error) {
        console.error("Packet log error:", error.message);
    }
}

function logBadPacket(message) {
    if (!badLogFile) return;

    try {
        // Gateway format: BAD,<gw_ms>,<fieldCount>,<rssi>,<snr>
        const parts = message.split(",").slice(1);

        const row = [
            new Date().toISOString(),
            parts[0],
            parts[1],
            parts[2],
            parts[3]
        ].map(csvCell).join(",") + "\n";

        fs.appendFileSync(badLogFile, row);

    } catch (error) {
        console.error("Bad-packet log error:", error.message);
    }
}

// =====================================================
// WINDOW
// =====================================================

function createWindow() {

    mainWindow = new BrowserWindow({
        width: 1400,
        height: 900,
        minWidth: 1000,
        minHeight: 700,

        webPreferences: {
            preload: path.join(__dirname, "preload.js"),
            contextIsolation: true,
            nodeIntegration: false
        }
    });

    mainWindow.loadFile(path.join(__dirname, "index.html"));

    mainWindow.on("closed", () => {
        mainWindow = null;
    });
}

// =====================================================
// SERIAL PORT (with auto-reconnect)
// =====================================================

let reconnectTimer = null;
let lastPortName = "COM7";
let lastBaud = 115200;

function scheduleReconnect() {

    if (reconnectTimer) return;

    console.log("Gateway disconnected. Retrying in 5 s...");

    reconnectTimer = setTimeout(() => {
        reconnectTimer = null;
        connectSerialPort(lastPortName, lastBaud);
    }, 5000);
}

async function connectSerialPort(portName, baudRate = 115200) {

    try {

        lastPortName = portName;
        lastBaud = baudRate;

        if (reconnectTimer) {
            clearTimeout(reconnectTimer);
            reconnectTimer = null;
        }

        if (serialPort && serialPort.isOpen) {
            serialPort.close();
        }

        const port = new SerialPort({
            path: portName,
            baudRate: baudRate
        });

        serialPort = port;

        port.on("open", () => {

            console.log(
                `Gateway connected: ${portName} @ ${baudRate}`
            );

        });

        // The Gateway prints a banner + debug lines and then one
        // JSON line per LoRa packet (and a BAD, line for packets it
        // could not parse). Everything else is just logged.
        const parser = port.pipe(
            new ReadlineParser({
                delimiter: "\n"
            })
        );

        parser.on("data", (line) => {

            const message = line.trim();

            if (!message) return;

            console.log("Gateway:", message);

            if (
                message.startsWith("{") &&
                message.endsWith("}")
            ) {
                processSensorData(message);
            } else if (message.startsWith("BAD,")) {
                logBadPacket(message);
            }

        });

        port.on("error", (error) => {

            console.error(
                "Serial error:",
                error.message
            );

            // Only retry if this is still the current port object
            if (serialPort === port) {
                scheduleReconnect();
            }

        });

        port.on("close", () => {

            if (serialPort === port) {
                scheduleReconnect();
            }

        });

        return {
            success: true,
            message: "Serial port connected"
        };

    } catch (error) {

        console.error(
            "Serial connection error:",
            error
        );

        return {
            success: false,
            error: error.message
        };

    }
}

// =====================================================
// PROCESS ONE JSON PACKET FROM THE GATEWAY
// =====================================================

function processSensorData(message) {

    try {

        const data = JSON.parse(message);

        const nodeType =
            data.node_type ||
            (String(data.node_id || "").toUpperCase().startsWith("FIRE")
                ? "fire"
                : "landslide");

        // Log first, so a database problem can never stop the log.
        logPacket(data, nodeType);

        const sensorData = {

            node_id: data.node_id || "UNKNOWN",
            node_type: nodeType,

            timestamp:
                data.timestamp ||
                new Date().toISOString(),

            status: data.status || "SAFE",

            // Landslide fields
            tiltX: numOrNull(data.tiltX),
            tiltY: numOrNull(data.tiltY),
            maxTilt: numOrNull(data.maxTilt),
            soilRaw: numOrNull(data.soilRaw),
            soilWet: data.soilWet ?? null,
            rainRaw: numOrNull(data.rainRaw),
            rainDetected: data.rainDetected ?? null,
            hx711Raw: numOrNull(data.hx711Raw),
            accX: numOrNull(data.accX),
            accY: numOrNull(data.accY),
            accZ: numOrNull(data.accZ),
            gyroX: numOrNull(data.gyroX),
            gyroY: numOrNull(data.gyroY),
            gyroZ: numOrNull(data.gyroZ),

            // Fire fields (FIRE01: flame, mq2Raw, temperature, pressure -
            // no humidity sensor on this node; smoke/gas both mirror
            // the single MQ-2 reading, see Gateway.ino)
            humidity: numOrNull(data.humidity),
            smoke: numOrNull(data.smoke),
            gas: numOrNull(data.gas),
            flame: data.flame === true || data.flame === 1 ? 1 : 0,
            pressure: numOrNull(data.pressure),

            // Shared
            temperature: numOrNull(data.temperature),
            rssi: numOrNull(data.rssi),
            snr: numOrNull(data.snr)

        };

        insertSensorRow(sensorData);

        console.log("Saved sensor data:", sensorData);

        // Push straight to the dashboard, live.
        if (mainWindow && !mainWindow.isDestroyed()) {
            mainWindow.webContents.send("sensor-data", sensorData);
        }

    } catch (error) {

        console.error(
            "Invalid Gateway data:",
            message,
            error
        );

    }
}

function numOrNull(value) {
    if (value === undefined || value === null || value === "") return null;
    const n = Number(value);
    return Number.isNaN(n) ? null : n;
}

function insertSensorRow(sensorData) {

    const insert = db.prepare(`
        INSERT INTO sensor_data
        (
            node_id, node_type, timestamp, status,
            tiltX, tiltY, maxTilt, soilRaw, soilWet,
            rainRaw, rainDetected, hx711Raw,
            accX, accY, accZ, gyroX, gyroY, gyroZ,
            humidity, smoke, gas, flame, pressure,
            temperature, rssi, snr
        )
        VALUES (
            @node_id, @node_type, @timestamp, @status,
            @tiltX, @tiltY, @maxTilt, @soilRaw, @soilWet,
            @rainRaw, @rainDetected, @hx711Raw,
            @accX, @accY, @accZ, @gyroX, @gyroY, @gyroZ,
            @humidity, @smoke, @gas, @flame, @pressure,
            @temperature, @rssi, @snr
        )
    `);

    return insert.run(sensorData);
}

// =====================================================
// APP START
// =====================================================

app.whenReady().then(() => {

    // Stop the PC from suspending during long runs
    powerSaveBlocker.start("prevent-app-suspension");

    // Load SQLite
    db = require("./database/database");

    // Start CSV logging
    initLogFiles();

    // Create window
    createWindow();

    // Automatically connect to TerraSense Gateway
    setTimeout(() => {
        connectSerialPort("COM7", 115200);
    }, 1000);

    app.on("activate", () => {

        if (BrowserWindow.getAllWindows().length === 0) {
            createWindow();
        }

    });

});

ipcMain.handle(
    "connect-serial",
    async (event, { port, baudRate }) => {

        return await connectSerialPort(
            port,
            baudRate
        );

    }
);

ipcMain.handle(
    "list-serial-ports",
    async () => {

        try {

            const ports =
                await SerialPort.list();

            return ports.map(port => ({
                path: port.path,
                manufacturer:
                    port.manufacturer || "Unknown"
            }));

        } catch (error) {

            console.error(
                "Port listing error:",
                error
            );

            return [];

        }

    }
);

ipcMain.handle("save-sensor-data", (event, data) => {

    try {

        const result = insertSensorRow({
            node_id: data.node_id || "UNKNOWN",
            node_type: data.node_type || "landslide",
            timestamp: data.timestamp || new Date().toISOString(),
            status: data.status || "SAFE",
            tiltX: numOrNull(data.tiltX),
            tiltY: numOrNull(data.tiltY),
            maxTilt: numOrNull(data.maxTilt),
            soilRaw: numOrNull(data.soilRaw),
            soilWet: data.soilWet ?? null,
            rainRaw: numOrNull(data.rainRaw),
            rainDetected: data.rainDetected ?? null,
            hx711Raw: numOrNull(data.hx711Raw),
            accX: numOrNull(data.accX),
            accY: numOrNull(data.accY),
            accZ: numOrNull(data.accZ),
            gyroX: numOrNull(data.gyroX),
            gyroY: numOrNull(data.gyroY),
            gyroZ: numOrNull(data.gyroZ),
            humidity: numOrNull(data.humidity),
            smoke: numOrNull(data.smoke),
            gas: numOrNull(data.gas),
            flame: data.flame === true || data.flame === 1 ? 1 : 0,
            pressure: numOrNull(data.pressure),
            temperature: numOrNull(data.temperature),
            rssi: numOrNull(data.rssi),
            snr: numOrNull(data.snr)
        });

        return {
            success: true,
            id: result.lastInsertRowid
        };

    } catch (error) {

        console.error("SQLite insert error:", error);

        return {
            success: false,
            error: error.message
        };

    }

});


// GET ALL SENSOR DATA

ipcMain.handle("get-sensor-data", () => {

    try {

        const rows = db.prepare(`
            SELECT *
            FROM sensor_data
            ORDER BY id DESC
        `).all();

        return rows;

    } catch (error) {

        console.error("SQLite read error:", error);

        return [];

    }

});


// GET LATEST SENSOR DATA

ipcMain.handle("get-latest-sensor-data", () => {

    try {

        const row = db.prepare(`
            SELECT *
            FROM sensor_data
            ORDER BY id DESC
            LIMIT 1
        `).get();

        return row || null;

    } catch (error) {

        console.error("SQLite latest data error:", error);

        return null;

    }

});

// CLOSE APP

app.on("window-all-closed", () => {

    if (process.platform !== "darwin") {
        app.quit();
    }

});