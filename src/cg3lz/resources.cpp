#include "resources.h"

std::string resources::index_html = R"(
Logs
====

{{#logs}}
- [{{filename}}]({{url}}) [{{size}}]
{{/logs}}


[Ticker](/ticker)


Config
======

App logging enabled: {{app_logging_enabled}}

<style>h1:before, h2:before { content: none; }</style>
<!-- Markdeep: --><style class="fallback">body{visibility:hidden;white-space:pre;font-family:monospace}</style><script src="markdeep.min.js"></script><script src="https://casual-effects.com/markdeep/latest/markdeep.min.js"></script><script>window.alreadyProcessedMarkdeep||(document.body.style.visibility="visible")</script>
)";

std::string resources::ticker_html = R"(
<!-- based on https://github.com/zaphoyd/websocketpp/tree/master/examples/telemetry_server -->
<!doctype html>
<html>
<head>
    <title>Incoming logs</title>
</head>
<body>

    <script type="text/javascript">
var ws;
var url;

function connect() {
    url = document.getElementById("server_url").value;

    if ("WebSocket" in window) {
        ws = new WebSocket(url);
    } else if ("MozWebSocket" in window) {
        ws = new MozWebSocket(url);
    } else {
        document.getElementById("messages").innerHTML += "This Browser does not support WebSockets<br />";
        return;
    }
    ws.onopen = function(e) {
        document.getElementById("messages").innerHTML += "Client: A connection to "+ws.url+" has been opened.<br />";

        document.getElementById("server_url").disabled = true;
        document.getElementById("toggle_connect").innerHTML = "Disconnect";
    };

    ws.onerror = function(e) {
        document.getElementById("messages").innerHTML += "Client: An error occured, see console log for more details.<br />";
        console.log(e);
    };

    ws.onclose = function(e) {
        document.getElementById("messages").innerHTML += "Client: The connection to "+url+" was closed. ["+e.code+(e.reason != "" ? ","+e.reason : "")+"]<br />";
        cleanup_disconnect();
    };

    ws.onmessage = function(e) {
        document.getElementById("messages").innerHTML += "Server: "+e.data+"<br />";
    };
}

function disconnect() {
    ws.close();
    cleanup_disconnect();
}

function cleanup_disconnect() {
    document.getElementById("server_url").disabled = false;
    document.getElementById("toggle_connect").innerHTML = "Connect";
}

function toggle_connect() {
    if (document.getElementById("server_url").disabled === false) {
        connect();
    } else {
        disconnect();
    }
}
    </script>

    <style>
        body, html {
            margin: 0px;
            padding: 0px;
        }

        #controls {
            float: right;
            background-color: #999;
        }
    </style>

    <div id="controls">
        <div id="server">
            <input type="text" name="server_url" id="server_url" value="ws://localhost:9002" /><br />
            <button id="toggle_connect" onclick="toggle_connect();">Connect</button>
        </div>
    </div>
    <div id="messages"></div>

</body>
</html>
)";