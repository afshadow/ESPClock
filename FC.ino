bool loadConfig() {
  File configFile = SPIFFS.open("/config.json", "r");
  if(!configFile) {
    if(printCom) Serial.println("Failed to open config file");
    saveConfig();
    configFile.close();
    return false;
  }
  size_t size = configFile.size();
  if(size > 1024) {
    if(printCom) Serial.println("Config file size is too large");
    configFile.close();
    return false;
  }
  jsonConfig = configFile.readString();
  DynamicJsonBuffer jsonBuffer; 
  configFile.close();
  JsonObject& root = jsonBuffer.parseObject(jsonConfig);
  ssidAP = root["ssidAP"].as<String>();
  passwordAP = root["asswordAP"].as<String>();
  ssid = root["ssid"].as<String>();
  password = root["password"].as<String>();
  ntpServerName = root["ntpServerName"].as<String>();
  timeZone = root["timeZone"];
  weatherKey = root["weatherKey"].as<String>();
  weatherHost = root["weatherHost"].as<String>();
  cityID1 = root["cityID1"].as<String>();
  cityID2 = root["cityID2"].as<String>();
  forecastView = root["forecastView"];
  forecastViewStart = root["forecastViewStart"];
  forecastViewStop = root["forecastViewStop"];
  speedLine = root["speedLine"];
  ds18b20 = root["ds18b20"];
  ds18b20Out = root["ds18b20Out"];
  rotate = root["rotate"];
  updateOTA = root["updateOTA"];
  volBrightnessD = root["volBrightnessD"];
  volBrightnessN = root["volBrightnessN"];
  timeBrightnessD = root["timeBrightnessD"];
  timeBrightnessN = root["timeBrightnessN"];
  snprintf(mqtt_server, 20, "%s", (root["mqtt_server"].as<String>()).c_str());
  mqtt_port = root["mqtt_port"];
  snprintf(mqtt_user, 30, "%s", (root["mqtt_user"].as<String>()).c_str());
  snprintf(mqtt_pass, 20, "%s", (root["mqtt_pass"].as<String>()).c_str());
  snprintf(mqtt_name, 20, "%s", (root["mqtt_name"].as<String>()).c_str());
  snprintf(mqtt_pub, 20, "%s", (root["mqtt_pub"].as<String>()).c_str());
  snprintf(mqtt_sub, 20, "%s", (root["mqtt_sub"].as<String>()).c_str());
  snprintf(mqtt_sub_inform, 20, "%s", (root["mqtt_sub_inform"].as<String>()).c_str());
  if(printCom) Serial.println("\r\n Load Config");
  if(printCom) Serial.println(jsonConfig);
  return true;
}
//=================================================================
bool saveConfig() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(jsonConfig);
  json["ssidAP"] = ssidAP;
  json["passwordAP"] = passwordAP;
  json["ssid"] = ssid;
  json["password"] = password;
  json["ntpServerName"] = ntpServerName;
  json["timeZone"] = timeZone;
  json["weatherKey"] = weatherKey;
  json["weatherHost"] = weatherHost;
  json["cityID1"] = cityID1;
  json["cityID2"] = cityID2;
  json["forecastView"] = forecastView;
  json["forecastViewStart"] = forecastViewStart;
  json["forecastViewStop"] = forecastViewStop;
  json["speedLine"] = speedLine;
  json["ds18b20"] = ds18b20;
  json["ds18b20Out"] = ds18b20Out;
  json["rotate"] = rotate;
  json["updateOTA"] = updateOTA;
  json["volBrightnessD"] = volBrightnessD;
  json["volBrightnessN"] = volBrightnessN;
  json["timeBrightnessD"] = timeBrightnessD;
  json["timeBrightnessN"] = timeBrightnessN;
  json["mqtt_server"] = mqtt_server;
  json["mqtt_port"] = mqtt_port;
  json["mqtt_user"] = mqtt_user;
  json["mqtt_pass"] = mqtt_pass;
  json["mqtt_name"] = mqtt_name;
  json["mqtt_pub"] = mqtt_pub;
  json["mqtt_sub"] = mqtt_sub;
  json["mqtt_sub_inform"] = mqtt_sub_inform;
  json.printTo(jsonConfig);
  File configFile=SPIFFS.open("/config.json", "w");
  if(!configFile) {
    configFile.close();
    return false;
  }
  json.printTo(configFile);
  if(printCom) Serial.println("\r\n Save Config");
  if(printCom) Serial.println(jsonConfig);
  configFile.close();
  return true;
}
