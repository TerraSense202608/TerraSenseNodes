// =====================================================
// TERRASENSE LIVE DATA (OFFLINE)
// Gateway (LoRa) -> Electron main.js -> here
// This file replaces the old Firebase script.js.
// It does not talk to the internet at all: it just
// listens for the "sensor-data" IPC messages that
// main.js forwards every time the Gateway sends a
// LoRa packet over USB serial, and re-dispatches them
// as the same window CustomEvents the dashboard already
// listens for, so dashboard.js / sensors.js need no
// changes.
// =====================================================

if (window.terraSense && window.terraSense.onSensorData) {

    window.terraSense.onSensorData((data) => {

        console.log("================================");
        console.log("LIVE DATA RECEIVED");
        console.log("Node:", data.node_id);
        console.log("Type:", data.node_type);
        console.log("Status:", data.status);
        console.log("Temperature:", data.temperature);
        console.log("Smoke:", data.smoke);
        console.log("Gas:", data.gas);
        console.log("Flame:", data.flame);
        console.log("Pressure:", data.pressure);
        console.log("================================");

        const nodeType =
            data.node_type ||
            (String(data.node_id || "").toUpperCase().startsWith("FIRE")
                ? "fire"
                : "landslide");

        if (nodeType === "fire") {

            console.log("🔥 DISPATCHING FIRE EVENT");

            window.dispatchEvent(
                new CustomEvent("terrasenseFireData", {
                    detail: data
                })
            );

        } else {

            console.log("⛰️ DISPATCHING LANDSLIDE EVENT");

            window.dispatchEvent(
                new CustomEvent("terrasenseData", {
                    detail: data
                })
            );
        }
    });

    console.log("================================");
    console.log("TERRASENSE LIVE DATA LISTENER READY");
    console.log("(offline - via Gateway USB serial)");
    console.log("================================");

} else {

    console.warn(
        "window.terraSense.onSensorData is not available. " +
        "Check preload.js is wired up correctly."
    );

}


// =====================================================
// MANUAL TEST HELPER
// Open DevTools and run testSensorData() to simulate a
// packet arriving, without needing real hardware plugged in.
// =====================================================

window.testSensorData = function (nodeType = "landslide") {

    const landslideSample = {
        node_id: "NODE1",
        node_type: "landslide",
        status: "WATCH",
        tiltX: 1.2,
        tiltY: 0.8,
        maxTilt: 2.1,
        soilRaw: 720,
        soilWet: "YES",
        rainRaw: 350,
        rainDetected: "YES",
        hx711Raw: 12500,
        accX: 0.12,
        accY: 0.08,
        accZ: 9.81,
        gyroX: 0.30,
        gyroY: 0.20,
        gyroZ: 0.10,
        temperature: 28.5,
        rssi: -72,
        snr: 8.5,
        online: true
    };

    const fireSample = {
        node_id: "FIRE01",
        node_type: "fire",
        status: "SAFE",
        flame: false,
        mq2Raw: 1200,
        smoke: 1200,
        gas: 1200,
        temperature: 30.0,
        pressure: 1008.2,
        rssi: -65,
        snr: 9.0,
        online: true
    };

    const sample = nodeType === "fire" ? fireSample : landslideSample;

    window.dispatchEvent(
        new CustomEvent(
            nodeType === "fire" ? "terrasenseFireData" : "terrasenseData",
            { detail: sample }
        )
    );

    console.log("Dispatched test", nodeType, "packet:", sample);
};
