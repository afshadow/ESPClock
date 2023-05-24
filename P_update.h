//
//  HTML PAGE
//
const char PAGE_UpdateSettings[] PROGMEM =  R"=====(
<meta http-equiv="Content-type" content="text/html; charset=utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Led_clock_weath на ESP-12 ver:5.21 (By IvanZah)</title>
<body onload="load();">
  <div class="container">
    <div class="row" style="text-align:center;">
      <h1 style="margin:50px;">Оновлення прошивки ESP</h1>
      <div class="col-sm-offset-2 col-sm-8 col-md-offset-3 col-md-6">
        <div class="alert alert-dismissible alert-info">
          <b>Оновлення: виберіть та завантажте прошивку (bin)</b>
          <form method="POST" action="/update" enctype="multipart/form-data">
            <div class="btn-group">
              <input type="file" class="btn btn-primary btn-xs" name="update" style="height: 33px;">
              <input type="submit" class="btn btn-default btn-sm" value="завантажити" onclick="this.value='Зачекайте...';" style="height: 33px;">
            </div>
          </form>
        </div>
        <hr>
        <div class="alert alert-dismissible alert-info">
          <b style="text-align:center;">Можливість Оновлення прошивки по OTA </b>
          <div>
            <b style="height: 33px;">Статус OTA:  </b>
            <input id="updateOTA" value="{{updateOTA}}" pattern="[0-1]{1,1}" class="form-control">
          </div>
          <input class="btn btn-block btn-success" value="Зберегти" onclick="set_updateOtta(this);" type="submit">
        </div>
        <hr>     
        <a class="btn btn-block btn-default" href="/">Налаштуваня підключеня WiFi</a>
        <a class="btn btn-block btn-default" href="/time.htm">Налаштуваня часу</a>
        <a class="btn btn-block btn-default" href="/weather.htm">Налаштуваня погоди</a>
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
  function set_updateOtta(submit){
    server = "/updateOtta?updateOTA="+val('updateOTA');
    send_request(submit,server);
  //  alert("Пристрій буде перезавантажено!");
  }
  function restart(submit,texts){
    if (confirm(texts)){
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
