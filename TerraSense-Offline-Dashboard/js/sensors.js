// =====================================================
// TERRASENSE SENSOR DISPLAY
// =====================================================

// Wait for data from script.js
window.addEventListener("terrasenseData", (event) => {

    const data = event.detail;

    console.log("SENSORS.JS RECEIVED:", data);


    // =================================================
    // SOIL
    // =================================================

    if (data.soilRaw !== undefined) {

        document.getElementById("soilSensor").innerText =
            data.soilRaw;

    }


    // =================================================
    // RAIN
    // =================================================

    if (data.rainRaw !== undefined) {

        document.getElementById("rainSensorRaw").innerText =
            data.rainRaw;

    }


    if (data.rainDetected !== undefined) {

        document.getElementById("rainSensor").innerText =
            data.rainDetected;

    }


    // =================================================
    // TILT X
    // =================================================

    if (data.tiltX !== undefined) {

        document.getElementById("tiltX").innerText =
            Number(data.tiltX).toFixed(2) + "°";

    }


    // =================================================
    // TILT Y
    // =================================================

    if (data.tiltY !== undefined) {

        document.getElementById("tiltY").innerText =
            Number(data.tiltY).toFixed(2) + "°";

    }


    // =================================================
    // DEBUG
    // =================================================

    console.log("Soil:", data.soilRaw);
    console.log("Rain:", data.rainRaw);
    console.log("Rain Status:", data.rainDetected);
    console.log("Tilt X:", data.tiltX);
    console.log("Tilt Y:", data.tiltY);

});