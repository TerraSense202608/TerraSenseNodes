const {
    contextBridge,
    ipcRenderer
} = require("electron");


contextBridge.exposeInMainWorld(
    "terraSense",
    {

        // SQLite
        saveSensorData: (data) => {

            return ipcRenderer.invoke(
                "save-sensor-data",
                data
            );

        },

        getSensorData: () => {

            return ipcRenderer.invoke(
                "get-sensor-data"
            );

        },

        getLatestSensorData: () => {

            return ipcRenderer.invoke(
                "get-latest-sensor-data"
            );

        },


        // Serial
        listSerialPorts: () => {

            return ipcRenderer.invoke(
                "list-serial-ports"
            );

        },

        connectSerial: (port, baudRate) => {

            return ipcRenderer.invoke(
                "connect-serial",
                {
                    port,
                    baudRate
                }
            );

        },


        // Live data (replaces the old Firebase onValue() feed).
        // main.js pushes one event per LoRa packet received on
        // the Gateway's serial port; call this once from the
        // renderer to subscribe.
        onSensorData: (callback) => {

            ipcRenderer.on(
                "sensor-data",
                (event, data) => callback(data)
            );

        }

    }
);
