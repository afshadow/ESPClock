//
//  HTML PAGE
//
const char PAGE_WertherSettings[] PROGMEM =  R"=====(
<meta http-equiv="Content-type" content="text/html; charset=utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Led_clock_weath на ESP-12 ver:5.21 (By IvanZah)</title>
<body onload="load();">
  <div class="container">
    <div class="row" style="text-align:center;">
      <h1 style="margin:50px;">Налаштуваня погоди на ESP</h1>
      <div class="col-sm-offset-2 col-sm-8 col-md-offset-3 col-md-6">
        <div class="row">
          <div class="col-sm-6" style="margin-top: 12px;">
            <div class="alert alert-dismissible alert-warning"> 
              Сервер погоди
            </div>
          </div>
          <div class="col-sm-6">
            <input id="weatherHost" value="{{weatherHost}}" class="form-control" pattern="[0-9a-zA-Z.]{1,32}" placeholder="Сервер погоди">
            <input class="btn btn-block btn-success" value="Зберегти" onclick="set_weatherHost(this);" type="submit">
          </div>
        </div>  
        <hr>
        <div class="row">
          <div class="col-sm-6" style="margin-top: 12px;">
            <div class="alert alert-dismissible alert-warning"> 
              Ключ погоди API
            </div>
          </div>
          <div class="col-sm-6">
            <input id="weatherKey" value="{{weatherKey}}" class="form-control" pattern="[0-9a-zA-Z.]{1,32}" placeholder="Ключ погоди">
            <input class="btn btn-block btn-success" value="Зберегти" onclick="set_weatherKey(this);" type="submit">
          </div>
        </div>  
        <a href="http://openweathermap.org/api" align="center" target="_blank">Тут берем API ключ</a>
        <hr>
        <div class="row">
          <div class="col-sm-6" style="margin-top: 12px;">
            <div class="alert alert-dismissible alert-warning"> 
              ID Міста
            </div>
          </div>
          <div class="col-sm-6">          
            <input id="cityID1" value="{{cityID1}}" class="form-control" pattern="[0-9]{1,10}" placeholder="ID Міста1">
            <input id="cityID2" value="{{cityID2}}" class="form-control" pattern="[0-9]{1,10}" placeholder="ID Міста2">
            <input class="btn btn-block btn-success" value="Зберегти" onclick="set_cityID(this);" type="submit">
          </div>
        </div>  
        <a href="http://openweathermap.org/help/city_list.txt" align="center" target="_blank">Тут знаходимо cityID свого міста</a>
        <hr>
        Якщо ви бажаєте не використовувати якесь місто, то в полі його ID введіть "0"
        <hr>
        <div class="row">
          <div class="col-sm-6" style="margin-top">
            <div class="alert alert-dismissible alert-warning"style="padding-top:8px; padding-bottom: 9px;"> 
              Відображення прогнозу погоди (0 або 1):
            </div>
          </div>
          <div class="col-sm-6">
            <input id="forecastView" value="{{forecastView}}" class="form-control" pattern="[0, 1]{1}" placeholder="Введіть 0 або 1">
          </div>
        </div>  
        <div class="row">
          <div class="col-sm-6" style="margin-top">
            <div class="alert alert-dismissible alert-warning"style="padding-top:8px; padding-bottom: 9px;"> 
              Відображення погоди починається з: 
            </div>
          </div>
          <div class="col-sm-6">
            <input id="forecastViewStart" value="{{forecastViewStart}}" class="form-control" pattern="[0-9]{1,2}" placeholder="Введіть од 0 до 23">
          </div>
        </div>  
        <div class="row">
          <div class="col-sm-6" style="margin-top">
            <div class="alert alert-dismissible alert-warning"style="padding-top:8px; padding-bottom: 9px;"> 
              Відображення погоди закінчуєтся після: 
            </div>
          </div>
          <div class="col-sm-6">
            <input id="forecastViewStop" value="{{forecastViewStop}}" class="form-control" pattern="[0-9]{1,2}" placeholder="Введіть од 0 до 23">
          </div>
        </div>  
        <input class="btn btn-block btn-success" value="Зберегти" onclick="set_forecastView(this);" type="submit">
        <hr>     
        <div class="alert alert-dismissible alert-warning">
          Погода оновлюється в 15 та в 45 хвилин кожної години. Якщо ви бажаєте оновити її зараз то натисніть:
          <input class="btn btn-block btn-success" value="ОНОВИТИ" onclick="set_weather_up(this,'Оновити погоду зараз?');" type="submit">
        </div>  
        <hr>     
        <a class="btn btn-block btn-default" href="/">Налаштуваня підключеня WiFi</a>
        <a class="btn btn-block btn-default" href="/time.htm">Налаштуваня часу</a>
        <a class="btn btn-block btn-default" href="/weather.htm">Налаштуваня погоди</a>
        <a class="btn btn-block btn-default" href="/mqtt.htm">Налаштуваня MQTT серверу</a>
        <a class="btn btn-block btn-default" href="/led.htm">Налаштуваня екрану</a>
        <a class="btn btn-block btn-default" href="/update.htm">Оновлення прошивки</a>
        <a class="btn btn-block btn-default" href="/help.htm">Поміч</a>
        <input class="btn btn-block btn-danger" value="Перезавантажити пристрій" onclick="restart(this,'Ви дійсно бажаєте Перезавантажити пристрій?');" type="submit">
      </div>
    </div>
  </div>
</body>
<head>
  <link rel="stylesheet" href="/bootstrap.min.css">
  <link rel="stylesheet" type="text/css" href="/style.css">
  <script type="text/javascript" src="/function.js"></script>
  <script type="text/javascript">
  var set_real_time;
  function set_weatherKey(submit){
    server = "/weatherKey?weatherKey="+val('weatherKey');
    send_request(submit,server);
  }
  function set_weatherHost(submit){
    server = "/weatherHost?weatherHost="+val('weatherHost');
    send_request(submit,server);
  }
  function set_cityID(submit){
    server = "/cityID?cityID1="+val('cityID1')+"&cityID2="+val('cityID2');
    send_request(submit,server);
  }
  function set_forecastView(submit){
    server = "/forecastView?forecastView="+val('forecastView')+"&forecastViewStart="+val('forecastViewStart')+"&forecastViewStop="+val('forecastViewStop');
    send_request(submit,server);
  }
  function set_weather_up(submit,texts){
    if (confirm(texts)) {
      server = "/weatherUpdate?update=ok";
      send_request(submit,server);
      return true;
    } else {
        return false;
    }
  }
  function restart(submit,texts){
    if (confirm(texts)) {
      server = "/restart?device=ok";
      send_request(submit,server);
      return true;
    } else {
      return false;
    }
  }
  </script>
</head>
)=====";
