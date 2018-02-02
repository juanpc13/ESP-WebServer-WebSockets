const char MAIN_JS[] PROGMEM = R"=====(

  window.onload = function() {
    window.setInterval(loadTime,50);
  };  
  
  function loadTime() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        var tiempo = JSON.parse(this.responseText);
        document.getElementById("tiempo").innerHTML = tiempo.time;
      }
    };
    xhttp.open("GET", "data", true);
    xhttp.send();    
  }
  
)=====";
