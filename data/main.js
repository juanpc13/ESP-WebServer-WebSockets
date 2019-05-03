window.onload = function() {
    window.setInterval(loadTime, 100);
};

function loadTime() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            var jsonData = JSON.parse(this.responseText);
            document.getElementById("luz").innerHTML = jsonData.luz;
        }
    };
    xhttp.open("GET", "data", true);
    xhttp.send();
}