var ws = new WebSocket("ws://"+location.hostname+"/ws");

ws.onopen = function() {
    // Web Socket is connected, send data using send()
    let text = "AppName="+navigator.appName;
    ws.send(text);
    console.log(text);
};
  
ws.onmessage = function (evt) { 
    var data = JSON.parse(evt.data);
    document.getElementById("millis").innerHTML = data.millis;
    console.log(data);
};
  
ws.onclose = function() {        
    console.log("Connection is closed..."); 
};