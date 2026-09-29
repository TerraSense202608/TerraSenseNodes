const themeButton =
    document.getElementById("themeButton");

if (themeButton) {

    themeButton.addEventListener(
        "click",
        function () {

            document.body.classList.toggle("dark");

            localStorage.setItem(
                "terrasenseTheme",
                document.body.classList.contains("dark")
                    ? "dark"
                    : "light"
            );

        }
    );

}


const savedTheme =
    localStorage.getItem("terrasenseTheme");

if (savedTheme === "dark") {

    document.body.classList.add("dark");

}


function updateClock() {

    const clock =
        document.getElementById("clock");

    if (!clock) return;

    const now = new Date();

    clock.innerText =
        now.toLocaleString();

}


setInterval(updateClock, 1000);

updateClock();