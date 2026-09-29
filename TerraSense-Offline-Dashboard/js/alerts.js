const alerts = [

    {
        title: "Soil moisture within normal range",
        type: "NORMAL"
    },

    {
        title: "MPU6050 connection stable",
        type: "NORMAL"
    },

    {
        title: "Rain sensor activity detected",
        type: "WARNING"
    },

    {
        title: "ESP32 Node 01 connected",
        type: "NORMAL"
    }

];


function loadAlerts() {

    const alertList =
        document.getElementById("alertList");


    alerts.forEach(alert => {

        const item =
            document.createElement("div");

        item.className =
            "alert-item";


        item.innerHTML = `

            <div>

                <strong>
                    ${alert.title}
                </strong>

                <span>
                    Status: ${alert.type}
                </span>

            </div>

            <div class="alert-time">

                ${new Date().toLocaleTimeString()}

            </div>

        `;


        alertList.appendChild(item);

    });

}


loadAlerts();