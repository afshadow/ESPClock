//
//  HTML PAGE
//
const char PAGE_TimeSettings[] PROGMEM =  R"=====(
<meta http-equiv="Content-type" content="text/html; charset=utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Led_clock_weath на ESP-12 ver:5.21 (By IvanZah)</title>
<body onload="load();">
  <div class="container">
    <div class="row" style="text-align:center;">
      <h1 style="margin:50px;">Налаштуваня часу на ESP</h1>
      <div class="col-sm-offset-2 col-sm-8 col-md-offset-3 col-md-6">
        <h5 class="alert-info" id="time">{{time}}</h5>
        <hr>
        <div class="row">
          <div class="col-sm-6">
            <div class="alert alert-dismissible alert-warning"> 
              Сервер точного часу типу "time.nist.gov"
            </div>
          </div>
          <div class="col-sm-6" > 
            <input id="ntpServerName" value="{{ntpServerName}}" class="form-control" pattern="[0-9a-zA-Z. ]{1,20}" placeholder="Сервер часу">
            <input class="btn btn-block btn-success" value="Зберегти" onclick="set_ntpServerName(this);" type="submit">
          </div>
        </div>
        <hr>
        <div class="row">
          <div class="col-sm-6" style="margin-top: 12px;">
            <div class="alert alert-dismissible alert-warning"> 
              Часова зона GMT
            </div>
          </div>
          <div class="col-sm-6">
            <input id="timeZone" value="{{timeZone}}" pattern="[0-9]{1,3}" class="form-control">
            <input class="btn btn-block btn-success" value="Зберегти" onclick="time_zone(this);" type="submit">
            <input class="btn btn-block btn-primary" value="Авто визначеня та збереження зони" onclick="set_time_zone(this);" type="submit">
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
  var set_real_time;
  function handleServerResponse(){
    clearTimeout(set_real_time);
    var res = jsonResponse.time.split(":");
    real_time(hours=res[0],min=res[1],sec=res[2]);
    document.body.style.backgroundColor="rgb("+jsonResponse.rgb+")";
  }
  function real_time(hours,min,sec){
    sec=Number(sec)+1;
    if (sec>=60){min=Number(min)+1;sec=0;}
    if (min>=60){hours=Number(hours)+1;min=0;}
    if (hours>=24){hours=0};
    document.getElementById("time").innerHTML = "Час на ESP "+hours+":"+(min<10?"0":"")+min+":"+(sec<10?"0":"")+sec;
    set_real_time = setTimeout("real_time("+hours+","+min+","+sec+");", 1000);
  }
  function load_time(submit){
    server = "/Time";
    send_request(submit,server);
    load();
  }
  function set_ntpServerName(submit){
    server = "/ntpServerName?ntpServerName="+val('ntpServerName');
    send_request(submit,server);
  }
  function time_zone(submit){if(val('timeZone')<13) {server = "/timeZone?timeZone="+val('timeZone'); send_request(submit,server);}}
  function set_time_zone(submit){
    var set_date = new Date();
    var gmtHours = -set_date.getTimezoneOffset()/60;
    document.getElementById('timeZone').value = gmtHours;
    server = "/timeZone?timeZone="+gmtHours;
    send_request(submit,server);
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
