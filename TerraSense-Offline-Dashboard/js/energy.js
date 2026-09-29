function updateEnergy() {

    const solar =
        (Math.random() * 18 + 2).toFixed(1);

    const battery =
        Math.floor(
            Math.random() * 35 + 65
        );

    const consumption =
        (Math.random() * 2 + 2).toFixed(1);


    document.getElementById(
        "energySolar"
    ).innerText =
        solar + " W";


    document.getElementById(
        "energyBattery"
    ).innerText =
        battery + "%";


    document.getElementById(
        "consumption"
    ).innerText =
        consumption + " W";

}


updateEnergy();

setInterval(
    updateEnergy,
    3000
);