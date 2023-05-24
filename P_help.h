//
//  HTML PAGE
//
const char PAGE_Help[] PROGMEM =  R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<meta http-equiv="Content-type" content="text/html; charset=utf-8">
<title>Led_clock_weath на ESP-12 ver:5.21 (By IvanZah)</title>
  <div class="container">
    <div class="row" style="text-align:center;">
      <h1 style="margin:50px;">Якщо щось пішло не так...</h1>
      <div class="col-sm-offset-2 col-sm-8 col-md-offset-3 col-md-6">
        <div class="alert alert-dismissible alert-warning">
          <strong>УВАГА!!! Деякі зміни можуть бути незворотніми!!!</strong>
        </div>
        <b>Якщо ви щось не вірно ввели і не можете цьoго змінити, то поверніть пристрій до заводських налаштувань. Файл конфігурації буде видалено. Пристрій перезавантажено</b>
        <a class="btn btn-block btn-default" href="/configs.json">Відкрити файл конфігурації</a>
        <hr>
        <b>Якщо деякі браузери не відкривають HTML сторінки вашої ESP, спробуйте вимкнути можливість прошивки пристрою по ОТА, вписавши "0" на відповідній сторінці налаштувань.  </b>
        <hr>
        <a class="btn btn-block btn-default" href="/">Налаштуваня підключеня WiFi</a>
        <a class="btn btn-block btn-default" href="/time.htm">Налаштуваня часу</a>
        <a class="btn btn-block btn-default" href="/weather.htm">Налаштуваня погоди</a>
        <a class="btn btn-block btn-default" href="/mqtt.htm">Налаштуваня MQTT серверу</a>
        <a class="btn btn-block btn-default" href="/led.htm">Налаштуваня екрану</a>
        <a class="btn btn-block btn-default" href="/update.htm">Оновлення прошивки</a>
        <a class="btn btn-block btn-default" href="/help.htm">Поміч</a>
        <input class="btn btn-block btn-danger" value="Повернутися до заводських налаштувань" onclick="resetConfig(this,'Ви дійсно бажаєте повернутися до заводських налаштувань?');" type="submit">
      </div>
    </div>
  </div>

<head>
  <link rel="stylesheet" href="/bootstrap.min.css">
  <link rel="stylesheet" type="text/css" href="/style.css">
  <script type="text/javascript" src="/function.js"></script>
  <script type="text/javascript">
  function resetConfig(submit,texts){
    if(confirm(texts)){
      server = "/resetConfig?device=ok";
      send_request(submit,server);
      return true;
    } else {
      return false;
    }
  }
  function restart(submit,texts){
    if(confirm(texts)){
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
