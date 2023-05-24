/*_______By IvanZah___________
 Піни LED------NodeMCU  1.0  |
                             |
  DataIn__________D7         |
  LOAD/CS_________D8         |
  CLK_____________D5         |
_____________________________|
*/

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h> 
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPUpdateServer.h> 
#include <OneWire.h>
#include <PubSubClient.h>

ESP8266HTTPUpdateServer httpUpdater;
WiFiClient ESPclient;
PubSubClient MQTTclient(ESPclient);
ESP8266WebServer server(80);
File fsUploadFile;
IPAddress apIP(192, 168, 4, 1);

#include "P_funct.js.h"
#include "P_style.css.h"
#include "P_boot.css.h"
#include "P_edit.h"
#include "P_index.h"
#include "P_time.h"
#include "P_weat.h"
#include "P_led.h"
#include "P_update.h"
#include "P_help.h"
#include "P_MQTT.h"
// =======================================================================
String ssid = "fhome";
String password = "2808197442";
String ssidAP = "WiFi-Clock";
String passwordAP = "";
String weatherKey = "*******************************";
String weatherLang = "en";
String cityID1 = "703845";   // Kryvyy Rih
String cityID2 = "3100946";
// ----------змінні для роботи з mqtt сервером
char mqtt_server[21] = "m13.cloudmqtt.com";
int  mqtt_port = 13011;
char mqtt_user[31] = "********";
char mqtt_pass[21] = "************";
char mqtt_name[21] = "ESP-12_informer";
char mqtt_sub_inform[21] = "Inform/mess";
char mqtt_sub[21] = "Ulica/temp";
char mqtt_pub[21] = "ClockInformer/temp";
// =====================================================================================
#define printCom 1
#define MAX_DIGITS 16
#define NUM_MAX 4

//Сохраняем яркость
uint8_t volBrightnessD  = 8;
uint8_t volBrightnessN  = 2;
uint8_t timeBrightnessD = 7;
uint8_t timeBrightnessN = 23;
int rotate = 90;

#define D3        10
#define DIN_PIN   13  // D7
#define CS_PIN    15  // D8
#define CLK_PIN   14  // D5
#define buzzerPin 12  // D6
OneWire  ds(D3);      // DS18B20 подключен к 10 пину (резистор на 4.7к обязателен)

#include "max7219.h"
#include "fonts.h"
boolean WIFI_connected = false;
// Настройка Оновлення часу
IPAddress timeServerIP;
String ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[ NTP_PACKET_SIZE];
WiFiUDP udp;
unsigned int localPort = 2390;
static const uint8_t monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
#define LEAP_YEAR(Y) (((1970 + Y) > 0 ) && !((1970 + Y) % 4) && (((1970 + Y) % 100) || !((1970 + Y) % 400)))
String weatherHost = "api.openweathermap.org";
String cityID;
String weatherDescription = "";
String country;
int humidity;
float pressure;
float temp;
String tempz;
int clouds;
int windDeg;
float windSpeed;
String dw, _month;
String jsonConfig = "{}";
//----------змінні для роботи локального годинника
int timeZone = 1;
bool isDayLightSaving = false;
long localEpoc = 0;
long localMillisAtUpdate = 0;
int hour = 7, minute = 52, second = 0, month = 6, day = 14,dayOfWeek = 4;
int year = 2017;
bool statusUpdateNtpTime = 0;
String y, mon, wd, d, h, m, s, mes;
uint8_t hourTest[3], minuteTest[3];
int secFr, lastSecond;
uint8_t speedLine = 40;
String date;
String cityName;
int cityId;
String weatherString;
String weatherStringZ;
String weatherStringCity1 = "Нема прогнозу для Міста #1";
String weatherStringCity2 = "Нема прогнозу для Міста #2";
bool weaterUpdate = true;  
byte dig[MAX_DIGITS] = {0};
byte digold[MAX_DIGITS] = {0};
byte digtrans[MAX_DIGITS] = {0};
int dx = 0;
int dy = 0;
byte del = 0;
int updateOTA = 1;
bool ds18b20 = true;
bool ds18b20Out = true;
bool forecastView = true;
uint8_t forecastViewStart = 7; 
uint8_t forecastViewStop = 23;
bool updateForecast = 0;
uint8_t updateForecastCity1 = 2;
uint8_t updateForecastCity2 = 2;
int t1 = 0;
int t2 = 0;
int t3 = 0;
int t4 = 0;
int t5 = 1;
float celsius;

//==============================================================================================
void setup() {
  Serial.begin(115200);
 // initMAX7219();
  sendCmdAll(CMD_SHUTDOWN, 1);
  sendCmdAll(CMD_INTENSITY, 1); 
 // temper();
  SPIFFS.begin();
//***********HTTP сторінки для налаштуваня ESP
  server.on("/", [](){Serial.println("admin.html"); server.send_P(200, "text/html", PAGE_WiFiSettings);});
  server.on("/time.htm", [](){Serial.println("time.html"); server.send_P(200, "text/html", PAGE_TimeSettings);}); 
  server.on("/weather.htm", [](){Serial.println("weather.html"); server.send_P(200, "text/html", PAGE_WertherSettings);});
  server.on("/mqtt.htm", [](){Serial.println("mqtt.html");server.send_P(200, "text/html", PAGE_MQTTSettings);});
  server.on("/led.htm", [](){Serial.println("led.html"); server.send_P(200, "text/html", PAGE_LedSettings);});
  server.on("/update.htm", [](){Serial.println("update.html"); server.send_P(200, "text/html", PAGE_UpdateSettings);});
  server.on("/help.htm", [](){Serial.println("help.htm"); server.send_P(200, "text/html", PAGE_Help);});
  server.on("/edit.htm", [](){Serial.println("help.htm"); server.send_P(200, "text/html", PAGE_Edit);});
  server.on("/style.css", [](){Serial.println("style.css"); server.send_P ( 200, "text/plain", PAGE_Style_css);});
  server.on("/function.js", [](){Serial.println("microajax.js"); server.send_P ( 200, "text/plain", PAGE_function_js);});
  server.on("/bootstrap.min.css", [](){Serial.println("style.css"); server.send_P ( 200, "text/plain", PAGE_bootstrap_css);});
  server.on("/favicon.ico", [](){Serial.println("favicon.ico"); server.send(200, "text/html", "");});
  server.onNotFound([]() {(404, "text/plain", "FileNotFound");});
//************
  loadConfig();
  localMillisAtUpdate = millis();
  localEpoc = (hour * 60 * 60 + minute * 60 + second);
  udp.begin(localPort);
  pinMode(buzzerPin, OUTPUT);
//************ WiFi connect
  wifiConnect();
  if(printCom) Serial.println("");
  if(printCom) Serial.print("Connected: "); 
  if(printCom) Serial.println(WiFi.localIP());
// ***********  OTA SETUP
  if(updateOTA) {
    ArduinoOTA.setPort(8266);
    ArduinoOTA.setHostname("ESP-Clock"); 
    ArduinoOTA.onEnd([](){ESP.restart();});
    ArduinoOTA.onError([](ota_error_t error){Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR && printCom) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR && printCom) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR && printCom) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR && printCom) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR && printCom) Serial.println("End Failed");
      ESP.restart(); 
    });
    ArduinoOTA.begin();
  }
  server_init();
  temper();
// *********** MQTT client
  MQTTclient.setServer(mqtt_server, mqtt_port);
  MQTTclient.setCallback(callback);
  MQTTclient.connect(mqtt_name);
  MQTTclient.subscribe(mqtt_sub_inform);
  MQTTclient.subscribe(mqtt_sub);   
}
//======================================================================================
void callback(char* topic, byte* payload, unsigned int length) {
  if(String(topic) == "Inform/mess") {
    String Text = "        ";
    for(int i = 0; i < length; i++) {
      Text += ((char)payload[i]);
    }
    Text += "        ";
    for(int i = 0; i < 4; i++) {
      tone(buzzerPin, 2000, 100);
      delay(200);
    }
    printStringWithShift(Text.c_str(), 30);
  }
  if(String(topic) == mqtt_sub){
    t3 = 0;
    t4 = 0;
    if((payload[0] >= 48 && payload[0] < 58) || payload[0] == 45) {
      if(payload[0] == 45) {
        t5 = -1;
        if(payload[1] >= 48 && payload[1] < 58) {
          t3 = payload[1] - 48;
          if(payload[2] >= 48 && payload[2] < 58) {
            t3 = t3 * 10 + (payload[2] - 48);
          }
        }
        if(payload[3] == 46) {
          if(payload[4] >= 48 && payload[4] < 58) t4 = payload[4] - 48;
        }
        if(payload[2] == 46) {
          if(payload[3] >= 48 && payload[3] < 58) t4 = payload[3] - 48;
        }
      } else {
        t5 = 1;
        t3 = payload[0] - 48;
        if(payload[1] >= 48 && payload[1] < 58) {
          t3 = t3 * 10 + (payload[1] - 48);
          if(payload[2] == 46) {
            if(payload[3] >= 48 && payload[3] < 58) t4 = payload[3] - 48;
          }
        }
        if(payload[1] == 46) {
          if(payload[2] >= 48 && payload[2] < 58) t4 = payload[2] - 48;
        }
      }
    }
  }
}
//======================================================================================
void reconnect(){
  if(!ESPclient.connected() && WiFi.status() == WL_CONNECTED) {
    Serial.print("Attempting MQTT connection...");
    if(MQTTclient.connect(mqtt_name, mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      MQTTclient.subscribe(mqtt_sub_inform);
      MQTTclient.subscribe(mqtt_sub);
    } else {
      Serial.print("failed, rc= ");
      Serial.println(MQTTclient.state());
    }
  }
}
//======================================================================================
//======================================================================================
void loop() {
  if(updateOTA) ArduinoOTA.handle();
  server.handleClient();
  updateTime();
  if(second != lastSecond) {
    lastSecond = second;
    secFr = 0;
    if(hour >= timeBrightnessD && hour < timeBrightnessN) sendCmdAll(CMD_INTENSITY, volBrightnessD);
    else sendCmdAll(CMD_INTENSITY, volBrightnessN);
  } else secFr++;
  if(minute == 0 && second == 0 && secFr == 0 && (hour >= 7 && hour <= 23)) {
    tone(buzzerPin, 2000, 40);
    delay(200);
    tone(buzzerPin, 2000, 40);
  }
  if(hour == 22 && minute == 55) {
    printStringWithShift("       22:55 \200\200\200 Моїй коханій Зайченяткі \200\200\200", speedLine);
    return;
  }
// ---------- 10-30 секунда ------------------------------------------------------------
  if(forecastView && forecastViewStart <= hour && forecastViewStop > hour) {
    if(second == 10 && secFr == 0 && forecastView) {
      temper();
      if(minute % (1 + (1 * cityID1.toInt() > 0) + (1 * cityID2.toInt() > 0)) == 0) {
        date = "     " + dw + ", " + String(day) + " " + _month + " " + String(year) + "р.           ";
        printStringWithShift(date.c_str(), speedLine);
      } else if(minute % (1 + (1 * cityID1.toInt() > 0) + (1 * cityID2.toInt() > 0)) == 1 && cityID1.toInt() > 0) {
        printStringWithShift(weatherStringCity1.c_str(), speedLine);
      } else if(cityID2.toInt() > 0) {
        printStringWithShift(weatherStringCity2.c_str(), speedLine);
      }
    }
  }
  if(second == 10 && secFr == 0 && (forecastViewStart > hour || forecastViewStop <= hour || forecastView == 0)) {
    temper();
    showSimpleDate();
  }
  // ---------- 0-39, 40-50, 51-59 секунда ---------------------------------------------
  if(second < 10 || second > 15 && second < (40 + 5 * !ds18b20) || second >= (50 - 5 * !ds18b20Out)) {
    if(millis() % 50 == 0) showAnimClock();
  }
  else if(second >= 40 && second < 45){
    showSimpleTemp();
  }
  else if(second >= 45 && second < 50) showSimpleTempU();
  // ---------- 41 перевірка доступності WiFi мережі -----------------------------------
  if(second == 41 && secFr == 00 && WiFi.status() != WL_CONNECTED && WIFI_connected != false) {
    WIFI_connected = false;
  }
  // ---------- 42 сек. повторне підключення до WiFi кожну 1, 6, 11, 16...56 хвилину ---
  if(!WIFI_connected && second == 42 && (minute % 5 == 1)) {
    wifiConnect();
  }
  // ---------- якщо мережа WiFi доступна то виконуємо наступні функції ----------------------------
  if(WIFI_connected) {
    MQTTclient.loop();
    // ---------- 43 сек. Оновлення мережевого часу -------------------------------------
    if((statusUpdateNtpTime == 0 && second == 43) || (minute == 5 && second == 43)) timeUpdateNTP();
    // ---------- 45 сек. перевірка доступності MQTT та публікація температури ---------
    if(second == 45 && secFr == 0) {
      if(!MQTTclient.connected()) {
        reconnect();
      }
      if(MQTTclient.connected()) {
        temper();
        MQTTclient.publish(mqtt_pub, (String(celsius)).c_str());
        if(printCom) Serial.print("Publish in topic Temperature: ");
        if(printCom) Serial.println(celsius);
      }
    }
    // ---------- 47 cек. оновлюємо прогноз погоди ------------------------------------- 
    if(forecastView && (cityID1 != "0" || cityID2 != "0")) {
      if(second == 47 && secFr == 0 && (minute == 15 || minute == 45 || updateForecastCity1 != 0 || updateForecastCity2 != 0) || weaterUpdate == true) {
        if((updateForecastCity1 != 0 || minute == 15 || minute == 45 || weaterUpdate == true) && cityID1 != "0") {
          cityID = cityID1;
          getWeatherData();
          getWeatherDataz();
          if(updateForecast == 0) {
            updateForecastCity1 = 0;
          } else updateForecastCity1++;
          if(updateForecastCity1 > 239) {
            weatherStringCity1 = "Нема прогнозу для Міста #1 більше 4-х годин!";
          } else weatherStringCity1 = weatherString + weatherStringZ;
        }
        if((updateForecastCity2 != 0 || minute == 15 || minute == 45 || weaterUpdate == true) && cityID2 != "0") {
          cityID = cityID2;
          getWeatherData();
          getWeatherDataz();
          if(updateForecast == 0) {
            updateForecastCity2 = 0;
          } else updateForecastCity2++;
          if(updateForecastCity2 > 239) {
            weatherStringCity2 = "Нема прогнозу для Міста #2 більше 4-х годин!";
          } else weatherStringCity2 = weatherString + weatherStringZ;
        }
        if(!MQTTclient.connected()) reconnect();
        weaterUpdate = false;
      }
    }
  }
}
//======================================================================================
//==========ВИВІД НА ЕКРАН ТЕМПЕРАТУРИ В БУДИНКУ========================================
void showSimpleTemp() {
  dx = dy = 0;
  clr();
  showDigit((t1 < 0 ? 14 : 13), 0, dig5x8rn);
  if(t1 <= -10.0 || t1 >= 10) showDigit((t1 < 0 ? (t1 * -1) / 10 : t1 / 10), 4, dig5x8rn);
  showDigit((t1 < 0 ? (t1 * -1) % 10 : t1 % 10), 10, dig5x8rn);
  showDigit(12, 16, dig5x8rn);
  showDigit(t2, 18, dig5x8rn);
  showDigit(10, 24, dig5x8rn);
  showDigit(11, 27, dig5x8rn);
  refreshAll();
}
//==========ВИВІД НА ЕКРАН ТЕМПЕРАТУРИ НА ВУЛИЦІ========================================
void showSimpleTempU() {
  dx = dy = 0;
  clr();
  showDigit((t5 < 0 ? 16 : 15), 0, dig5x8rn);
  if(t3 >= 10) showDigit(t3 / 10, 4, dig5x8rn); 
  showDigit(t3 % 10, 10, dig5x8rn);
  showDigit(12, 16, dig5x8rn);
  showDigit(t4, 18, dig5x8rn);
  showDigit(10, 24, dig5x8rn);
  showDigit(11, 27, dig5x8rn);
  refreshAll();
}
//==========ВИВІД НА ЕКРАН ДАТИ=========================================================

void showSimpleDate() { 
  dx = dy = 0;
  clr();
  showDigit(day / 10, 0, dig4x8);
  showDigit(day % 10,  5, dig4x8);
  showDigit(month / 10, 12, dig4x8);
  showDigit(month % 10, 17, dig4x8);
  showDigit((year - 2000) / 10, 23, dig4x8);
  showDigit((year - 2000) % 10, 28, dig4x8);
  setCol(10, 0xC0);
  setCol(22, 0xC0);
  refreshAll();
}
//==========ВИВІД НА ЕКРАН АНІМАЦІЙНОГО ГОДИННИКА=======================================
void showAnimClock() {
  byte digPos[4] = {1, 8, 18, 25};
  int digHt = 16;
  int num = 4;
  int i;
  if(del == 0) {
    del = digHt;
    for(i = 0; i < num; i++) digold[i] = dig[i];
    dig[0] = hour / 10 ? hour / 10 : 10;
    dig[1] = hour % 10;
    dig[2] = minute / 10;
    dig[3] = minute % 10;
    for(i = 0; i < num; i++)  digtrans[i] = (dig[i] == digold[i]) ? 0 : digHt;
  } else del--;
  clr();
  for(i = 0; i < num; i++){
    if(digtrans[i] == 0) {
      dy = 0;
      showDigit(dig[i], digPos[i], dig6x8);
    } else {
      dy = digHt-digtrans[i];
      showDigit(digold[i], digPos[i], dig6x8);
      dy =- digtrans[i];
      showDigit(dig[i], digPos[i], dig6x8);
      digtrans[i]--;
    }
  }
  dy = 0;
  int flash = millis()%1000;
  if((flash >= 180 && flash < 360) || flash >= 540){
    setCol(15, WIFI_connected ? 0x66 : 0x60);
    setCol(16, WIFI_connected ? 0x66 : 0x60);
  }
  if(statusUpdateNtpTime) {
    if(flash >= 0 && flash < 180){
      setCol(15, WIFI_connected ? 0x24 : 0x20);
      setCol(16, WIFI_connected ? 0x42 : 0x40);
    }
    if(flash >= 360 && flash < 540){
      setCol(15, WIFI_connected ? 0x42 : 0x40);
      setCol(16, WIFI_connected ? 0x24 : 0x20);
    }
  }
  if(updateForecastCity1 != 0 && forecastView && cityID1 != "0") setCol(00, flash < 500 ? 0x80 : 0x00);
  if(updateForecastCity2 != 0 && forecastView && cityID2 != "0") setCol(31, flash < 500 ? 0x80 : 0x00);
  refreshAll();
}
//=================================================
void showAnimWifi(byte probaWifi) {
  byte digPos[2] = {18, 25};
  int digHt = 16;
  int num = 2;
  int ii;
  if(del == 0) {
    del = digHt;
    for(ii = 0; ii < num; ii++) digold[ii] = dig[ii];
    dig[0] = probaWifi / 10 ? probaWifi / 10 : 10;
    dig[1] = probaWifi % 10;
    for(ii = 0; ii < num; ii++)  digtrans[ii] = (dig[ii] == digold[ii]) ? 0 : digHt;
  } else del--;
  clr();
  for(ii = 0; ii < num; ii++){
    if(digtrans[ii] == 0) {
      dy = 0;
      showDigit(dig[ii], digPos[ii], dig6x8);
    } else {
      dy = digHt-digtrans[ii];
      showDigit(digold[ii], digPos[ii], dig6x8);
      dy =- digtrans[ii];
      showDigit(dig[ii], digPos[ii], dig6x8);
      digtrans[ii]--;
    }
  }
  dy = 0;
  refreshAll();
}
//======================================================================================
void showDigit(char ch, int col, const uint8_t *data) {
  if(dy < - 8 | dy > 8) return;
  int len = pgm_read_byte(data);
  int w = pgm_read_byte(data + 1 + ch * len);
  col += dx;
  for(int i = 0; i < w; i++){
    if(col + i >= 0 && col + i < 8 * NUM_MAX) {
      byte v = pgm_read_byte(data + 1 + ch * len + 1 + i);
      if(!dy) scr[col + i] = v; else scr[col + i] |= dy > 0 ? v >> dy : v << -dy;
    }
  }
}
//======================================================================================
void setCol(int col, byte v) {
  if(dy <- 8 | dy > 8) return;
  col += dx;
  if(col >= 0 && col < 8 * NUM_MAX){
    if(!dy) scr[col] = v; else scr[col] |= dy > 0 ? v >> dy : v << -dy;
  }
}
//======================================================================================
int showChar(char ch, const uint8_t *data) {
  int len = pgm_read_byte(data);
  int i,w = pgm_read_byte(data + 1 + ch * len);
  for(i = 0; i < w; i++)
    scr[NUM_MAX * 8 + i] = pgm_read_byte(data + 1 + ch * len + 1 + i);
    scr[NUM_MAX * 8 + i] = 0;
  return w;
}
//==========КОНВЕРТАЦІЯ СИМВОЛІВ В РАЗІ ВИКОРИСТАННЯ УКРАЇНСЬКИХ ЛІТЕР==================
int dualChar = 0;
unsigned char convertUA(unsigned char _c) {
  unsigned char c = _c;
  if(c == 208) {
    dualChar = 1;
    return 0;
  } else if(c == 209 || c == 210) {
    dualChar = 2;
    return 0;
  }
  if(c == 32 && dualChar != 3) {
    dualChar = 3;
    return c;
  }
  if(dualChar == 1) {
    if(c >= 144 && c < 192) c += 48;
    dualChar = 0;
    return c;
  }
  if(dualChar == 2) {
    if(c >= 128 && c < 144) c += 112;
    switch(_c) {
      case 144: c = 133; break;
      case 145: c = 149; break;
    }
    dualChar = 0;
    return c;
  }
  return c;
}
//==========ДРУКУВАННЯ БІГУЧОГО СИМВОЛУ с - символ, shiftDelay - швидкість=====================================
void printCharWithShift(unsigned char c, int shiftDelay) {
  c=convertUA(c);
  if(c < ' ') return;
  c -= 32;
  int w = showChar(c, fontUA);
  for(int i = 0; i < w + 1; i++){
    delay(shiftDelay);
    scrollLeft();
    refreshAll();
  }
}
//==========ДРУКУВАННЯ БІГУЧОЇ СТРОКИ *s - текст, shiftDelay - швидкість==========================================
void printStringWithShift(const char* s, int shiftDelay) {
  while(*s) {
    printCharWithShift(*s, shiftDelay);
    s++;
    server.handleClient();
    MQTTclient.loop();
  }
}

//==========Оновлення ЛОКАЛЬНОГО ЧАСУ (ЛОКАЛЬНІ ЧАСИ)===============================================================
void updateTime() {
  long curEpoch = localEpoc + ((millis() - localMillisAtUpdate) / 1000);
  long roundEpoch = round(curEpoch + 86400L);
  long epoch = roundEpoch % 86400L;
  hour = ((epoch % 86400L) / 3600) % 24;
  minute = (epoch % 3600) / 60;
  second = epoch % 60;
}
//==========Оновлення МЕРЕЖЕВОГО ЧАСУ (перевірка в три проходи)====================================================
void timeUpdateNTP() {
  if(!WIFI_connected) return;
  statusUpdateNtpTime = 1;
  for(int timeTest = 0; timeTest < 3; timeTest++) {
    if(printCom) Serial.print("Proba #");
    if(printCom) Serial.println(timeTest + 1);
    updateTime();
    getNTPtime();
    hourTest[timeTest] = hour;
    minuteTest[timeTest] = minute;
    if(statusUpdateNtpTime == 0) {
      if(printCom) Serial.print("ERROR TIME!!!\r\n");
        return;
    }
    if(timeTest > 0) {
      if((hourTest[timeTest] != hourTest[timeTest - 1] || minuteTest[timeTest] != minuteTest[timeTest - 1])) {
        statusUpdateNtpTime = 0;
        if(printCom) Serial.print("ERROR TIME!!!\r\n");
        return;
      }
    }
  }
  statusUpdateNtpTime = 1;
  localMillisAtUpdate = millis();
  localEpoc = (hour * 60 * 60 + minute * 60 + second);
  convertDw();
  convertMonth();
  date = "     " + dw + ", " + String(day) + " " + _month + " " + String(year) + "р.          ";
  if(printCom) Serial.print(String(hour) + ":" + String(minute) + ":" + String(second) + " ");
  if(printCom) Serial.print(String(day) + "-" + String(month) + "-" + String(year) + " DW=" + String(dayOfWeek) + "\r\n");
  if(printCom) Serial.println("Time update OK");
}
//==========ОТРИМАННЯ ДАТИ ТА ЧАСУ ВІД СЕРВЕРА ТОЧНОГО ЧАСУ =============================================================
void getNTPtime() {
  WiFi.hostByName(ntpServerName.c_str(), timeServerIP); 
  int cb;
  for(int i = 0; i < 3; i++){
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    packetBuffer[0] = 0b11100011;
    packetBuffer[1] = 0;
    packetBuffer[2] = 6;
    packetBuffer[3] = 0xEC;
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;
    udp.beginPacket(timeServerIP, 123);
    udp.write(packetBuffer, NTP_PACKET_SIZE);
    udp.endPacket();
    delay(800);
    cb = udp.parsePacket();
    if(!cb && printCom) Serial.println("no packet yet..." + String (i + 1)); 
    if(!cb && i == 2) {
      statusUpdateNtpTime = 0;
      return;
    }
    if(cb) i = 3;
  }
  if(cb) {
    udp.read(packetBuffer, NTP_PACKET_SIZE);
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;        // Unix час станом на 1 січня 1970. в секундах, то 2208988800:
    unsigned long epoch = secsSince1900 - seventyYears;
    boolean summerTime;
    if(month < 3 || month > 10) summerTime = false;
    if(month > 3 && month < 10) summerTime = true;
    if(month == 3 && (hour + 24 * day) >= (3 + 24 * (31 - (5 * year / 4 + 4) % 7)) || month == 10 && (hour + 24 * day) < (3 + 24 * (31 - (5 * year / 4 + 1) % 7))) summerTime = true; 
    epoch = epoch + timeZone * 3600 + (3600 * (isDayLightSaving && summerTime));      
    year = 0;
    int days = 0;
    uint32_t time;
    time = epoch / 86400;
    hour = (epoch % 86400L) / 3600;
    minute = (epoch % 3600) / 60;
    second = epoch % 60;
    dayOfWeek = (((time) + 4) % 7) + 1;
    while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
      year++;
    }
    days -= LEAP_YEAR(year) ? 366 : 365;
    time -= days;
    days = 0;
    month = 0;
    uint8_t monthLength = 0;
    for(month = 0; month < 12; month++) {
      if(month == 1) {
        if(LEAP_YEAR(year)) monthLength = 29;
        else monthLength = 28;
      }
      else monthLength = monthDays[month];
      if(time >= monthLength) time -= monthLength;
      else break;
    }
    month = month + 1;
    day = time + 1;
    year += 1970;
    return;
  }
  if(printCom) Serial.println("Nie ma czasu(((");
}

//===============================================================================================================================//
//                              БЕРЕМО ПОГОДУ З САЙТУ api.openweathermap.org                                                     //
//===============================================================================================================================//
void getWeatherData() {
  updateForecast = 0;
  if(printCom) Serial.print("Getting weather forecast for City-"); 
  if(printCom) Serial.println(cityID);
  weatherString = "       Для міста #" + cityID + " - не має погоди на зараз     ";
  if(ESPclient.connect(weatherHost.c_str(), 80)) {
    ESPclient.println(String("GET /data/2.5/weather?id=") + cityID + "&units=metric&appid=" + weatherKey + "&lang="+ weatherLang + "\r\n" +
                "Host: " + weatherHost + "\r\nUser-Agent: ArduinoWiFi/1.1\r\n" +
                "Connection: close\r\n\r\n");
  } else {
      updateForecast = 1;
    if(printCom) Serial.println("Not connection server!!!");
    return;
  }
  //---------------Якщо було підключення та запрос було відправлено, перевіряємо чи маємо відповідь (чекаємо 0,5с * 10 разів)------
  int repeatCounter = 0;
  while(!ESPclient.available() && repeatCounter < 10) {
    delay(500);
    if(printCom) {
      Serial.print("wait - ");
      Serial.println(repeatCounter);
    }
    repeatCounter++;
  }
  //---------------Якщо було підключення то була получена відповідь, то в змінну line записуємо отримані символи окрім ( [ та ] )--
  String line;
  while(ESPclient.connected() && ESPclient.available()) {
    char c = ESPclient.read(); 
    if(c == '[' || c == ']') c = ' ';
    line += c;
  }
  //---------------Зупеняемо роботу WiFi клієнта------------------------------------------------------------------------------
  ESPclient.stop();
  if(printCom) Serial.println(line);
  //---------------Зтворюємо json буфер, та парсимо в ньго дані зі змінної line--------------------------------------------------
  DynamicJsonBuffer jsonBuf;
  JsonObject &root = jsonBuf.parseObject(line);
  if(!root.success()){
      updateForecast = 1;
    if(printCom) Serial.println("Parse weather forecast - failed!!!");
    return;
  }
  //weatherMain = root["weather"]["main"].as<String>();
  weatherDescription = root["weather"]["description"].as<String>();
  weatherDescription.toLowerCase();
  convertWeatherDes();
  //country = root["sys"]["country"].as<String>();
  cityName = root ["name"].as<String>();
  cityId = root ["id"];
  convertCity();
  temp = root["main"]["temp"];
  humidity = root["main"]["humidity"];
  pressure = root["main"]["pressure"];
  pressure = (pressure / 1.3332239) - 24;
  windDeg = root["wind"]["deg"];
  windSpeed = root["wind"]["speed"];
  clouds = root["clouds"]["all"];
  String windDegString;
  if(windDeg >= 345 || windDeg <= 22) windDegString = "\211";    //"Північний";
  if(windDeg >= 23 && windDeg <= 68) windDegString = "\234";    //"Північно-східний";
  if(windDeg >= 69 && windDeg <= 114) windDegString = "\230";    //"Східний";
  if(windDeg >= 115 && windDeg <= 160) windDegString = "\235";    //"Південно-східний";
  if(windDeg >= 161 && windDeg <= 206) windDegString = "\210";    //"Південний";
  if(windDeg >= 207 && windDeg <= 252) windDegString = "\232";    //"Південно-західний";
  if(windDeg >= 253 && windDeg <= 298) windDegString = "\231";    //"Західний";
  if(windDeg >= 299 && windDeg <= 344) windDegString = "\233";    //"Північно-західний";
  weatherString = "         " + cityName + ", зараз:    \212 " + String(temp, 0) + ("\202") + "C   ";
  weatherString += "  \213 " + String(humidity) + "%   ";
  weatherString += "  \215 " + String(pressure, 0) + "ммРс   ";
  weatherString += "  \214 " + windDegString + " " + String(windSpeed, 1) + "м/с   ";
  weatherString += "  \216 " + String(clouds) + "%   ";
  weatherString += weatherDescription + "        ";
  if(printCom) Serial.println("Getting weather forecast - is OK.");
}

// =======================================================================//
//               Беремо ПРОГНОЗ!!! погоди з сайту openweathermap.org      //
// =======================================================================//
void getWeatherDataz() {
  if(printCom) Serial.println("Getting weather forecast for tomorrow");
  weatherStringZ = "Для міста #" + cityID + " - не має погоди на завтра     ";
  if(ESPclient.connect(weatherHost.c_str(), 80)) {
    ESPclient.println(String("GET /data/2.5/forecast/daily?id=") + cityID + "&units=metric&appid=" + weatherKey + "&lang=" + weatherLang + "&cnt=2" + "\r\n" +
                "Host: " + weatherHost + "\r\nUser-Agent: ArduinoWiFi/1.1\r\n" +
                "Connection: close\r\n\r\n");
  } else {
    updateForecast = 1;
    if(printCom) Serial.println("No connection server (for tomorrow)!!!");
    return;
  }
  String line;
  int  repeatCounter = 0;
  while(!ESPclient.available() && repeatCounter < 10) {
    delay(500);
    if(printCom) {
      Serial.print("wait - ");
      Serial.println(repeatCounter);
    }
    repeatCounter++;
  }
  while(ESPclient.connected() && ESPclient.available()) {
    char c = ESPclient.read(); 
    if(c == '[' || c == ']') c = ' ';
    line += c;
  }
  tvoday(line);
  ESPclient.stop();
  if(printCom) Serial.println(line);
  DynamicJsonBuffer jsonBuf;
  JsonObject &root = jsonBuf.parseObject(tempz);
  if(!root.success()){
    updateForecast = 1;
    if(printCom) Serial.println("Parse weather forecast for tomorrow - failed!!!");
    return;
  }
  //lon = root ["coord"]["lon"];
  //lat = root ["coord"]["lat"];
  float tempMin = root ["temp"]["min"];
  float tempMax = root ["temp"]["max"];  
  float wSpeed = root ["speed"];
  int wDeg = root ["deg"];
  weatherDescription = root ["weather"]["description"].as<String>();
  weatherDescription.toLowerCase();
  convertWeatherDes();
  String wDegString;
  if(wDeg >= 345 || wDeg <= 22) wDegString = "\211";     //"Північний";
  if(wDeg >= 23 && wDeg <= 68) wDegString = "\234";      //"Північно-східний";
  if(wDeg >= 69 && wDeg <= 114) wDegString = "\230";     //"Східний";
  if(wDeg >= 115 && wDeg <= 160) wDegString = "\235";    //"Південно-східний";
  if(wDeg >= 161 && wDeg <= 206) wDegString = "\210";    //"Південний";
  if(wDeg >= 207 && wDeg <= 252) wDegString = "\232";    //"Південно-західний";
  if(wDeg >= 253 && wDeg <= 298) wDegString = "\231";    //"Західний";
  if(wDeg >= 299 && wDeg <= 344) wDegString = "\233";    //"Північно-західний";
  weatherStringZ = "Завтра:   \212" + String(tempMin, 1) + ".." + String(tempMax, 1) + "\202" + "C  ";
  weatherStringZ += "  \214 " + wDegString + " " + String(wSpeed, 1) + "м/с   ";
  weatherStringZ += weatherDescription + "        ";
  if(printCom) Serial.println("Getting weather forecast for tomorrow - is OK.");
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void wifiConnect(){
  if(printCom) Serial.print("Connecting WiFi ");
//  printStringWithShift("Підключеня до WiFi   ", 10);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
  for(int i = 0; i < 15; i++){
    if(WiFi.status() == WL_CONNECTED){
      WIFI_connected = true;
      if(printCom) Serial.print("IP address:       ");
      if(printCom) Serial.println(WiFi.localIP());
      String aaa=WiFi.localIP().toString();
      printStringWithShift("IP address: ", 15);
      printStringWithShift(aaa.c_str(), 25);
      timeUpdateNTP();
      return;
    }
    if(printCom) Serial.print(".");
    int j = 0;
    while(j < 500){
      if(j % 10 == 0) showAnimWifi(i);
      j++;
      delay(1);
    }
    clr();
    refreshAll();
  }
  Serial.println("Setting AP mode default parameters");
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssidAP.c_str(), passwordAP.c_str());
  printStringWithShift("Підключіться до точки доступу WiFi-Clock і введіть в браузері адресу: 192.168.4.1         ", 35);
  if(printCom) Serial.print("Wifi ip:");
  if(printCom) Serial.println(WiFi.softAPIP());
}

// =======================================================================
void tvoday(String line){
  String s;
  int strt = line.indexOf('}');
  for(int i = 1; i <= 4; i++){
    strt=line.indexOf('}', strt + 1);
  }
  s = line.substring(2 + strt, line.length());
  tempz = s;
}

// ===========================КОНВЕРТАЦІЯ НАЗВ МІСТ НА УКРАЇНСЬКУ МОВУ============================================
void convertCity(){
  switch(cityId){
    case 756135 : cityName = "Варшава";    break;
    case 703845 : cityName = "Кривий Ріг"; break;
    case 703448 : cityName = "Київ";       break;
    case 698740 : cityName = "Одеса";      break;
    case 709930 : cityName = "Дніпро";     break;
    case 710791 : cityName = "Черкаси";    break;
    case 702550 : cityName = "Львів";      break;
    case 706448 : cityName = "Херсон";     break;
    case 700569 : cityName = "Миколаїв";   break;
    case 687700 : cityName = "Запоріжжя";  break;
    case 706483 : cityName = "Харків";     break;
    case 709717 : cityName = "Донецьк";    break;
    case 702658 : cityName = "Луганськ";   break;
    case 696643 : cityName = "Полтава";    break;
    case 692194 : cityName = "Суми";       break;
    case 710735 : cityName = "Чернігів";   break;
    case 686967 : cityName = "Житомир";    break;
    case 695594 : cityName = "Рівне";      break;
    case 705812 : cityName = "Кропивницький"; break;
    case 690688 : cityName = "Умань";      break;
    case 689558 : cityName = "Вінниця";    break;
    case 706369 : cityName = "Хмельницький"; break;
    case 691650 : cityName = "Тернопіль";  break;
    case 702569 : cityName = "Луцьк";      break;
    case 704617 : cityName = "Ковель";     break;
    case 710719 : cityName = "Чернівці";   break;
    case 707471 : cityName = "Івано-Франківськ"; break;
    case 692372 : cityName = "Стрий";      break;
    case 700646 : cityName = "Мукачево";   break;
    case 690548 : cityName = "Ужгород";    break;
    case 3100946 : cityName = "Ченстохова"; break;
  }
}

// ===========================КОНВЕРТАЦІЯ НАЗВ ДНІВ ТИЖНЯ НА УКРАЇНСЬКУ МОВУ============================================
void convertDw(){
  switch(dayOfWeek){
    case 2 : dw = "Понеділок"; break;
    case 3 : dw = "Вівторок"; break;
    case 4 : dw = "Середа"; break;
    case 5 : dw = "Четверг"; break;
    case 6 : dw = "П'ятниця"; break;
    case 7 : dw = "Субота"; break;
    case 1 : dw = "Неділя"; break;
  }
}

// ===========================КОНВЕРТАЦІЯ НАЗВ МІСЯЦІВ НА УКРАЇНСЬКУ МОВУ============================================
void convertMonth(){
  switch(month){
    case 1 : _month = "січня"; break;
    case 2 : _month = "лютого"; break;
    case 3 : _month = "березня"; break;
    case 4 : _month = "квітня"; break;
    case 5 : _month = "травня"; break;
    case 6 : _month = "червня"; break;
    case 7 : _month = "липня"; break;
    case 8 : _month = "серпня"; break;
    case 9 : _month = "вересня"; break;
    case 10 : _month = "жовтня"; break;
    case 11 : _month = "листопада"; break;
    case 12 : _month = "грудня"; break;
  }
}

// ===========================КОНВЕРТАЦІЯ ХМАРНОСТІ НА УКРАЇНСЬКУ МОВУ============================================
void convertWeatherDes() {
  if(weatherDescription == "clear sky") weatherDescription = "  чисте небо  ";
  else if(weatherDescription == "sky is clear") weatherDescription = "  ясне небо  ";
  else if(weatherDescription == "few clouds") weatherDescription = "  малохмарно  ";
  else if(weatherDescription == "scattered clouds") weatherDescription = "  розірвані хмари  ";
  else if(weatherDescription == "broken clouds") weatherDescription = "  хмарно з проясненнями  ";
  else if(weatherDescription == "overcast clouds") weatherDescription = "  дужа хмарність  ";
  else if(weatherDescription == "light rain") weatherDescription = "  невеликий дощ  ";  
  else if(weatherDescription == "moderate rain") weatherDescription = "  помірний дощ  ";
  else if(weatherDescription == "light intensity shower rain") weatherDescription = "  легкий проливний дощ  "; 
  else if(weatherDescription == "thunderstorm") weatherDescription = "  гроза  ";
  else if(weatherDescription == "haze") weatherDescription = "  туман  ";
  else if(weatherDescription == "fog") weatherDescription = "  туман  ";
  else if(weatherDescription == "mist") weatherDescription = "  густий туман  ";
  else if(weatherDescription == "shower sleet") weatherDescription = "  сніг з дощем  ";
  else if(weatherDescription == "light snow") weatherDescription = "  легкий сніг  ";
  else if(weatherDescription == "light shower snow") weatherDescription = "  невеликий снігопад  "; 
  else if(weatherDescription == "snow") weatherDescription = "  сніг  ";
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void temper() {
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  if(!ds.search(addr)) {
    ds.reset_search();
    delay(250);
    return;
  }
  if(OneWire::crc8(addr, 7) != addr[7]) return; 
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);
  delay(800);
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);
  for(byte i = 0; i < 9; i++){
    data[i] = ds.read();
  }
  int16_t raw = (data[1] << 8) | data[0];
  if(type_s) {
    raw=raw << 3;
    if(data[7] == 0x10) {
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else{
    byte cfg = (data[4] & 0x60);
    if(cfg == 0x00) raw = raw & ~7;
    else if(cfg == 0x20) raw = raw & ~3;
    else if(cfg == 0x40) raw = raw & ~1;
  }
  celsius=(float)raw / 16.00;
  celsius=celsius - 1.50;
  t1 = (int)celsius;
  t2 = (int)(celsius * (celsius < 0 ? -10 : 10)) % 10;
}
