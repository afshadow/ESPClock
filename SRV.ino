void server_init(void) {
  server.on("/configs.json", handle_ConfigJSON);
  server.on("/ssid", handle_Set_Ssid);
  server.on("/ssidap", handle_Set_Ssidap);
  server.on("/timeZone", handle_time_zone);
  server.on("/weatherKey", handle_weatherKey);
  server.on("/weatherUpdate", handle_weather_update);
  server.on("/ntpServerName", handle_ntpServerName);
  server.on("/weatherHost", handle_weatherHost);
  server.on("/cityID", handle_cityID);
  server.on("/forecastView", handle_forecastView);
  server.on("/updateOtta", handle_updateOta);
  server.on("/mqttUst", handle_mqtt_ust);
  server.on("/mqttTopic", handle_mqtt_topic);
  server.on("/led", handle_led);
  server.on("/led2", handle_led2);
  server.on("/restart", handle_Restart);
  server.on("/resetConfig", handle_resetConfig);
  httpUpdater.setup(&server);
  server.begin();
}
//======================================================================================================
void handle_ConfigJSON(){
  String json="{"; 
  json += "\"ssid\":\"";
  json += ssid;
  json += "\",\"password\":\"";
  json += password;
  json += "\",\"ssidAP\":\"";
  json += ssidAP;
  json += "\",\"passwordAP\":\"";
  json += passwordAP;
  json += "\",\"ip\":\"";
  json += WiFi.localIP().toString();
  json += "\",\"ntpServerName\":\"";
  json += ntpServerName;
  json += "\",\"timeZone\":\"";
  json += timeZone;
  json += "\",\"weatherKey\":\"";
  json += weatherKey;  
  json += "\",\"weatherHost\":\"";
  json += weatherHost;
  json += "\",\"cityID1\":\"";
  json += cityID1;
  json += "\",\"cityID2\":\"";
  json += cityID2;
  json += "\",\"forecastView\":\"";
  json += forecastView;
  json += "\",\"forecastViewStart\":\"";
  json += forecastViewStart;
  json += "\",\"forecastViewStop\":\"";
  json += forecastViewStop;
  json += "\",\"time\":\"";
  json += (String(hour)+":"+(minute<10?"0":"")+String(minute)+":"+(second<10?"0":"")+String(second));
  json += "\",\"updateOTA\":\"";
  json += updateOTA;
  json += "\",\"tbd\":\"";
  json += timeBrightnessD;
  json += "\",\"vbd\":\"";
  json += volBrightnessD;
  json += "\",\"tbn\":\"";
  json += timeBrightnessN;
  json += "\",\"vbn\":\"";
  json += volBrightnessN;
  json += "\",\"ds18b20\":\"";
  json += ds18b20;
  json += "\",\"ds18b20Out\":\"";
  json += ds18b20Out;
  json += "\",\"speedLine\":\"";
  json += 100 - speedLine;
  json += "\",\"rotate\":\"";
  json += rotate;
  json += "\",\"mqtt_server\":\"";
  json += mqtt_server;
  json += "\",\"mqtt_port\":\"";
  json += mqtt_port;
  json += "\",\"mqtt_user\":\"";
  json += mqtt_user;
  json += "\",\"mqtt_pass\":\"";
  json += mqtt_pass;
  json += "\",\"mqtt_name\":\"";
  json += mqtt_name;
  json += "\",\"mqtt_sub\":\"";
  json += mqtt_sub;
  json += "\",\"mqtt_sub_inform\":\"";
  json += mqtt_sub_inform;
  json += "\",\"mqtt_pub\":\"";
  json += mqtt_pub;
  json += "\"}";
  server.send(200, "text/json", json);
}
//======================================================================================================
void handle_Set_Ssid() {
  ssid=server.arg("ssid").c_str();
  password=server.arg("password").c_str();
  saveConfig();
  server.send(200, "text/plain", "OK");
}
//======================================================================================================
void handle_Set_Ssidap() {
  ssidAP=server.arg("ssidAP").c_str();
  passwordAP=server.arg("passwordAP").c_str();
  saveConfig();
  server.send(200, "text/plain", "OK");
}
//======================================================================================================
void handle_ntpServerName() {
  ntpServerName=server.arg("ntpServerName").c_str();
  timeUpdateNTP();
  saveConfig();
  server.send(200, "text/plain", "OK");
}
//======================================================================================================
void handle_time_zone() {
  timeZone=server.arg("timeZone").toInt();
  timeUpdateNTP();
  saveConfig();
  server.send(200, "text/plain", "OK"); 
}
//======================================================================================================
void handle_weatherKey() {
  weatherKey=server.arg("weatherKey").c_str();
  saveConfig();
  server.send(200, "text/plain", "OK");
}
//======================================================================================================
void handle_weather_update() {
  String updateWeather=server.arg("update");
  if(updateWeather == "ok") {
    weaterUpdate = true; 
    Serial.println("Update weather!");
    server.send(200, "text/plain", "UPDATE");
  } else server.send(403, "text/plain", "ERROR");
}
//======================================================================================================
void handle_weatherHost() {
  weatherHost=server.arg("weatherHost").c_str();
  saveConfig();
  server.send(200, "text/plain", "OK");
}
//======================================================================================================
void handle_cityID() {
  cityID1 = server.arg("cityID1").c_str();
  cityID2 = server.arg("cityID2").c_str();
  saveConfig();
  server.send(200, "text/plain", "OK");
}
//======================================================================================================
void handle_forecastView() {
  forecastView = server.arg("forecastView").toInt();
  int i = server.arg("forecastViewStart").toInt();
  int j = server.arg("forecastViewStop").toInt();
  if(i >= j || i < 0 || i > 23 || j < 0 || j > 23) {
    server.send(403, "text/plain", "ВІДМОВА");
    return;
  }
  forecastViewStart = i;
  forecastViewStop = j;
  saveConfig();
  server.send(200, "text/plain", "OK");
}
//======================================================================================================
void handle_updateOta() {
  updateOTA=server.arg("updateOTA").toInt();
  saveConfig(); 
  server.send(200, "text/plain", "OK");
  delay(100);
  ESP.restart();
}  
//======================================================================================================
void handle_led() {
  int i = server.arg("tbd").toInt();
  int j = server.arg("vbd").toInt();
  int ii = server.arg("tbn").toInt();
  int jj = server.arg("vbn").toInt();
  if(i >= ii || i < 0 || i > 23 || ii < 0 || ii > 23 || j < 0 || j >15 || jj < 0 || jj > 15) {
    server.send(403, "text/plain", "ВІДМОВА");
    return;
  }
  timeBrightnessD = i;
  volBrightnessD = j;
  timeBrightnessN = ii;
  volBrightnessN = jj;
  server.send(200, "text/plain", "OK");
  saveConfig(); 
}
//======================================================================================================
void handle_led2() {
  ds18b20 = server.arg("ds18b20").toInt();
  ds18b20Out = server.arg("ds18b20Out").toInt();
  rotate = 0 + (90 * (server.arg("rotate").toInt() == 90)) + (270 * (server.arg("rotate").toInt() == 270));
  int i = server.arg("speedLine").toInt();
  if(i > 0 && i < 100) speedLine = 100 - i;
  server.send(200, "text/plain", "OK");
  saveConfig(); 
}
//======================================================================================================
void handle_mqtt_ust() {
  snprintf(mqtt_server, 20, "%s", server.arg("mqtt_server").c_str());
  mqtt_port = server.arg("mqtt_port").toInt();                                                    //
  snprintf(mqtt_user, 30, "%s", server.arg("mqtt_user").c_str());                                 //
  snprintf(mqtt_pass, 20, "%s", server.arg("mqtt_pass").c_str());                                 //
  reconnect();
  saveConfig(); 
  server.send(200, "text/plain", "UPDATE SET MQTT");
}
//======================================================================================================
void handle_mqtt_topic() {  
  snprintf(mqtt_name, 20, "%s", server.arg("mqtt_name").c_str());                                 //
  snprintf(mqtt_pub, 20, "%s", server.arg("mqtt_pub").c_str());                                   //
  snprintf(mqtt_sub, 20, "%s", server.arg("mqtt_sub").c_str());                                   //
  snprintf(mqtt_sub_inform, 20, "%s", server.arg("mqtt_sub_inform").c_str());                     //
  reconnect();
  saveConfig(); 
  server.send(200, "text/plain", "UPDATE TOPIC MQTT");
}
//======================================================================================================
void handle_resetConfig() {
  String restart=server.arg("device");
  if(restart=="ok") {
    SPIFFS.format();
    Serial.println(ESP.eraseConfig());
    delay(3000);
    server.send(200, "text/plain", "DEL CONFIG");
    delay(3000);
    ESP.reset();
  }
}
//======================================================================================================
void handle_Restart() {
  String restart=server.arg("device");
  if(restart=="ok") {
    server.send(200, "text/plain", "OK"); 
    ESP.restart();
  }
}
