#include <WiFi.h>
#include <WebServer.h>
#include "RoboClaw.h"

RoboClaw roboclaw(&Serial2, 10000);

const char* pw = "basicmicro";
const char* ap_name = "esp32";

WebServer server(80);

const char home_page[] = R"=====(
<!DOCTYPE html>
<html>
<head>
<script>
  function sendCommand(chan,state) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
 
      }
    };
    xhttp.open("GET", "handleMotor?motorChannel="+chan+"&motorState="+state, true);
    xhttp.send();
  }

  setInterval(function(){
   getEnc_1();
  }, 200);

  setInterval(function(){
   getEnc_2();
  }, 200);

   setInterval(function(){
   getTemp();
  }, 5000);

   setInterval(function(){
   getVoltage();
  }, 1500);

  function getEnc_1() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if(this.readyState == 4 && this.status == 200){
        document.getElementById("encoder_1_value").innerHTML = this.responseText;
      }
    };
    xhttp.open("POST","readEncoder_1",true);
    xhttp.send();
  }

  function getEnc_2() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if(this.readyState == 4 && this.status == 200){
        document.getElementById("encoder_2_value").innerHTML = this.responseText;
      }
    };
    xhttp.open("POST","readEncoder_2",true);
    xhttp.send();
  }

  function getTemp() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if(this.readyState == 4 && this.status == 200){
        document.getElementById("temp").innerHTML = this.responseText;
      }
    };
    xhttp.open("POST","readTemp",true);
    xhttp.send();
  }

  function getVoltage() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if(this.readyState == 4 && this.status == 200){
        document.getElementById("voltage").innerHTML = this.responseText;
      }
    };
    xhttp.open("POST","readVoltage",true);
    xhttp.send();
  }
</script>

<style>
      .round {
        border-radius: 25px;
        background-color: #cde6f5;
        width: 25%;
        height: 25%;
        margin: 20px;
        padding: 20px;
      }

      h2 {
        color: #707078;
      }

      .button {
        background-color: #87919e;
        display: inline-block;
        border-radius: 10px;
        border: none;
        color: white;
        font-size: 15px;
        padding: 25px;
      }

      span {
        color: #707078;
      }

      p {
        color: #707078;
        font-size: 25px;
      }
    </style>
</head
<body>

    <div class="round">
      <h2>Motor Channel 1</h2>
      <button type="button" class="button" onclick="sendCommand('1','1')">Motor On</button>
        <button type="button" class="button" onclick="sendCommand('1','0')">Motor Off</button>
        <br />
        <br />
        <p>Encoder Count: <span id="encoder_1_value">0</span></p>
    </div>

    <div class="round">
      <h2>Motor Channel 2</h2>
      <button type="button" class="button" onclick="sendCommand('2','1')">Motor On</button>
        <button type="button" class="button" onclick="sendCommand('2','0')">Motor Off</button>
        <br />
        <br />
        <p>Encoder Count: <span id="encoder_2_value">0</span></p>
    </div>

    <div class="round">
      <h2>Info</h2>
        <p>Voltage: <span id="voltage">0</span>V</p>
        <p>Temperature: <span id="temp">0</span>C</p>
    </div>
    
</body>
</html>

)=====";

void handleHome(){
  String page = home_page;
  server.send(200, "text/html",page);
}

void handleEncoder_1(){
  int enc_value = roboclaw.ReadEncM1(0x80);
  String value = String(enc_value);
  server.send(200, "text/plain", value);
    
}

void handleEncoder_2(){
  int enc_value = roboclaw.ReadEncM2(0x80);
  String value = String(enc_value);
  server.send(200, "text/plain", value);
    
}

void handleMotor(){
  String motor_channel = server.arg("motorChannel");
  String motor_state = server.arg("motorState");
  if(motor_channel == "1") {
    if(motor_state == "1") {
      roboclaw.ForwardM1(0x80, 64);
    }
    else if(motor_state == "0") {
      roboclaw.ForwardM1(0x80, 0);
    }
  }

  else if(motor_channel == "2") {
     if(motor_state == "1") {
      roboclaw.ForwardM2(0x80, 64);
    }
    else if(motor_state == "0") {
      roboclaw.ForwardM2(0x80, 0);
    }
  }
  
}

void handleTemp() {
  uint16_t temp = 0;
  roboclaw.ReadTemp(0x80, temp);
  float temp_f = (float)temp/10;
  String temp_str = String(temp_f);
  server.send(200, "text/plain", temp_str);
}

void handleVoltage() {
  int voltage = 0;
  voltage = roboclaw.ReadMainBatteryVoltage(0x80);
  float voltage_f = (float)voltage/10;
  String voltage_str = String(voltage_f);
  server.send(200, "text/plain", voltage_str);
}

void setup() {
  roboclaw.begin(38400);
  WiFi.softAP(ap_name, pw);

  server.on("/", handleHome);
  server.on("/readEncoder_1", handleEncoder_1);
  server.on("/readEncoder_2", handleEncoder_2);
  server.on("/readTemp", handleTemp);
  server.on("/readVoltage", handleVoltage);
  server.on("/handleMotor", handleMotor);

  server.begin();

}

void loop() {
  server.handleClient();
  delay(1);
}
