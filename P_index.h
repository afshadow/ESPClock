//
//  HTML PAGE
//
const char PAGE_WiFiSettings[] PROGMEM =  R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<meta http-equiv="Content-type" content="text/html; charset=utf-8">
<title>Led_clock_weath на ESP-12 ver:5.21 (By IvanZah)</title>
<body onload="load();">
  <div class="container">
    <div class="row" style="text-align:center;">
      <h1 style="margin:50px;">Налаштуваня підключеня ESP до WiFi</h1>
      <div class="col-sm-offset-2 col-sm-8 col-md-offset-3 col-md-6">
        <h5 class="alert-info" id="ip"><strong>Зараз Ваш IP </strong>{{ip}}</h5>
        <hr>
        <div class="alert alert-dismissible alert-warning">
          <strong>Будь ласка підключіться до вашого роутеру.</strong> Це дозволить керувати вашим пристроєм в локальній мережі і синхронізувати час використовуючи ваш роутер.
        </div>
        <hr>
        <div class="row">
          <div class="col-sm-6" style="margin-top: 12px;">
            <div class="alert alert-dismissible alert-warning"> 
              Підключеня до WiFi роутеру
            </div>
          </div>
          <div class="col-sm-6">
            <input id="ssid" value="{{ssid}}" class="form-control" pattern="[-\w\s#@./]{1,20}" placeholder="Ім'я WiFi мережі">
            <input id="password" value="{{password}}" pattern=".{0,20}" onfocus="this.type='text'" type="password" class="form-control" placeholder="Пароль">
            <input class="btn btn-block btn-success" value="Зберегти" onclick="set_ssid(this);" type="submit">
          </div>
        </div>
        <hr>
        <div class="row" >
          <div class="col-sm-6" style="margin-top: 12px;">
            <div class="alert alert-dismissible alert-warning"style="padding-top:8px; padding-bottom: 9px;">
              Параметри точки доступу вашего ESP (IP_192.168.4.1)
            </div>
          </div>
          <div class="col-sm-6">
            <input id="ssidap" value="{{ssidAP}}" class="form-control" pattern="[-\w\s#@./]{1,20}" placeholder="Ім'я WiFi мережі">
            <input id="passwordAp" value="{{passwordAP}}" pattern=".{0,20}" onfocus="this.type='text'" type="password" class="form-control" placeholder="Пароль">
            <input class="btn btn-block btn-success" value="Зберегти" onclick="set_ssid_ap(this);" type="submit">
          </div>
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
  
  function set_ssid(submit){
      server = "/ssid?ssid="+val('ssid')+"&password="+encodeURIComponent(val('password'));
      send_request(submit,server);
      alert("Зміни вступлять в силу після перезагрузки. Будьласка перезагрузіть пристрій.");
    }
    function set_ssid_ap(submit){
      server = "/ssidap?ssidAP="+val('ssidap')+"&passwordAP="+encodeURIComponent(val('passwordAp'));
      send_request(submit,server);
      alert("Зміни вступлять в силу після перезагрузки. Будьласка перезагрузіть пристрій.");
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
