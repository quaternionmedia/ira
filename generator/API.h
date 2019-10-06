#include "generator.h"

#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"
#include "AsyncJson.h"
#include "ArduinoJson.h"

#include "Secrets.h"

AsyncWebServer server(80);


// uint8_t dmx[8];

void initServer() {
  Serial.begin(9600);
  WiFi.begin(WiFiName, WiFiPassword);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    StaticJsonDocument<200> doc;
    JsonArray dmxDoc = doc.createNestedArray("dmx");
    for (uint8_t & d : DMX) {
      dmxDoc.add(d);
    }
    // request->send(200, "application/json", doc);

    serializeJson(doc, *response);
    request->send(response);
  });

  AsyncCallbackJsonWebHandler* jhandler = new AsyncCallbackJsonWebHandler("/", [](AsyncWebServerRequest *request, JsonVariant &json) {
    JsonObject jsonObj = json.as<JsonObject>();
    if (jsonObj.getMember("dmx")){
      for (uint8_t i=0; i<8; i++) {
        DMX[i] = json["dmx"][i];
      }
    }
    request->send(200, "OK");
  });
  server.addHandler(jhandler);

  server.on("/count", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(apiCounter++));
  });

  server.begin();
}
