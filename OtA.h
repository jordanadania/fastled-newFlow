WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
IPAddress ip(192,168,1,23);   
IPAddress gateway(192,168,1,254);   
IPAddress subnet(255,255,255,0);   
WiFi.config(ip, gateway, subnet);
while (WiFi.waitForConnectResult() != WL_CONNECTED) {
  Serial.println("Connection Failed! Rebooting...");
  delay(5000);
  ESP.restart();
}
ArduinoOTA.begin();
