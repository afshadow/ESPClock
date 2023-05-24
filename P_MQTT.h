//
//  HTML PAGE
//
const char PAGE_MQTTSettings[] PROGMEM =  R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<meta http-equiv="Content-type" content="text/html; charset=utf-8">
<title>Led_clock_weath на ESP-12 ver:5.55 (By IvanZah)</title>
<body onload="load();">
  <div class="container">
    <div class="row" style="text-align:center;">
      <h1 style="margin:50px;">Налаштуваня підключеня ESP до MQTT серверу</h1>
      <div class="col-sm-offset-2 col-sm-8 col-md-offset-3 col-md-6">
        <div class="alert alert-dismissible alert-warning">
          Якщо ви бажаєте передавати повідомлення про температуру з вашого датчика DS18B20, та отримувати повідомлення від зовнішньго датчика, то виконайте налаштування MQTT серверу.
          Ви можете використать завнішній сервер, наприклад: <a href="https://www.cloudmqtt.com" align="center" target="_blank"> cloudmqtt.com</a>
        </div>
        <hr>
        <div class="row">
          <div class="col-sm-6" style="margin-top">
            <div class="alert alert-dismissible alert-warning"style="padding-top:8px; padding-bottom: 9px;"> 
              Підключеня до MQTT серверу:
            </div>
          </div>
          <div class="col-sm-6">
            <input id="mqtt_server" value="{{mqtt_server}}" class="form-control" pattern="[-\w\s#@./]{1,20}" placeholder="MQTT сервер">
          </div>
        </div>
        <div class="row" >
          <div class="col-sm-6" style="margin-top">
            <div class="alert alert-dismissible alert-warning"style="padding-top:8px; padding-bottom: 9px;">
              Порт MQTT серверу:
            </div>
          </div>
          <div class="col-sm-6">
            <input id="mqtt_port" value="{{mqtt_port}}" class="form-control" pattern="[\d]{1,8}" placeholder="MQTT порт">
          </div>
        </div>
        <div class="row">
          <div class="col-sm-6" style="margin-top">
            <div class="alert alert-dismissible alert-warning"style="padding-top:8px; padding-bottom: 9px;"> 
              Логін користувача:
            </div>
          </div>
          <div class="col-sm-6">
            <input id="mqtt_user" value="{{mqtt_user}}" class="form-control" pattern="[-\w\s#@./]{0,30}" placeholder="MQTT логін">
          </div>
        </div>
        <div class="row" >
          <div class="col-sm-6" style="margin-top">
            <div class="alert alert-dismissible alert-warning"style="padding-top:8px; padding-bottom: 9px;">
              Пароль користувача:
            </div>
          </div>
          <div class="col-sm-6">
            <input id="mqtt_pass" value="{{mqtt_pass}}" class="form-control" pattern="[-\w\s#@./]{0,20}" placeholder="MQTT пароль">
          </div>
        </div>
        <input class="btn btn-block btn-success" value="Зберегти" onclick="set_mqtt_ust(this);" type="submit">
        <hr>
        <div class="row">
          <div class="col-sm-6" style="margin-top">
            <div class="alert alert-dismissible alert-warning"style="padding-top:8px; padding-bottom: 9px;"> 
              Назва пристрою (ESP):
            </div>
          </div>
          <div class="col-sm-6">
            <input id="mqtt_name" value="{{mqtt_name}}" class="form-control" pattern="[-\w\s#@./]{1,20}" placeholder="Назва пристрою">
          </div>
        </div>
        <div class="row" >
          <div class="col-sm-6" style="margin-top">
            <div class="alert alert-dismissible alert-warning"style="padding-top:8px; padding-bottom: 9px;">
              Топік для отримання температури зовнішнього датчика:
            </div>
          </div>
          <div class="col-sm-6">
            <input id="mqtt_sub" value="{{mqtt_sub}}" class="form-control" pattern="[-\w\s#@./]{1,20}" placeholder="Температура на вулиці">
          </div>
        </div>
        <div class="row">
          <div class="col-sm-6" style="margin-top">
            <div class="alert alert-dismissible alert-warning"style="padding-top:8px; padding-bottom: 9px;"> 
              Топік для отримання інформ повідомлень:
            </div>
          </div>
          <div class="col-sm-6">
            <input id="mqtt_sub_inform" value="{{mqtt_sub_inform}}" class="form-control" pattern="[-\w\s#@./]{1,20}" placeholder="Топік інформера">
          </div>
        </div>
        <div class="row" >
          <div class="col-sm-6" style="margin-top">
            <div class="alert alert-dismissible alert-warning"style="padding-top:8px; padding-bottom: 9px;">
              Топік для публікації температури:
            </div>
          </div>
          <div class="col-sm-6">
            <input id="mqtt_pub" value="{{mqtt_pub}}" class="form-control" pattern="[-\w\s#@./]{1,20}" placeholder="Публикація температури">
          </div>
        </div>
        <input class="btn btn-block btn-success" value="Зберегти" onclick="set_mqtt_topic(this);" type="submit">
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
    function set_mqtt_ust(submit) {
      server = "/mqttUst?mqtt_server="+val('mqtt_server')+"&mqtt_port="+(val('mqtt_port'))+"&mqtt_user="+(val('mqtt_user'))+"&mqtt_pass="+(val('mqtt_pass'));
      send_request(submit,server);
    }
    function set_mqtt_topic(submit) {
      server = "/mqttTopic?mqtt_name="+(val('mqtt_name'))+"&mqtt_sub="+(val('mqtt_sub'))+"&mqtt_sub_inform="+(val('mqtt_sub_inform'))+"&mqtt_pub="+(val('mqtt_pub'));
      send_request(submit,server);
    }
    function restart(submit,texts){
      if(confirm(texts)) {
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
