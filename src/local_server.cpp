#include "local_server.h"

extern IPAddress apIP;


// DNS server
const byte DNS_PORT = 53;
DNSServer dnsServer;

// Web server
WebServer server(80);

void startHttpServer() {
    /* Setup the DNS server redirecting all the domains to the apIP */
    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.start(DNS_PORT, "*", apIP);

    /* Setup config web pages: root, wifi config pages, SO captive portal detectors. */
    server.on("/", handleRoot);
    server.on("/connect", handleConnect);
    server.on("/saveconfig", handleSaveConfig);
    server.on("/clear", handleClearConfig);
    //httpUpdater.setup(&server);
    server.on("/hotspot-detect.html", handleApple); // iOS
    server.on("/generate_204", handleRoot);  // Android captive portal. Maybe not needed. Might be handled by notFound handler.
    server.on("/fwlink", handleRoot);  // Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.

    /* Setup not-found and start web server. */
    server.onNotFound(handleNotFound);
    server.begin();  // Web server start
    Serial.println("HTTP server started");
}

void loopHttpServer() {
    dnsServer.processNextRequest();
    server.handleClient();
}

void setNoCacheHeaders() {
    server.sendHeader(String(F("Cache-Control")), String(F("no-cache, no-store, must-revalidate")));
    server.sendHeader(String(F("Pragma")), String(F("no-cache")));
    server.sendHeader(String(F("Expires")), String(F("-1")));
}