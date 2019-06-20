window.onload = function() {
    window.setInterval(loadTime, 500);
};

function loadTime() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            var jsonData = JSON.parse(this.responseText);
            document.getElementById("millis").innerHTML = jsonData.millis;
        }
    };
    xhttp.open("GET", "status", true);
    xhttp.send();
}