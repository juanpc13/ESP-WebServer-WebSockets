var switchButton = document.getElementById("switchButton");
var ws;

switchButton.onclick = function () {
    var json = { relay: this.checked };
    if (ws.readyState == WebSocket.OPEN) {
        ws.send(JSON.stringify(json));
    }
}

function connect() {
    ws = new WebSocket("ws://" + location.hostname + "/ws");
    ws.onopen = function () {
        // subscribe to some channels
        console.log("WebSocket Open");
    };

    ws.onmessage = function (evt) {
        var json = JSON.parse(evt.data);
        switchButton.checked = json.relay;
        console.log(json);
    };

    ws.onclose = function (evt) {
        console.log('Socket is closed. Reconnect will be attempted in 1 second.', evt.reason);
        setTimeout(function () {
            connect();
        }, 1000);
    };

    ws.onerror = function (err) {
        console.error('Socket encountered error: ', err.message, 'Closing socket');
        ws.close();
    };
}

connect();