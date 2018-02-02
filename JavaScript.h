const char MAIN_JS[] PROGMEM = R"=====(
  var mouseclicks = 0;
  var mousemoves = 0;
  window.onload = function Inicializar(){
    document.getElementById("txtClick").innerHTML = mouseclicks;
    document.getElementById("txtMouse").innerHTML = mousemoves;
  }
  function AumentosClicks() {
    mouseclicks++;
    document.getElementById("txtClick").innerHTML = mouseclicks;
  }
  function AumentosMovimientos() {
    mousemoves++;
    document.getElementById("txtMouse").innerHTML = mousemoves;
  }
)=====";
