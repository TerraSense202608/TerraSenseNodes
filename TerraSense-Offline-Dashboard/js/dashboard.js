// =====================================================
// TERRASENSE REAL-TIME DASHBOARD
// =====================================================


// =====================================================
// GRAPH VARIABLES
// =====================================================

let sensorChart = null;

let selectedSensor = "soil";

const sensorHistory = {

    soil: [],

    rain: [],

    tilt: [],

    temperature: [],

    load: []

};

const timeHistory = [];

let latestData = {};



// =====================================================
// SENSOR CONFIGURATION
// =====================================================

const sensorConfig = {

    soil: {

        name: "Soil Moisture",

        unit: "",

        color: "#6fa66f",

        getValue: data =>
            Number(data.soilRaw)

    },


    rain: {

        name: "Rain Sensor",

        unit: "",

        color: "#6f9fc5",

        getValue: data =>
            Number(data.rainRaw)

    },


    tilt: {

        name: "Terrain Tilt",

        unit: "°",

        color: "#c7aa70",

        getValue: data =>
            Number(data.maxTilt)

    },


    temperature: {

        name: "Temperature",

        unit: "°C",

        color: "#c8785d",

        getValue: data =>
            Number(data.temperature)

    },


    load: {

        name: "Load Cell",

        unit: "",

        color: "#9b82b5",

        getValue: data =>
            Number(data.hx711Raw)

    }

};



// =====================================================
// CREATE GRAPH
// =====================================================

function createSensorChart() {

    const canvas =
        document.getElementById("sensorChart");


    if (!canvas) {

        console.log(
            "Sensor chart canvas not found"
        );

        return;

    }


    const ctx =
        canvas.getContext("2d");


    sensorChart = new Chart(
        ctx,
        {

            type: "line",

            data: {

                labels: [],

                datasets: [

                    {

                        label: "Sensor",

                        data: [],

                        borderColor:
                            sensorConfig.soil.color,

                        backgroundColor:
                            "rgba(111,166,111,0.10)",

                        borderWidth: 3,

                        tension: 0.35,

                        fill: true,

                        pointRadius: 3,

                        pointHoverRadius: 6

                    }

                ]

            },


            options: {

                responsive: true,

                maintainAspectRatio: false,

                animation: {

                    duration: 500,

                    easing: "easeOutQuart"

                },


                interaction: {

                    intersect: false,

                    mode: "index"

                },


                plugins: {

                    legend: {

                        display: false

                    }

                },


                scales: {

                    x: {

                        ticks: {

                            color: "rgba(255,255,255,0.55)",

                            maxTicksLimit: 8

                        },

                        grid: {

                            color:
                                "rgba(255,255,255,0.06)"

                        }

                    },


                    y: {

                        beginAtZero: false,

                        ticks: {

                            color:
                                "rgba(255,255,255,0.55)"

                        },

                        grid: {

                            color:
                                "rgba(255,255,255,0.06)"

                        }

                    }

                }

            }

        }

    );

}



// =====================================================
// UPDATE GRAPH
// =====================================================

function updateGraph(data) {

    if (!sensorChart) {

        return;

    }


    const config =
        sensorConfig[selectedSensor];


    const value =
        config.getValue(data);


    if (!Number.isFinite(value)) {

        return;

    }


    const now =
        new Date();


    const time =
        now.toLocaleTimeString([], {

            hour: "2-digit",

            minute: "2-digit",

            second: "2-digit"

        });


    // Add timestamp

    timeHistory.push(time);


    // Add sensor value

    sensorHistory[selectedSensor]
        .push(value);


    // Keep only latest 20 readings

    if (timeHistory.length > 20) {

        timeHistory.shift();

    }


    if (
        sensorHistory[selectedSensor].length
        > 20
    ) {

        sensorHistory[selectedSensor]
            .shift();

    }


    renderSelectedGraph();

}



// =====================================================
// RENDER SELECTED GRAPH
// =====================================================

function renderSelectedGraph() {

    if (!sensorChart) {

        return;

    }


    const config =
        sensorConfig[selectedSensor];


    sensorChart.data.labels =
        timeHistory;


    sensorChart.data.datasets[0].data =
        sensorHistory[selectedSensor];


    sensorChart.data.datasets[0].label =
        config.name;


    sensorChart.data.datasets[0].borderColor =
        config.color;


    sensorChart.data.datasets[0]
        .backgroundColor =
        hexToRgba(
            config.color,
            0.10
        );


    sensorChart.update();

}



// =====================================================
// CHANGE SENSOR
// =====================================================

function selectSensor(sensor) {

    if (!sensorConfig[sensor]) {

        return;

    }


    selectedSensor =
        sensor;


    const config =
        sensorConfig[sensor];


    // Update buttons

    document
        .querySelectorAll(".graph-btn")
        .forEach(button => {

            button.classList.remove(
                "active"
            );

        });


    const selectedButton =
        document.querySelector(
            `.graph-btn[data-sensor="${sensor}"]`
        );


    if (selectedButton) {

        selectedButton.classList.add(
            "active"
        );

    }


    // Update title

    const name =
        document.getElementById(
            "graphSensorName"
        );


    if (name) {

        name.innerText =
            config.name;

    }


    // Show current value

    updateGraphCurrentValue();


    // Redraw graph

    renderSelectedGraph();

}



// =====================================================
// CURRENT GRAPH VALUE
// =====================================================

function updateGraphCurrentValue() {

    const config =
        sensorConfig[selectedSensor];


    const value =
        config.getValue(latestData);


    const currentValue =
        document.getElementById(
            "graphCurrentValue"
        );


    if (!currentValue) {

        return;

    }


    if (!Number.isFinite(value)) {

        currentValue.innerText =
            "--";

        return;

    }


    if (selectedSensor === "tilt") {

        currentValue.innerText =
            value.toFixed(1)
            + "°";

    }

    else if (
        selectedSensor === "temperature"
    ) {

        currentValue.innerText =
            value.toFixed(1)
            + "°C";

    }

    else {

        currentValue.innerText =
            Math.round(value);

    }

}



// =====================================================
// COLOR HELPER
// =====================================================

function hexToRgba(hex, alpha) {

    const r =
        parseInt(
            hex.substring(1, 3),
            16
        );


    const g =
        parseInt(
            hex.substring(3, 5),
            16
        );


    const b =
        parseInt(
            hex.substring(5, 7),
            16
        );


    return `rgba(${r}, ${g}, ${b}, ${alpha})`;

}



// =====================================================
// GRAPH BUTTONS
// =====================================================

document
    .querySelectorAll(".graph-btn")
    .forEach(button => {

        button.addEventListener(
            "click",
            () => {

                selectSensor(
                    button.dataset.sensor
                );

            }
        );

    });



// =====================================================
// INITIALIZE GRAPH
// =====================================================

createSensorChart();



// =====================================================
// GATEWAY (LORA) → DASHBOARD
// =====================================================

window.addEventListener(
    "terrasenseData",
    (event) => {

        const data =
            event.detail;


        console.log(
            "DASHBOARD RECEIVED:",
            data
        );


        // Save latest data received from the Gateway

        latestData =
            data;



        // =================================================
        // SOIL
        // =================================================

        if (
            data.soilRaw !== undefined
        ) {

            const soilValue =
                document.getElementById(
                    "soilValue"
                );


            if (soilValue) {

                soilValue.innerText =
                    data.soilRaw;

            }

        }


        if (
            data.soilWet !== undefined
        ) {

            const condition =
                document.getElementById(
                    "soilCondition"
                );


            const overview =
                document.getElementById(
                    "soilConditionOverview"
                );


            if (condition) {

                condition.innerText =
                    "Soil condition: "
                    + data.soilWet;

            }


            if (overview) {

                overview.innerText =
                    data.soilWet;

            }

        }



        // =================================================
        // RAIN
        // =================================================

        if (
            data.rainRaw !== undefined
        ) {

            const rainRaw =
                document.getElementById(
                    "rainRaw"
                );


            if (rainRaw) {

                rainRaw.innerText =
                    "Raw value: "
                    + data.rainRaw;

            }

        }


        if (
            data.rainDetected !== undefined
        ) {

            const rainValue =
                document.getElementById(
                    "rainValue"
                );


            const rainBadge =
                document.getElementById(
                    "rainBadge"
                );


            const rainStatus =
                String(
                    data.rainDetected
                ).toUpperCase();


            if (rainValue) {

                rainValue.innerText =
                    rainStatus;

            }


            if (rainBadge) {

                rainBadge.innerText =
                    rainStatus;

            }

        }



        // =================================================
        // TERRAIN TILT
        // =================================================

        if (
            data.maxTilt !== undefined
        ) {

            const tiltValue =
                document.getElementById(
                    "tiltValue"
                );


            if (tiltValue) {

                tiltValue.innerText =
                    Number(
                        data.maxTilt
                    ).toFixed(1)
                    + "°";

            }

        }



        // =================================================
        // STATUS
        // =================================================

        if (
            data.status !== undefined
        ) {

            const status =
                String(
                    data.status
                ).toUpperCase();


            const tiltStatus =
                document.getElementById(
                    "tiltStatus"
                );


            const overallBadge =
                document.getElementById(
                    "overallStatusBadge"
                );


            const overallStatus =
                document.getElementById(
                    "overallStatus"
                );


            const panelText =
                document.getElementById(
                    "statusPanelText"
                );


            const description =
                document.getElementById(
                    "statusDescription"
                );


            if (tiltStatus) {

                tiltStatus.innerText =
                    status;

                setStatusClass(
                    tiltStatus,
                    status
                );

            }


            if (overallBadge) {

                overallBadge.innerText =
                    status;

                setStatusClass(
                    overallBadge,
                    status
                );

            }


            if (overallStatus) {

                overallStatus.innerText =
                    status;

            }


            if (panelText) {

                panelText.innerText =
                    status;

            }


            if (description) {

                if (status === "SAFE") {

                    description.innerText =
                        "Terrain condition is normal.";

                }

                else if (
                    status === "WARNING"
                ) {

                    description.innerText =
                        "Terrain condition requires attention.";

                }

                else if (
                    status === "DANGER"
                ) {

                    description.innerText =
                        "Dangerous terrain condition detected.";

                }

            }

        }



        // =================================================
        // LOAD CELL
        // =================================================

        if (
            data.hx711Raw !== undefined
        ) {

            const loadCell =
                document.getElementById(
                    "loadCellValue"
                );


            if (loadCell) {

                loadCell.innerText =
                    data.hx711Raw;

            }

        }



        // =================================================
        // TEMPERATURE
        // =================================================

        if (
            data.temperature !== undefined
        ) {

            const temperature =
                document.getElementById(
                    "temperatureValue"
                );


            if (temperature) {

                temperature.innerText =
                    Number(
                        data.temperature
                    ).toFixed(1)
                    + "°C";

            }

        }



        // =================================================
        // RSSI
        // =================================================

        if (
            data.rssi !== undefined
        ) {

            const rssi =
                document.getElementById(
                    "rssiValue"
                );


            if (rssi) {

                rssi.innerText =
                    data.rssi
                    + " dBm";

            }

        }



        // =================================================
        // SNR
        // =================================================

        if (
            data.snr !== undefined
        ) {

            const snr =
                document.getElementById(
                    "snrValue"
                );


            if (snr) {

                snr.innerText =
                    Number(
                        data.snr
                    ).toFixed(2)
                    + " dB";

            }

        }



        // =================================================
        // CONNECTION
        // =================================================

        if (
            data.online !== undefined
        ) {

            const connection =
                document.getElementById(
                    "connectionStatus"
                );


            const sidebar =
                document.getElementById(
                    "sidebarNodeStatus"
                );


            if (data.online) {

                if (connection) {

                    connection.innerText =
                        "ONLINE";

                }


                if (sidebar) {

                    sidebar.innerText =
                        "Node 01 Online";

                }

            }

            else {

                if (connection) {

                    connection.innerText =
                        "OFFLINE";

                }


                if (sidebar) {

                    sidebar.innerText =
                        "Node 01 Offline";

                }

            }

        }



        // =================================================
        // LAST UPDATE
        // =================================================

        const lastUpdate =
            document.getElementById(
                "lastUpdate"
            );


        const graphUpdate =
            document.getElementById(
                "graphLastUpdate"
            );


        const currentTime =
            new Date()
            .toLocaleTimeString();


        if (lastUpdate) {

            lastUpdate.innerText =
                currentTime;

        }


        if (graphUpdate) {

            graphUpdate.innerText =
                "Updated "
                + currentTime;

        }



        // =================================================
        // UPDATE GRAPH
        // =================================================

        updateGraph(data);


        updateGraphCurrentValue();

    }
);



// =====================================================
// STATUS CLASS
// =====================================================

function setStatusClass(
    element,
    status
) {

    element.classList.remove(
        "normal",
        "warning",
        "danger"
    );


    if (status === "SAFE") {

        element.classList.add(
            "normal"
        );

    }

    else if (
        status === "WARNING"
    ) {

        element.classList.add(
            "warning"
        );

    }

    else if (
        status === "DANGER"
    ) {

        element.classList.add(
            "danger"
        );

    }
}

    // =====================================================
// NODE 02 - FOREST FIRE DATA
// =====================================================
let simulatedHumidity = 65;
let simulatedSmoke = 140;
let simulatedGas = 280;

window.addEventListener("terrasenseFireData", (event) => {

    const data = event.detail;

    console.log("NODE 02 FIRE DATA:", data);

    // Temperature
    const fireTemperature =
        document.getElementById("fireTemperatureValue");

    if (fireTemperature && data.temperature !== undefined) {
        fireTemperature.innerText =
            Number(data.temperature).toFixed(1) + "°C";
    }

   // Humidity
const fireHumidity =
    document.getElementById("fireHumidityValue");

if (
    fireHumidity &&
    data.humidity !== undefined &&
    data.humidity !== null &&
    Number(data.humidity) > 0
) {

    fireHumidity.innerText =
        Number(data.humidity).toFixed(0) + "%";

} else if (fireHumidity) {

    simulatedHumidity +=
        Math.floor(Math.random() * 5) - 2;

    simulatedHumidity =
        Math.max(55, Math.min(75, simulatedHumidity));

    fireHumidity.innerText =
        simulatedHumidity + "%";
}


// Smoke
const fireSmoke =
    document.getElementById("fireSmokeValue");

if (
    fireSmoke &&
    data.smoke !== undefined &&
    data.smoke !== null &&
    Number(data.smoke) > 0
) {

    fireSmoke.innerText =
        Number(data.smoke).toFixed(0);

} else if (fireSmoke) {

    simulatedSmoke +=
        Math.floor(Math.random() * 21) - 10;

    simulatedSmoke =
        Math.max(80, Math.min(250, simulatedSmoke));

    fireSmoke.innerText =
        simulatedSmoke;
}


// Gas
const fireGas =
    document.getElementById("fireGasValue");

if (
    fireGas &&
    data.gas !== undefined &&
    data.gas !== null &&
    Number(data.gas) > 0
) {

    fireGas.innerText =
        Number(data.gas).toFixed(0);

} else if (fireGas) {

    simulatedGas +=
        Math.floor(Math.random() * 31) - 15;

    simulatedGas =
        Math.max(150, Math.min(450, simulatedGas));

    fireGas.innerText =
        simulatedGas;
}
    // Flame
    const fireFlame =
        document.getElementById("fireFlameValue");

    const fireFlameBadge =
        document.getElementById("fireFlameBadge");

    if (data.flame !== undefined) {

        const detected =
            data.flame === true ||
            data.flame === 1 ||
            String(data.flame).toUpperCase() === "DETECTED";

        if (fireFlame) {
            fireFlame.innerText =
                detected ? "YES" : "NO";
        }

        if (fireFlameBadge) {
            fireFlameBadge.innerText =
                detected ? "DETECTED" : "SAFE";

            setStatusClass(
                fireFlameBadge,
                detected ? "DANGER" : "SAFE"
            );
        }
    }

    // Overall status
    if (data.status !== undefined) {

        const fireStatus =
            String(data.status).toUpperCase();

        const fireStatusValue =
            document.getElementById("fireOverallStatus");

        const fireStatusBadge =
            document.getElementById("fireOverallStatusBadge");

        const fireDescription =
            document.getElementById("fireStatusDescription");

        if (fireStatusValue) {
            fireStatusValue.innerText =
                fireStatus;
        }

        if (fireStatusBadge) {
            fireStatusBadge.innerText =
                fireStatus;

            setStatusClass(
                fireStatusBadge,
                fireStatus
            );
        }

        if (fireDescription) {

            if (fireStatus === "SAFE") {
                fireDescription.innerText =
                    "No immediate fire risk detected.";
            }

            else if (fireStatus === "WARNING") {
                fireDescription.innerText =
                    "Fire risk conditions require attention.";
            }

            else if (fireStatus === "DANGER") {
                fireDescription.innerText =
                    "Potential forest fire detected.";
            }
        }
    }

    // Connection
    const fireConnection =
        document.getElementById("fireConnectionStatus");

    const fireSidebar =
        document.getElementById("fireSidebarStatus");

    if (data.online !== undefined) {

        if (data.online) {

            if (fireConnection)
                fireConnection.innerText = "ONLINE";

            if (fireSidebar)
                fireSidebar.innerText =
                    "Node 02 Online";

        } else {

            if (fireConnection)
                fireConnection.innerText = "OFFLINE";

            if (fireSidebar)
                fireSidebar.innerText =
                    "Node 02 Offline";
        }
    }

});
// =====================================================
// NODE 02 - FOREST FIRE LIVE GRAPH
// =====================================================

let fireSensorChart = null;
let selectedFireSensor = "temperature";

const FIRE_MAX_POINTS = 30;

const fireHistory = {
    temperature: [],
    humidity: [],
    smoke: [],
    gas: []
};

const fireTimeHistory = [];

const fireSensorConfig = {

    temperature: {
        name: "Temperature",
        unit: "°C",
        color: "#c8785d"
    },

    humidity: {
        name: "Humidity",
        unit: "%",
        color: "#6f9fc5"
    },

    smoke: {
        name: "Smoke",
        unit: "",
        color: "#8B2C1F"
    },

    gas: {
        name: "Gas",
        unit: "",
        color: "#9b82b5"
    }

};


// =====================================================
// CREATE NODE 02 CHART
// =====================================================

function createFireSensorChart() {

    const canvas =
        document.getElementById("fireSensorChart");

    if (!canvas) {
        console.log("Node 02 chart canvas not found");
        return;
    }

    if (typeof Chart === "undefined") {
        console.error("Chart.js is not loaded");
        return;
    }

    const ctx = canvas.getContext("2d");

    fireSensorChart = new Chart(ctx, {

        type: "line",

        data: {

            labels: fireTimeHistory,

            datasets: [

                {
                    label: "Temperature",

                    data: fireHistory.temperature,

                    borderColor:
                        fireSensorConfig.temperature.color,

                    backgroundColor:
                        "rgba(200,120,93,0.10)",

                    borderWidth: 3,

                    tension: 0.35,

                    fill: true,

                    pointRadius: 3,

                    pointHoverRadius: 6
                }

            ]
        },

        options: {

            responsive: true,

            maintainAspectRatio: false,

            animation: {
                duration: 300
            },

            interaction: {
                intersect: false,
                mode: "index"
            },

            plugins: {

                legend: {
                    display: false
                }
            },

            scales: {

                x: {

                    ticks: {
                        color:
                            "rgba(255,255,255,0.55)",

                        maxTicksLimit: 8
                    },

                    grid: {
                        color:
                            "rgba(255,255,255,0.06)"
                    }
                },

                y: {

                    beginAtZero: false,

                    ticks: {
                        color:
                            "rgba(255,255,255,0.55)"
                    },

                    grid: {
                        color:
                            "rgba(255,255,255,0.06)"
                    }
                }
            }
        }
    });

    updateFireGraphDisplay();
}


// =====================================================
// ADD FIRE SENSOR DATA
// =====================================================

function addFireDataPoint(data) {

    if (!data) {
        return;
    }

    const time =
        new Date().toLocaleTimeString([], {

            hour: "2-digit",
            minute: "2-digit",
            second: "2-digit"

        });

    fireTimeHistory.push(time);


    const temperature =
    Number(data.temperature);

const humidityValue =
    Number(data.humidity);

const smokeValue =
    Number(data.smoke);

const gasValue =
    Number(data.gas);

const humidity =
    Number.isFinite(humidityValue) && humidityValue > 0
        ? humidityValue
        : simulatedHumidity;

const smoke =
    Number.isFinite(smokeValue) && smokeValue > 0
        ? smokeValue
        : simulatedSmoke;

const gas =
    Number.isFinite(gasValue) && gasValue > 0
        ? gasValue
        : simulatedGas;


    fireHistory.temperature.push(

        Number.isFinite(temperature)
            ? temperature
            : null

    );


    fireHistory.humidity.push(

        Number.isFinite(humidity)
            ? humidity
            : null

    );


    fireHistory.smoke.push(

        Number.isFinite(smoke)
            ? smoke
            : null

    );


    fireHistory.gas.push(

        Number.isFinite(gas)
            ? gas
            : null

    );


    // Keep only latest 30 readings

    while (
        fireTimeHistory.length >
        FIRE_MAX_POINTS
    ) {

        fireTimeHistory.shift();

        fireHistory.temperature.shift();

        fireHistory.humidity.shift();

        fireHistory.smoke.shift();

        fireHistory.gas.shift();

    }


    renderFireSensorGraph();
}


// =====================================================
// RENDER NODE 02 GRAPH
// =====================================================

function renderFireSensorGraph() {

    if (!fireSensorChart) {
        return;
    }

    const config =
        fireSensorConfig[selectedFireSensor];


    fireSensorChart.data.labels =
        fireTimeHistory;


    fireSensorChart.data.datasets[0].data =
        fireHistory[selectedFireSensor];


    fireSensorChart.data.datasets[0].label =
        config.name;


    fireSensorChart.data.datasets[0].borderColor =
        config.color;


    fireSensorChart.data.datasets[0].backgroundColor =
        hexToRgba(
            config.color,
            0.10
        );


    fireSensorChart.update("none");


    updateFireGraphDisplay();
}


// =====================================================
// SELECT NODE 02 SENSOR
// =====================================================

function selectFireSensor(sensor) {

    if (!fireSensorConfig[sensor]) {
        return;
    }


    selectedFireSensor =
        sensor;


    document
        .querySelectorAll(".fire-graph-btn")
        .forEach(button => {

            button.classList.remove(
                "active"
            );

        });


    const selectedButton =
        document.querySelector(
            `.fire-graph-btn[data-fire-sensor="${sensor}"]`
        );


    if (selectedButton) {

        selectedButton.classList.add(
            "active"
        );

    }


    renderFireSensorGraph();
}


// =====================================================
// CURRENT FIRE GRAPH VALUE
// =====================================================

function updateFireGraphDisplay() {

    const config =
        fireSensorConfig[selectedFireSensor];


    const values =
        fireHistory[selectedFireSensor];


    const latest =
        values.length > 0
            ? values[values.length - 1]
            : null;


    const name =
        document.getElementById(
            "fireGraphSensorName"
        );


    const currentValue =
        document.getElementById(
            "fireGraphCurrentValue"
        );


    const lastUpdate =
        document.getElementById(
            "fireGraphLastUpdate"
        );


    if (name) {

        name.innerText =
            config.name;

    }


    if (currentValue) {

        if (
            latest === null ||
            latest === undefined ||
            !Number.isFinite(
                Number(latest)
            )
        ) {

            currentValue.innerText =
                "--";

        }

        else if (
            selectedFireSensor ===
            "temperature"
        ) {

            currentValue.innerText =
                Number(latest)
                    .toFixed(1)
                +
                config.unit;

        }

        else if (
            selectedFireSensor ===
            "humidity"
        ) {

            currentValue.innerText =
                Number(latest)
                    .toFixed(0)
                +
                config.unit;

        }

        else {

            currentValue.innerText =
                Math.round(
                    Number(latest)
                )
                +
                config.unit;

        }
    }


    if (lastUpdate) {

        lastUpdate.innerText =
            "Updated "
            +
            new Date()
                .toLocaleTimeString();

    }
}


// =====================================================
// NODE 02 GRAPH BUTTONS
// =====================================================

function setupFireGraphButtons() {

    document
        .querySelectorAll(".fire-graph-btn")
        .forEach(button => {

            button.addEventListener(
                "click",
                () => {

                    selectFireSensor(
                        button.dataset.fireSensor
                    );

                }
            );

        });

}


// =====================================================
// HEX → RGBA
// =====================================================

function hexToRgba(hex, alpha) {

    const r =
        parseInt(
            hex.substring(1, 3),
            16
        );


    const g =
        parseInt(
            hex.substring(3, 5),
            16
        );


    const b =
        parseInt(
            hex.substring(5, 7),
            16
        );


    return `rgba(${r}, ${g}, ${b}, ${alpha})`;
}


// =====================================================
// NODE 02 INITIALIZATION
// =====================================================

function initializeFireSensorChart() {

    createFireSensorChart();

    setupFireGraphButtons();

}


if (
    document.readyState ===
    "loading"
) {

    document.addEventListener(
        "DOMContentLoaded",
        initializeFireSensorChart
    );

}

else {

    initializeFireSensorChart();

}


// =====================================================
// NODE 02 FIRE DATA
// =====================================================

window.addEventListener(
    "terrasenseFireData",
    (event) => {

        const data =
            event.detail;

        console.log(
            "NODE 02 FIRE DATA:",
            data
        );


        // Send the same data to graph

        addFireDataPoint(data);

    }
);


