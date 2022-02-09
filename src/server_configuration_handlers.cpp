#include "server_configuration_handlers.h"

extern char wifiSSID[33];
extern char wifiPassword[33];

extern WebServer server;

String WiFiconnectionStatus(int which)
{
  switch (which)
  {
  case WL_CONNECTED:
    return String(F("Connected"));

  case WL_NO_SSID_AVAIL:
    return String(F("Network is not availible"));

  case WL_CONNECT_FAILED:
    return String(F("Wrong password"));

  case WL_IDLE_STATUS:
    return String(F("Idle status"));

  case WL_DISCONNECTED:
    return String(F("Disconnected"));

  default:
    return String(F("Unknown"));
  }
}

void outputDeviceState() {

    String Page = F("<p>Saved SSID: ");

    if (wifiSSID[0] != 0)
    {
        Page += wifiSSID;
        Page += F("</p>");
    } else {
        Page += F("(not set)</p>");
    }

    Page += F("<p>Saved password: ");

    if (wifiPassword[0] != 0)
    {
        Page += F("(set)</p>");
    } else {
        Page += F("(not set)</p>");
    }

    Page += F("<p>Connection to wifi: ");

    Page += WiFiconnectionStatus(WiFi.status());


    Page += F("<p>IP: ");
    Page += WiFi.localIP().toString();
    Page += F("</p>");

    server.sendContent(Page);
}

/** Handle root or redirect to captive portal */
void handleRoot() {
    if (captivePortal()) {  // If caprive portal redirect instead of displaying the page.
        return;
    }
    setNoCacheHeaders();

    server.setContentLength(CONTENT_LENGTH_UNKNOWN);

    server.send(200, "text/html", String(F("<html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></head><body>")));

    server.sendContent(String(F(
        "<h1>MBV Device</h1>"
    )));

    outputDeviceState();    

    server.sendContent(String(F(
        "<br /><h2><a href='/connect'>Connect to WIFI</a></h2>"
        "</body></html>"
    )));
}

void handleConnect() {
    if (captivePortal()) {  // If caprive portal redirect instead of displaying the page.
        return;
    }
    int n = WiFi.scanNetworks();

    setNoCacheHeaders();

    server.setContentLength(CONTENT_LENGTH_UNKNOWN);

    server.send(200, "text/html", String(F("<html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></head><body>")));

    server.sendContent(String(F(
        "<h1>MBV Device | Connect to WiFi</h1>"
    )));

    outputDeviceState(); 

    String Page;

    Page += String(F(
        "<form method='POST' action='saveconfig'>"
        "<label for='ssid'>SSID:</label><select name='ssid'><option value=''>manually</option>"
    ));

    for (int i = 0; i < n; i++) {
        String ssid_name = WiFi.SSID(i);
        Serial.println(ssid_name);
        Page += F("<option value='");
        Page += ssid_name;
        Page += F("'>");
        Page += ssid_name;
        Page += F("</option>");
    }

    Page += String(F(""
          "</select>"
          "<br /><label for='ssid_manually'>SSID (manually):</label><input type='text' name='ssid_manually' value=''/>"
          "<br /><label for='password'>Password:</label><input type='password' name='password' value=''/>"
          "<br /><input type='submit' value='Save'/></form>"
          "</body></html>"));

    server.sendContent(Page);
}

void handleApple() {
    setNoCacheHeaders();

    String Page;
    Page += String(F(
            "<html><head><title>Success</title</head><body>"
            "Success"
            "</body></html>"
    ));

    server.send(200, "text/html", Page);
}

/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do
 * not try to handle the request again. */
boolean captivePortal() {
    if (!isIp(server.hostHeader())) {
        Serial.println("Request redirected to captive portal");
        server.sendHeader("Location", String("http://") + toStringIp(server.client().localIP()), true);
        server.send(302, "text/plain", "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
        server.client().stop();  // Stop is needed because we sent no content length
        return true;
    }
    return false;
}

void handleSaveConfig() {
    Serial.println("save config");
    String ssid_name = server.arg("ssid");

    if (ssid_name.length() == 0) {
        ssid_name = server.arg("ssid_manually");
    }

    if (ssid_name.length() == 0) {
        //error
        Serial.println("ssid_name error");
    }

    String ssid_password = server.arg("password");

    if (ssid_password.length() < 8) {
        //error
        Serial.println("ssid_password error");
    }

    ssid_name.toCharArray(wifiSSID, 33);
    ssid_password.toCharArray(wifiPassword, 33);

    if (wifiSSID[0] == 0) {
        Serial.println("wifiSSID not set up");
    } else {
        Serial.print("wifiSSID: ");
        Serial.println(String(wifiSSID));
    }

    if (wifiPassword[0] == 0) {
        Serial.println("wifiPassword not set up");
    } else {
        Serial.print("wifiPassword: ");
        Serial.println(String(wifiPassword));
    }    

    saveEEPROM();

    connectToSsid();

    WiFi.waitForConnectResult();

    server.sendHeader("Location", "/", true);
    setNoCacheHeaders();
    server.send(302, "text/plain", "");         // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.client().stop();  // Stop is needed because we sent no content length
    //saveSerialNumber();
}

void handleNotFound() {
    if (captivePortal()) {  // If caprive portal redirect instead of displaying the error page.
        return;
    }
    String message = F("File Not Found\n\n");
    message += F("URI: ");
    message += server.uri();
    message += F("\nMethod: ");
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += F("\nHost: ");
    message += server.hostHeader();
    message += F("\nArguments: ");
    message += server.args();
    message += F("\n");

    for (uint8_t i = 0; i < server.args(); i++) {
        message += String(F(" ")) + server.argName(i) + F(": ") + server.arg(i) + F("\n");
    }
    
    Serial.println(message);

    setNoCacheHeaders();
    server.send(404, "text/plain", message);
}