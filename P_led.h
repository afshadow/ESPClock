//
//  HTML PAGE
//
const char PAGE_LedSettings[] PROGMEM =  R"=====(
<meta http-equiv="Content-type" content="text/html; charset=utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Led_clock_weath на ESP-12 ver:5.21 (By IvanZah)</title>
<body onload="load();">
  <div class="container">
    <div class="row" style="text-align:center;">
      <h1 style="margin:50px;">Налаштуваня екрану</h1>
      <div class="col-sm-offset-2 col-sm-8 col-md-offset-3 col-md-6">
        <div class="row">
          <div class="col-sm-6" style="margin-top">
            <div class="alert alert-dismissible alert-warning"style="padding-top:8px; padding-bottom: 9px;"> 
              Денний режим починається з: 
            </div>
          </div>
          <div class="col-sm-6">
            <input id="tbd" value="{{tbd}}" class="form-control" pattern="[0-9]{1,2}" placeholder="Введіть од 0 до 23">
          </div>
        </div>  
        <div class="row">
          <div class="col-sm-6" style="margin-top">
            <div class="alert alert-dismissible alert-warning"style="padding-top:8px; padding-bottom: 9px;"> 
              Денний режим - яскравість: 
            </div>
          </div>
          <div class="col-sm-6">
            <input id="vbd" value="{{vbd}}" class="form-control" pattern="[0-9]{1,2}" placeholder="Введіть од 0 до 15">
          </div>
        </div>  
        <div class="row">
          <div class="col-sm-6" style="margin-top">
            <div class="alert alert-dismissible alert-warning"style="padding-top:8px; padding-bottom: 9px;"> 
              Нічний режим починається з: 
            </div>
          </div>
          <div class="col-sm-6">
            <input id="tbn" value="{{tbn}}" class="form-control" pattern="[0-9]{1,2}" placeholder="Введіть од 0 до 23">
          </div>
        </div>  
        <div class="row">
          <div class="col-sm-6" style="margin-top">
            <div class="alert alert-dismissible alert-warning"style="padding-top:8px; padding-bottom: 9px;"> 
              Нічний режим - яскравість: 
            </div>
          </div>
          <div class="col-sm-6">
            <input id="vbn" value="{{vbn}}" class="form-control" pattern="[0-9]{1,2}" placeholder="Введіть од 0 до 15">
          </div>
        </div> 
        <hr>
        <input class="btn btn-block btn-success" value="Зберегти" onclick="set_led(this);" type="submit">
        <hr>
        <div class="row">
          <div class="col-sm-6" style="margin-top">
            <div class="alert alert-dismissible alert-warning"style="padding-top:8px; padding-bottom: 9px;"> 
              Відображення внутрішньої температури:
            </div>
          </div>
          <div class="col-sm-6">
            <input id="ds18b20" value="{{ds18b20}}" class="form-control" pattern="[0, 1]{1}" placeholder="Введіть 0 або 1">
          </div>
        </div>  
        <div class="row">
          <div class="col-sm-6" style="margin-top">
            <div class="alert alert-dismissible alert-warning"style="padding-top:8px; padding-bottom: 9px;"> 
              Відображення зовнішньої температури:
            </div>
          </div>
          <div class="col-sm-6">
            <input id="ds18b20Out" value="{{ds18b20Out}}" class="form-control" pattern="[0, 1]{1}" placeholder="Введіть 0 або 1">
          </div>
        </div>  
        <div class="row">
          <div class="col-sm-6" style="margin-top">
            <div class="alert alert-dismissible alert-warning"style="padding-top:8px; padding-bottom: 9px;"> 
              Кут повороту екрану:
            </div>
          </div>
          <div class="col-sm-6">
            <input id="rotate" value="{{rotate}}" class="form-control" pattern="[0-9]{1,3}" placeholder="Введіть 0 або 90 або 270">
          </div>
        </div> 
        <div class="row">
          <div class="col-sm-6" style="margin-top">
            <div class="alert alert-dismissible alert-warning"style="padding-top:8px; padding-bottom: 9px;"> 
              Швидкість біжучого рядка:
            </div>
          </div>
          <div class="col-sm-6">
            <input id="speedLine" value="{{speedLine}}" class="form-control" pattern="[0-9]{1,3}" placeholder="Введіть від 1 до 99">
          </div>
        </div> 
        <hr>
        <input class="btn btn-block btn-success" value="Зберегти" onclick="set_led2(this);" type="submit">
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
    function set_led(submit){
      if(val('tbd')<24&&val('vbd')<16&&val('tbn')<24&&val('vbn')<16){
        server = "/led?tbd="+val('tbd')+"&vbd="+val('vbd')+"&tbn="+val('tbn')+"&vbn="+val('vbn');
        send_request(submit,server);}
    }
    function set_led2(submit){
      server = "/led2?ds18b20="+val('ds18b20')+"&ds18b20Out="+val('ds18b20Out')+"&rotate="+val('rotate')+"&speedLine="+val('speedLine');
      send_request(submit,server);
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
