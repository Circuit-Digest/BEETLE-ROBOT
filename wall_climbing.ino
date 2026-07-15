#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "your_ssid";
const char* password = "your-password";

WebServer server(80);

// ESC
const int escPin = 4;
const int pwmFreq = 50;
const int pwmResolution = 16;

// MX1508
const int L1 = 16;
const int L2 = 17;
const int R1 = 18;
const int R2 = 19;

bool bldcEnabled = false;
int throttle = 0;

uint32_t usToDuty(uint16_t us)
{
  uint32_t maxDuty = (1UL << pwmResolution) - 1;
  return (uint32_t)((us / 20000.0) * maxDuty);
}

void setThrottle(int percent)
{
  if (!bldcEnabled)
  {
    ledcWrite(escPin, usToDuty(1000));
    return;
  }

  percent = constrain(percent, 0, 100);

  uint16_t pulse =
      map(percent, 0, 100, 1000, 2000);

  ledcWrite(escPin, usToDuty(pulse));
}

// --------------------
// Robot Movement
// --------------------

void stopRobot()
{
  digitalWrite(L1, LOW);
  digitalWrite(L2, LOW);
  digitalWrite(R1, LOW);
  digitalWrite(R2, LOW);
}

void forward()
{
  digitalWrite(L1, HIGH);
  digitalWrite(L2, LOW);

  digitalWrite(R1, HIGH);
  digitalWrite(R2, LOW);
}

void backward()
{
  digitalWrite(L1, LOW);
  digitalWrite(L2, HIGH);

  digitalWrite(R1, LOW);
  digitalWrite(R2, HIGH);
}

void left()
{
  digitalWrite(L1, LOW);
  digitalWrite(L2, HIGH);

  digitalWrite(R1, HIGH);
  digitalWrite(R2, LOW);
}

void right()
{
  digitalWrite(L1, HIGH);
  digitalWrite(L2, LOW);

  digitalWrite(R1, LOW);
  digitalWrite(R2, HIGH);
}

void handleRoot()
{
  String page = R"rawliteral(
<!DOCTYPE html>
<html>

<head>

<meta name="viewport" content="width=device-width, initial-scale=1">

<style>

body{
    font-family:Arial;
    background:#f4f4f4;
    text-align:center;
}

h2{
    margin-top:20px;
}

.blueBtn{
    background:#007BFF;
    color:white;
    border:none;
    padding:15px 28px;
    font-size:18px;
    border-radius:8px;
    margin:8px;
    cursor:pointer;
}

.blueBtn:hover{
    background:#0056b3;
}

.slider{
    width:85%;
    margin-top:10px;
}

#val{
    color:#007BFF;
}

.controller{
    width:260px;
    margin:25px auto;
}

.row{
    display:flex;
    justify-content:center;
    margin:8px 0;
}

.ctrlBtn{
    width:80px;
    height:80px;
    font-size:18px;
    border:none;
    border-radius:12px;
    background:#4CAF50;
    color:white;
    cursor:pointer;
    margin:5px;
}

.stopBtn{
    background:#E53935;
}

.ctrlBtn:hover{
    opacity:0.85;
}

</style>

</head>

<body>

<h2>Wall Climbing Robot</h2>

<h3>BLDC Suction Control</h3>

<button class="blueBtn"
onclick="fetch('/bldcOn')">
ON
</button>

<button class="blueBtn"
onclick="fetch('/bldcOff')">
OFF
</button>

<br><br>

<input
type="range"
min="0"
max="100"
value="0"
class="slider"
id="speed">

<h2 id="val">0%</h2>

<hr>

<h3>Movement Control</h3>

<div class="controller">

    <div class="row">
        <button class="ctrlBtn"
        onclick="fetch('/forward')">
        FWD
        </button>
    </div>

    <div class="row">

        <button class="ctrlBtn"
        onclick="fetch('/left')">
        LEFT
        </button>

        <button class="ctrlBtn stopBtn"
        onclick="fetch('/stop')">
        STOP
        </button>

        <button class="ctrlBtn"
        onclick="fetch('/right')">
        RIGHT
        </button>

    </div>

    <div class="row">
        <button class="ctrlBtn"
        onclick="fetch('/backward')">
        BKWD
        </button>
    </div>

</div>

<script>

var slider=document.getElementById("speed");
var val=document.getElementById("val");

slider.oninput=function(){

    val.innerHTML=this.value+" %";

    fetch("/speed?value="+this.value);

}

</script>

</body>
</html>
)rawliteral";

  server.send(200, "text/html", page);
}

// --------------------
// Web Requests
// --------------------

void handleSpeed()
{
  throttle =
    server.arg("value").toInt();

  setThrottle(throttle);

  server.send(200, "text/plain", "OK");
}

void handleBLDCOn()
{
  bldcEnabled = true;
  setThrottle(throttle);

  server.send(200, "text/plain", "ON");
}

void handleBLDCOff()
{
  bldcEnabled = false;
  setThrottle(0);

  server.send(200, "text/plain", "OFF");
}

// --------------------
// Setup
// --------------------

void setup()
{
  Serial.begin(115200);

  Serial.println("Serial connected successfully!");

  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);

  stopRobot();

  ledcAttach(
    escPin,
    pwmFreq,
    pwmResolution);

  ledcWrite(
    escPin,
    usToDuty(1000));

  delay(5000); // arm ESC

  WiFi.begin(ssid, password);

  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.on("/speed", handleSpeed);

  server.on("/bldcOn", handleBLDCOn);
  server.on("/bldcOff", handleBLDCOff);

  server.on("/forward",
    [](){ forward(); server.send(200); });

  server.on("/backward",
    [](){ backward(); server.send(200); });

  server.on("/left",
    [](){ left(); server.send(200); });

  server.on("/right",
    [](){ right(); server.send(200); });

  server.on("/stop",
    [](){ stopRobot(); server.send(200); });

  server.begin();
}

void loop()
{
  server.handleClient();
}